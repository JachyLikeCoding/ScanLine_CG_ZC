#include "ObjLoader.h"

int Round(GLfloat x) {
	if (x > 0) {
		return (int)(x + 0.5);
	}
	else {
		return (int)(x - 0.5);
	}
}

void Object::initObject(const string &objName, int _width, int _height, int _mode) {
	winWidth = _width;
	winHeight = _height;
	mode = _mode;
	bool flag = loadObj(objName);
	colorlist.resize(faces.size());
	
	if (!flag) 
	{
		cerr << "ERROR: File loaded fail. Please check your file '" << objName << " '.\n";
	}
}


bool Object::loadObj(const string &objName) {
	fstream input;
	input.open(objName);

	if (!input.is_open()) {
		cerr << "Fail to open the input file ' " << objName << " '" << endl;
		return false;
	}
	
	string head;
	GLfloat x, y, z;
	minY = INT_MAX;
	minX = INT_MAX;
	maxY = INT_MIN;
	maxX = INT_MIN;

	while (input >> head) {
		if (input.eof())
			break;
		else {
			if (head == "v") {
				input >> x >> y >> z;
				originvertices.push_back(vec3(x,y,z));
				
				minY = minY > y ? y : minY;
				minX = minX > x ? x : minX;
				maxY = maxY > y ? maxY : y;
				maxX = maxX > x ? maxX : x;
			}
			else if (head == "f") {
				string::iterator iter;
				vector<int> vid(3);
				vector<string> tmps(3);
				input >> tmps[0] >> tmps[1] >> tmps[2];
				for (int i = 0; i < 3; i++) {
					iter = find(tmps[i].begin(), tmps[i].end(), '/');
					vid[i] = stoi(tmps[i].substr(0, iter - tmps[i].begin()));//注意obj文件里顶点序号从1或-1开始
					vid[i] > 0 ? (vid[i]--) : (vid[i] += originvertices.size());//这里把它统一成0,1,2,3序号
				}
				faces.push_back(vid);
			}
			else
			{
				continue;
			}
		}
	}

	ChangeScreenSize();
	//just for debug:
	cout << originvertices.size() << endl;
	cout << faces.size() << endl;
	cout << "+++++++++++++before change vertices:" << endl;
	cout << "minY:-----------------" << minY << endl;
	cout << "maxY:-----------------" << maxY << endl;
	cout << "minX:-----------------" << minX << endl;
	cout << "maxX:-----------------" << maxX << endl;
	cout << "-------------------------after change vertices:" << endl;
	//getScreenPos(originvertices, vertices);
	ChangeOriginvertices();
	cout << "minY:-----------------" << minY << endl;
	cout << "maxY:-----------------" << maxY << endl;
	cout << "minX:-----------------" << minX << endl;
	cout << "maxX:-----------------" << maxX << endl;
	
	return true;
}



ClassifiedEdge Object::CalEdge(int polygon_id, int v1_id, int v2_id, ClassifiedEdge edge) {
	vec3 v1 = vertices[v1_id];
	vec3 v2 = vertices[v2_id];
	edge.edge_polygon_id = polygon_id;
	edge.x = v1.y > v2.y ? v1.x : v2.x;//边的上端点的x坐标
	
	edge.dy = ((int)(v1.y + 0.5f) - (int)(v2.y + 0.5f));
	if (edge.dy == 0) {
		cout << "dy = 0" << endl;
		system("pause");
	}
	edge.dx = -(v2.x - v1.x) / (v2.y - v1.y);
	edge.maxY = v1.y > v2.y ? (int)(v1.y + 0.5f) : (int)(v2.y + 0.5f);
	return edge;
}

void Object::CalFace(int face_id, GLfloat &a, GLfloat &b, GLfloat &c, GLfloat &d, int &maxY, GLfloat &maxZ, int &dy, vec3 &color) {
	int v1_id = faces[face_id][0];//three vertices of the face
	int v2_id = faces[face_id][1];
	int v3_id = faces[face_id][2];
	//step1: calculate parameters a,b,c,d
	a = (float)((vertices[v2_id].y - vertices[v1_id].y)*(vertices[v3_id].z - vertices[v1_id].z)
		- (vertices[v2_id].z - vertices[v1_id].z)*(vertices[v3_id].y - vertices[v1_id].y));

	b = (float)((vertices[v2_id].z - vertices[v1_id].z)*(vertices[v3_id].x - vertices[v1_id].x)
		- (vertices[v2_id].x - vertices[v1_id].x)*(vertices[v3_id].z - vertices[v1_id].z));

	c = (float)((vertices[v2_id].x - vertices[v1_id].x)*(vertices[v3_id].y - vertices[v1_id].y)
		- (vertices[v2_id].y - vertices[v1_id].y)*(vertices[v3_id].x - vertices[v1_id].x));

	GLfloat sum = abs(a) + abs(b) + abs(c);
	/*a = a / sum;
	b = b / sum;
	c = c / sum;*/
	d = (float)(0 - (a*vertices[v1_id].x + b * vertices[v1_id].y + c * vertices[v1_id].z));

	//step2: calculate maxZ and maxY
	maxZ = (vertices[v1_id].z > vertices[v2_id].z) ? vertices[v1_id].z : vertices[v2_id].z;
	maxZ = (maxZ > vertices[v3_id].z) ? maxZ : vertices[v3_id].z;

	float max_Y = (vertices[v1_id].y > vertices[v2_id].y) ? vertices[v1_id].y : vertices[v2_id].y;
	max_Y = (max_Y > vertices[v3_id].y) ? max_Y : vertices[v3_id].y;

	//step 3: calculate dy
	float minY = (vertices[v1_id].y < vertices[v2_id].y) ? vertices[v1_id].y : vertices[v2_id].y;
	minY = (minY < vertices[v3_id].y) ? minY : vertices[v3_id].y;
	
	maxY = (int)(max_Y + 0.5);
	minY = (int)(minY + 0.5);
	dy = maxY - minY;
	//step4: calculate color
	
	GLfloat p0 = abs(a);
	GLfloat p1 = abs(b);
	GLfloat p2 = abs(c);
	GLfloat cosTheta = p2 / sqrt(p0 + p1 + p2);
	/*color.x = cosTheta;
	color.y = cosTheta;
	color.z = cosTheta;*/
	color.x = rand() % 100 / (float)(100);
	color.y = rand() % 100 / (float)(100);
	color.z = rand() % 100 / (float)(100);
	/*color.y = cosTheta;
	color.z = cosTheta;*/

	//cout << face_id << " face color : " << color.x << "\t" << color.y << "\t" << color.z << endl;
}

void Object::CalFaceEdges(int face_id) {
	ClassifiedEdge edge1, edge2, edge3;

	int v1, v2, v3;// v1.y > v2.y > v3.y
	int v1_id = faces[face_id][0];//three vertices of the face
	int v2_id = faces[face_id][1];
	int v3_id = faces[face_id][2];

	if (vertices[v1_id].y < vertices[v2_id].y) {
		if (vertices[v2_id].y < vertices[v3_id].y) {
			v1 = v3_id;
			v2 = v2_id;
			v3 = v1_id;
		}else {
			if (vertices[v1_id].y < vertices[v3_id].y) {
				v1 = v2_id;
				v2 = v3_id;
				v3 = v1_id;
			}else {
				v1 = v2_id;
				v2 = v1_id;
				v3 = v3_id;
			}
		}
	}else {
		if (vertices[v1_id].y < vertices[v3_id].y) {
			v1 = v3_id;
			v2 = v1_id;
			v3 = v2_id;
		}else {
			if (vertices[v2_id].y < vertices[v3_id].y) {
				v1 = v1_id;
				v2 = v3_id;
				v3 = v2_id;
			}else {
				v1 = v1_id;
				v2 = v2_id;
				v3 = v3_id;
			}
		}
	}
	//没有把平行于扫描线的边加入分类边表
	if(vertices[v1].y != vertices[v2].y)
		edges.push_back(CalEdge(face_id, v1, v2, edge1));
	if (vertices[v1].y != vertices[v3].y)
		edges.push_back(CalEdge(face_id, v1, v3, edge2));
	if (vertices[v2].y != vertices[v3].y)
		edges.push_back(CalEdge(face_id, v2, v3, edge3));
}

//just for debug
void Object::test() {
	//test vertices of face
	cout << "face count: " << faces.size() << endl;
	cout << "vertices count:" << originvertices.size() << endl;
	/*for (int i = 0; i < faces.size(); i++) {
		CalFaceEdges(i);
		cout << "[" << i << "]";
		for (int j = 0; j < faces[i].size(); j++) {
			cout << faces[i][j] << "\t";
		} 
		cout << endl;
	}*/
	//test edges of face:
	/*cout << edges.size() << endl;
	for (int i = 0; i < edges.size(); i++) {
		cout << "edge_polygon_id: " << edges[i].edge_polygon_id;
		cout << "\tedge_dy: " << edges[i].dy << endl;
	}*/
}

void Object::ChangeScreenSize() {
	GLfloat dx = round(maxX) - round(minX);
	GLfloat dy = round(maxY) - round(minY);
	if (dx > dy) {	//winWidth不变
		winHeight = (int)((winWidth) * dy / dx);
	}
	else {	//winHeight不变
		winWidth = (int)((winHeight)* dx / dy);
	}
}



void Object::ChangeOriginvertices() {
	float xDis = maxX - minX;
	float yDis = maxY - minY;
	float xScale = (float)(xDis / winWidth);
	float yScale = (float)(yDis / winHeight);
	float Scale = xScale > yScale ? xScale : yScale;
	cout << "scale: " << Scale << endl;

	//移到左上角
	float xMovement = minX <= 0 ? 1.2 * abs(minX) : 0.1* abs(minX);
	float yMovement = minY <= 0 ? 1.2 * abs(minY) : 0.1 * abs(minX);

	if (Scale <= 0.01)
		Scale = (int)(1 / Scale - 0.5) * 0.5;
	else if (Scale < 0.5)
		Scale = (int)(1 / Scale) * 0.6;
	else if (Scale > 1)
		Scale = 1.0f / (int)(Scale + 0.5);
	else
		Scale = 1;

	cout << "scale: " << Scale << endl;
	maxY = INT_MIN;
	minY = INT_MAX;
	maxX = INT_MIN;
	minX = INT_MAX;
	vertices.clear();
	if (Scale != 1 || xMovement != 0 || yMovement != 0) {
		for (auto v : originvertices) {
			//cout << "v: " << v.x << ", " << v.y << ", " << v.z << endl;
			vec3 tmpv(v.x + xMovement, v.y + yMovement, v.z);//平移
			tmpv = tmpv * Scale;//缩放
			tmpv.x = round(tmpv.x);
			tmpv.y = round(tmpv.y);
			tmpv.z = round(tmpv.z);

			maxY = maxY > tmpv.y ? maxY : tmpv.y;
			maxX = maxX > tmpv.x ? maxX : tmpv.x;
			minY = minY < tmpv.y ? minY : tmpv.y;
			minX = minX < tmpv.x ? minX : tmpv.x;
			vertices.push_back(tmpv);
			//cout << "after v: " << tmpv.x << ", " << tmpv.y  << ", " << tmpv.z << endl;
		}
	}
	cout << "最值：" << maxY << ", " << maxX << ", " <<  minY << ", " << minX << endl;
	if (maxY > winHeight) {
		cerr << "Y out of range!!!!!!\n";
	}
	if (maxX > winWidth) {
		cerr << "x out of range!!!!!!\n";
	}
}



void Object::getScreenPos(vector<vec3> &original, vector<ivec3> &screen_output) {
	screen_output.clear();

	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	vector<vec3>::iterator iter;
	for (iter = original.begin(); iter != original.end(); ++iter)
	{
		GLdouble x, y, z;

		gluProject(iter->x, iter->y, iter->z, modelview, projection, viewport, &x, &y, &z);
		ivec3 screen_pos(x, y, iter->z);
		screen_output.push_back(screen_pos);
	}
}
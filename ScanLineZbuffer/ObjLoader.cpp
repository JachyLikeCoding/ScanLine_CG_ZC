#include "ObjLoader.h"

void Object::initObject(const string &objName, int _width, int _height, int _mode) {
	winWidth = _width;
	winHeight = _height;
	mode = _mode;
	bool flag = loadObj(objName);
	colorlist.resize(faces.size());
	//ChangeOriginvertices();
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
	/*cout << originvertices.size() << endl;
	cout << faces.size() << endl;
	cout << "+++++++++++++before change vertices:" << endl;
	cout << "minY:-----------------" << minY << endl;
	cout << "maxY:-----------------" << maxY << endl;
	cout << "minX:-----------------" << minX << endl;
	cout << "maxX:-----------------" << maxX << endl;*/
	/*cout << "-------------------------after change vertices:" << endl;*/
	ChangeOriginvertices();
	/*cout << "minY:-----------------" << minY << endl;
	cout << "maxY:-----------------" << maxY << endl;
	cout << "minX:-----------------" << minX << endl;
	cout << "maxX:-----------------" << maxX << endl;*/

	return true;
}



ClassifiedEdge Object::CalEdge(int polygon_id, int v1_id, int v2_id, ClassifiedEdge edge) {
	vec3 v1 = vertices[v1_id];
	vec3 v2 = vertices[v2_id];
	edge.edge_polygon_id = polygon_id;
	edge.x = v1.y > v2.y ? v1.x : v2.x;
	edge.dx = -(v2.x - v1.x) / (v2.y - v1.y);
	edge.dy = ((int)(v1.y + 0.5f) - (int)(v2.y + 0.5f));
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
	/*GLfloat param_abssum = abs(a) + abs(b) + abs(c);
	if (param_abssum == 0) {
		cerr << "normal vector is 0." << endl;
		param_abssum = 1;
	}
	a /= param_abssum;
	b /= param_abssum;
	c /= param_abssum;*/
	GLfloat p0 = abs(a);
	GLfloat p1 = abs(b);
	GLfloat p2 = abs(c);
	GLfloat cosTheta = p2 / sqrt(p0 + p1 + p2);
	//d = -a * vertices[v1_id].x - b * vertices[v1_id].y - c * vertices[v1_id].z;
	//color.x = cosTheta;
	color.x = rand() % 100 / (float)(100);
	color.y = rand() % 100 / (float)(100);
	color.z = rand() % 100 / (float)(100);
	/*color.y = cosTheta;
	color.z = cosTheta;*/

	cout << face_id << " face color : " << color.x << "\t" << color.y << "\t" << color.z << endl;
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

	edges.push_back(CalEdge(face_id, v1, v2, edge1));
	edges.push_back(CalEdge(face_id, v1, v3, edge2));
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
	GLfloat dx = maxX - minX;
	GLfloat dy = maxY - minY;
	if (dx > dy) {	//winWidth不变
		winHeight = (int)((winWidth) * dy / dx);
	}
	else {	//winHeight不变
		winWidth = (int)((winHeight)* dx / dy);
	}
}

void Object::ChangeOriginvertices() {
	GLfloat dx = maxX - minX;
	GLfloat dy = maxY - minY;
	
	GLfloat scale = dx / winWidth;
	/*GLfloat scaleY = dy / winHeight;
	GLfloat scale = scaleX < scaleY ? scaleX : scaleY;*/

	GLfloat centerX = dx / 2;
	GLfloat centerY = dy / 2;

	GLfloat moveX = (minX < 0) ? 1.1 * abs(minX) : 0.1 * minX;
	GLfloat moveY = (minY < 0) ? 1.1 * abs(minY) : 0.1 * minY;

	/*if (scale < 1) {
		scale = (int)(1 / scale) >> 1;
	}
	else if (scale > 1) {
		scale = 1.0f / (int)(scale + 0.5);
	}
	else {
		scale = 1;
	}*/
	cout << "scale: " << scale << endl;
	maxY = INT_MIN;
	maxX = INT_MIN;
	minY = INT_MAX;
	minX = INT_MAX;
	vertices.clear();
	
	for (auto v : originvertices) {
		vec3 tmpv(v.x + moveX, v.y + moveY, v.z);
		tmpv = tmpv / scale;

		maxY = maxY > tmpv.y ? maxY : tmpv.y;
		maxX = maxX > tmpv.x ? maxX : tmpv.x;
		minY = minY < tmpv.y ? minY : tmpv.y;
		minX = minX < tmpv.x ? minX : tmpv.x;

		vertices.push_back(tmpv);
	}
	cout << "screen vertices:" << endl;
	for (auto v : vertices) {
		cout << v.x << ", " << v.y << ", " << v.z << endl;
	}
}
#include "ObjLoader.h"

void Object::initObject(const string &objName, int _width, int _height, int _mode) {
	winWidth = _width;
	winHeight = _height;
	mode = _mode;
	loadObj(objName);
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
	maxX = INT_MAX;

	while (input >> head) {
		if (input.eof())
			break;
		else {
			if (head == "v") {
				input >> x >> y >> z;
				vertexes.push_back(vec3(x,y,z));

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
					vid[i] > 0 ? (vid[i]--) : (vid[i] += vertexes.size());//这里把它统一成0,1,2,3序号
				}
				faces.push_back(vid);
			}
			else
			{
				continue;
			}
		}
	}

	//just for debug:
	/*for (int i = 0; i < vertexes.size(); i++) {
		cout << vertexes[i][0] << vertexes[i][1] << vertexes[i][2] << endl;
	}*/
	cout << vertexes.size() << endl;
	cout << faces.size() << endl;
	return true;
}



void Object::CalEdge(int polygon_id, int v1_id, int v2_id, ClassifiedEdge *edge) {
	vec3 v1 = vertexes[v1_id];
	vec3 v2 = vertexes[v2_id];
	edge->edge_polygon_id = polygon_id;
	edge->x = v1.y > v2.y ? v1.x : v2.x;
	edge->dx = -(v2.x - v1.x) / (v2.y - v1.y);
	edge->dy = abs((int)(v1.y + 0.5f) - (int)(v2.y + 0.5f));
	edge->maxY = v1.y > v2.y ? (int)(v1.y + 0.5f) : (int)(v2.y + 0.5f);
}

void Object::CalFace(int face_id, int a, int b, int c, int d, int &maxY, float &maxZ, int &dy, vec3 color) {
	int v1_id = faces[face_id][0];//three vertexes of the face
	int v2_id = faces[face_id][1];
	int v3_id = faces[face_id][2];
	//step1: calculate parameters a,b,c,d
	a = ((vertexes[v2_id].y - vertexes[v1_id].y)*(vertexes[v3_id].z - vertexes[v1_id].z)
		- (vertexes[v2_id].z - vertexes[v1_id].z)*(vertexes[v3_id].y - vertexes[v1_id].y));

	b = ((vertexes[v2_id].z - vertexes[v1_id].z)*(vertexes[v3_id].x - vertexes[v1_id].x)
		- (vertexes[v2_id].x - vertexes[v1_id].x)*(vertexes[v3_id].z - vertexes[v1_id].z));

	c = ((vertexes[v2_id].x - vertexes[v1_id].x)*(vertexes[v3_id].y - vertexes[v1_id].y)
		- (vertexes[v2_id].y - vertexes[v1_id].y)*(vertexes[v3_id].x - vertexes[v1_id].x));

	d = (0 - (a*vertexes[v1_id].x + b * vertexes[v1_id].y + c * vertexes[v1_id].z));

	//step2: calculate maxZ and maxY
	maxZ = (vertexes[v1_id].z > vertexes[v2_id].z) ? vertexes[v1_id].z : vertexes[v2_id].z;
	maxZ = (maxZ > vertexes[v3_id].z) ? maxZ : vertexes[v3_id].z;

	maxY = (vertexes[v1_id].y > vertexes[v2_id].y) ? vertexes[v1_id].y : vertexes[v2_id].y;
	maxY = (maxY > vertexes[v3_id].y) ? maxY : vertexes[v3_id].y;

	//step 3: calculate dy
	float minY = (vertexes[v1_id].y < vertexes[v2_id].y) ? vertexes[v1_id].y : vertexes[v2_id].y;
	minY = (minY < vertexes[v3_id].y) ? minY : vertexes[v3_id].y;
	
	maxY = (int)(maxY + 0.5);
	minY = (int)(minY + 0.5);
	dy = maxY - minY;
}

void Object::CalFaceEdges(int face_id) {
	ClassifiedEdge *edge1 = new ClassifiedEdge;
	ClassifiedEdge *edge2 = new ClassifiedEdge;
	ClassifiedEdge *edge3 = new ClassifiedEdge;

	int v1, v2, v3;// v1.y > v2.y > v3.y
	int v1_id = faces[face_id][0];//three vertexes of the face
	int v2_id = faces[face_id][1];
	int v3_id = faces[face_id][2];

	if (vertexes[v1_id].y < vertexes[v2_id].y) {
		if (vertexes[v2_id].y < vertexes[v3_id].y) {
			v1 = v3_id;
			v2 = v2_id;
			v3 = v1_id;
		}else {
			if (vertexes[v1_id].y < vertexes[v3_id].y) {
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
		if (vertexes[v1_id].y < vertexes[v3_id].y) {
			v1 = v3_id;
			v2 = v1_id;
			v3 = v2_id;
		}else {
			if (vertexes[v2_id].y < vertexes[v3_id].y) {
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

	CalEdge(face_id, v1, v2, edge1);
	CalEdge(face_id, v1, v3, edge2);
	CalEdge(face_id, v2, v3, edge3);
	edges.push_back(edge1);
	edges.push_back(edge2);
	edges.push_back(edge3);
}

//just for debug
void Object::test() {
	//test vertexes of face
	cout << "face count: " << faces.size() << endl;
	for (int i = 0; i < faces.size(); i++) {
		CalFaceEdges(i);
		cout << "[" << i << "]";
		for (int j = 0; j < faces[i].size(); j++) {
			cout << faces[i][j] << "\t";
		} 
		cout << endl;
	}
	//test edges of face:
	cout << edges.size() << endl;
	for (int i = 0; i < edges.size(); i++) {
		cout << "edge_polygon_id: " << edges[i]->edge_polygon_id;
		cout << "\tedge_dy: " << edges[i]->dy << endl;
	}
}
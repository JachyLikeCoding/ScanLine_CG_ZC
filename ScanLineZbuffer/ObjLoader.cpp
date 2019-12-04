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
					vid[i] = stoi(tmps[i].substr(0, iter - tmps[i].begin()));
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

void Object::CalFace(int face_id, int a, int b, int c, int d, vec3 color) {
	int v1_id = faces[face_id][0];
	int v2_id = faces[face_id][1];
	int v2_id = faces[face_id][2];

	faces[face_id] = ((vertexes[v2_id].y - vertexes[v1_id].y)*(vertexes[v3_id].z - vertexes[v1_id].z)
		  - (vertexes[v2_id].z - vertexes[v1_id].z)*(vertexes[v3_id].y - vertexes[v1_id].y));

	b = ((vertexes[v2_id].z - vertexes[v1_id].z)*(vertexes[v3_id].x - vertexes[v1_id].x)
		- (vertexes[v2_id].x - vertexes[v1_id].x)*(vertexes[v3_id].z - vertexes[v1_id].z));

	c = ((vertexes[v2_id].x - vertexes[v1_id].x)*(vertexes[v3_id].y - vertexes[v1_id].y)
		- (vertexes[v2_id].y - vertexes[v1_id].y)*(vertexes[v3_id].x - vertexes[v1_id].x));

	d = (0 - (a*vertexes[v1_id].x + b * vertexes[v1_id].y + c * vertexes[v1_id].z));
	
}
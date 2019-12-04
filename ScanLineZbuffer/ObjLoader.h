#pragma once
#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H
#include "Basic.h"
#include "DataStructure.h"

class Object {
public:
	Object(){}
	vector<vec3> vertexes;
	vector<vector<int>> faces;
	vector<vec3> colorlist;
	void initObject(const string &objName, int width, int height, int mode);
	void CalEdge(int polygon_id, int v1_id, int v2_id, ClassifiedEdge *edge);
	void CalFace(int face_id, int a, int b, int c, int d, vec3 color);
	GLfloat maxY, minY, maxX, minX;
	bool loadObj(const string &objName);
	
private:
	int winWidth, winHeight;
	int mode;
	void resize();
};





#endif // !OBJ_LOADER_H

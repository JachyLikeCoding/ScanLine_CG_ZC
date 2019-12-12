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
	vector<ClassifiedEdge> edges;
	void initObject(const string &objName, int width, int height, int mode);
	ClassifiedEdge CalEdge(int polygon_id, int v1_id, int v2_id, ClassifiedEdge edge);
	void CalFace(int face_id, GLfloat &a, GLfloat &b, GLfloat &c, GLfloat &d, int &maxY, GLfloat &maxZ, int &dy, vec3 &color);
	void CalFaceEdges(int face_id);
	GLfloat maxY, minY, maxX, minX;
	bool loadObj(const string &objName);
	void test();

private:
	int winWidth, winHeight;
	int mode;

};


#endif // !OBJ_LOADER_H
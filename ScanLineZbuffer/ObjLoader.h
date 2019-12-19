#pragma once
#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H
#include "Basic.h"
#include "DataStructure.h"

class Object {
public:
	Object(){}
	vector<ivec3> vertices;
	vector<vec3> originvertices;
	vector<vec3> colorlist;
	vector<vector<int>> faces;
	vector<ClassifiedEdge> edges;
	void initObject(const string &objName, int width, int height, int mode);
	
	void CalFace(int face_id, GLfloat &a, GLfloat &b, GLfloat &c, GLfloat &d, int &maxY, GLfloat &maxZ, int &dy, vec3 &color);
	void CalFaceEdges(int face_id);
	GLfloat maxY, minY, maxX, minX;
	int getWinWidth() { return winWidth; }
	int getWinHeight() { return winHeight; }
	void test();

private:
	int winWidth, winHeight;
	int mode;
	bool loadObj(const string &objName);
	ClassifiedEdge CalEdge(int polygon_id, int v1_id, int v2_id, ClassifiedEdge edge);
	void getScreenPos(vector<vec3> &originalposition, vector<ivec3> &screenposition);
	void ChangeOriginvertices();
	void ChangeScreenSize();
};


#endif // !OBJ_LOADER_H
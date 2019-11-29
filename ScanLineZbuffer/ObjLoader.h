#pragma once
#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H
#include "Basic.h"
#include "DataStructure.h"

class Object {
public:
	vector<vec3> &vertexes;
	vector<vector<int>> face;

	
private:
	bool loadObj(const string &objName);

};





#endif // !OBJ_LOADER_H

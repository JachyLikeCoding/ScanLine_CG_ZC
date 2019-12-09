#pragma once
#ifndef SCANLINE_Z_BUFFER_H
#define SCANLINE_Z_BUFFER
#include "Basic.h"
#include "DataStructure.h"
#include "ObjLoader.h"
class ScanlineZBufferProcessor {
public:
	void initProcessor(Object &_obj);
	void buildCET();
	void buildCPT();
	bool addPolygonToAPT(int maxY, vector<ActivePolygon> &APT, vector<vector<ClassifiedPolygon>> &CPT);
	bool addEdgeToAET(vector<ClassifiedEdge> &CET_y, vector<ActiveEdge> &AET, vector<ClassifiedPolygon> &CPT_y);
	void ScanlineZBuffer(Object &_obj);
	void resizeWindow(int width, int height);
	void test();//just for debug
	void printAET(vector<ActiveEdge> &AET);
	void printAPT(vector<ActivePolygon> &APT);
	Object obj;
	int winWidth, winHeight;
	vector<vector<ClassifiedPolygon>> CPT;
	vector<vector<ClassifiedEdge>> CET;
	vector<ActivePolygon> APT;
	vector<ActiveEdge> AET;
	vector<GLfloat> zbuffer;//z»º³åÆ÷
	vector<int> coloridbuffer;//Ö¡»º³åÆ÷
};

#endif // !SCANLINE_Z_BUFFER_H

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
	void buildAET(int y);
	void buildAPT(int y);
	void updateZBuffer(int y);
	void ScanlineZBuffer(Object &_obj);
	void resizeWindow(int width, int height);
	void test();//just for debug
	
	Object obj;
	int winWidth, winHeight;
	vector<ClassifiedPolygon *> CPT;
	vector<ClassifiedEdge *> CET;
	vector<ActivePolygon *>APT;
	vector<ActiveEdge *> AET;
	vector<GLfloat> zbuffer;	//z»º³åÆ÷
	vector<int> coloridbuffer;//Ö¡»º³åÆ÷
};

#endif // !SCANLINE_Z_BUFFER_H

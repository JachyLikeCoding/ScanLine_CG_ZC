#pragma once
#ifndef SCANLINE_Z_BUFFER_H
#define SCANLINE_Z_BUFFER

#include "DataStructure.h"
#include "ObjLoader.h"
class ScanlineZBufferProcessor {
public:
	void buildCET();
	void buildCPT();
	void buildAET(int y);
	void buildAPT(int y);
	void updateZBuffer(int y);
	void ScanlineZBuffer();
	

private:
	Object obj;
	int winWidth, winHeight;
	vector<ClassifiedPolygon *> CPT;
	vector<ClassifiedEdge *> CET;
	ActivePolygon *APT;
	ActiveEdge *AET;
	vector<GLfloat> zbuffer;
};



























#endif // !SCANLINE_Z_BUFFER_H

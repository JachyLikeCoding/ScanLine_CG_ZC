#pragma once
#ifndef SCANLINE_Z_BUFFER_H
#define SCANLINE_Z_BUFFER
#include "Basic.h"
#include "DataStructure.h"
#include "ObjLoader.h"
class ScanlineZBufferProcessor {
public:
	Object obj;
	int winWidth, winHeight;
	vector<vector<ClassifiedPolygon>> CPT;
	vector<vector<ClassifiedEdge>> CET;
	vector<ActivePolygon> APT;
	vector<ActiveEdge> AET;
	void initProcessor(Object &_obj);
	
	void ScanlineZBuffer(Object &_obj, int windowWidth);
	
	void resizeWindow(int width, int height);
	vector<GLfloat> getframebuffer() { return framebuffer; }
	

private:
	vec3 getColor(int polygon_id, vector<ActivePolygon> &APT);
	vector<GLfloat> zbuffer;//z»º³åÆ÷
	vector<int> coloridbuffer;//Ö¡»º³åÆ÷
	vector<GLfloat> framebuffer;
	
	void buildCET();
	void buildCPT();
	bool addPolygonToAPT(int maxY, vector<ActivePolygon> &APT, vector<vector<ClassifiedPolygon>> &CPT);
	void addEdgeToAET(vector<ClassifiedEdge> &CET_y, vector<ActiveEdge> &AET, vector<ClassifiedPolygon> &CPT_y);
	void clearDS();
	void updateBuffer(vector<ActiveEdge> &AET, int y);
	void update_APTAET(vector<ActivePolygon> &APT, vector<ActiveEdge> &AET, vector<ClassifiedEdge> &CET_y);
	//just for debug
	void test();
	void printAET(vector<ActiveEdge> &AET);
	void printAPT(vector<ActivePolygon> &APT);
};

#endif // !SCANLINE_Z_BUFFER_H

#include "ScanlineZBuffer.h"

void ScanlineZBufferProcessor::initProcessor(Object &_obj) {
	obj = _obj;
	CPT.resize(WINDOW_HEIGHT, NULL);
	CET.resize(WINDOW_HEIGHT, NULL);
}

//build classified polygon table
void ScanlineZBufferProcessor::buildCPT() {
	cout << "obj.faces.size()£º" << obj.faces.size() << endl;
	for (int i = 0; i < obj.faces.size(); i++) {
		int maxY;
		GLfloat maxZ;
		GLfloat a, b, c, d;
		int dy;
		vec3 color;
		
		obj.CalFace(i, a, b, c, d, maxY, maxZ, dy, color);
		obj.CalFaceEdges(i);
		ClassifiedPolygon *newCP = new ClassifiedPolygon(i, a, b, c, d, dy, color, nullptr);

		cout << "maxY:" << maxY << endl;

		//TODO:MAXY wrong here!
		if (CPT[maxY] == NULL) {
			CPT[maxY] = newCP;
		}
		else
		{
			newCP->nxtPolygon = CPT[maxY];
			CPT[maxY] = newCP;
		}
	}
}


void ScanlineZBufferProcessor::buildCET() {
	cout << "obj.faces.size()£º" << obj.faces.size() << endl;
	for (int i = 0; i < obj.faces.size(); i++) {
		obj.CalFaceEdges(i);
	}
	for (auto edge : obj.edges) {
		if (CET[edge->maxY] == NULL) {
			CET[edge->maxY] = edge;
		}
		else {
			edge->nxtEdge = CET[edge->maxY];
			CET[edge->maxY] = edge;
		}
	}
}

void ScanlineZBufferProcessor::buildAET(int y) {

}

void ScanlineZBufferProcessor::buildAPT(int y) {
	ActivePolygon *ap = NULL;
	if (APT != NULL) {

	}
}

void ScanlineZBufferProcessor::updateZBuffer(int y) {

}

void clear() {
	

}

void ScanlineZBufferProcessor::ScanlineZBuffer(Object &_obj) {
	initProcessor(_obj);
	buildCPT();
	buildCET();
	
	for (int y = obj.maxY; y >= obj.minY; y--) {
		vector<GLfloat> zbuffer(WINDOW_WIDTH, -INT_MAX);
		vector<int> coloridbuffer(WINDOW_WIDTH, -1);
	}

	clear();
}


void ScanlineZBufferProcessor::resizeWindow(int width, int height) {
	winHeight = height;
	winWidth = width;
	CPT.clear();
	CET.clear();
	CPT.resize(height, NULL);
	CET.resize(height, NULL);
	zbuffer.clear();
	//TODO:Check zbuffer size
	zbuffer.resize(height * width, NULL);
}


//just for debug:
void ScanlineZBufferProcessor::test() {
	cout << "CET size:" << CET.size() << endl;
	cout << "CPT size:" << CPT.size() << endl;
}
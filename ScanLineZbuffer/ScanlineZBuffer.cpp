#include "ScanlineZBuffer.h"

//build classified polygon table
void ScanlineZBufferProcessor::buildCPT() {
	for (int i = 0; i < obj.faces.size(); i++) {
		int maxY, dy;
		int a, b, c, d;
		vec3 color;
		GLfloat maxZ;
		obj.CalFace(i, a, b, c, d, maxY, maxZ, dy, color);
		obj.CalFaceEdges(i);
		ClassifiedPolygon *newCP = new ClassifiedPolygon(i, a, b, c, d, dy, color, nullptr);

		if (CPT[maxY] == nullptr) {
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

}

void ScanlineZBufferProcessor::buildAET(int y) {

}

void ScanlineZBufferProcessor::buildAPT(int y) {

}

void ScanlineZBufferProcessor::updateZBuffer(int y) {

}

void clear() {
	

}

void ScanlineZBufferProcessor::ScanlineZBuffer() {
	buildCPT();
	buildCET();

	for (int y = obj.maxY; y >= obj.minY; y--) {
		buildAPT(y);
		buildAET(y);
		updateZBuffer(y);
	}

	clear();
}
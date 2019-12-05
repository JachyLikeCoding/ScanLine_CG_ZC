#include "ScanlineZBuffer.h"

void ScanlineZBufferProcessor::buildCET() {

}

void ScanlineZBufferProcessor::buildCPT() {

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
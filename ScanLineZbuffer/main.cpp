#include "Basic.h"
#include "ObjLoader.h"
#include "DataStructure.h"
#include "ScanlineZBuffer.h"
#include "display.h"

	int main(int argc, char* argv[]) {
	ScanlineZBufferProcessor scanlineProcessor;
	Object obj;
	string inputfilepath = "models/";
	string objfile = "test1.obj";
	int mode = 1;
	inputfilepath += objfile;
	obj.initObject(inputfilepath, WINDOW_WIDTH, WINDOW_HEIGHT, mode);

	//test obj loader
	/*bool flag;
	flag = obj.loadObj(inputfilepath);*/
	obj.test();

	//display(argc, argv);

	scanlineProcessor.ScanlineZBuffer(obj);
	system("pause");
	return 0;
}
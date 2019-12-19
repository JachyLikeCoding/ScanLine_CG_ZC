#include "Basic.h"
#include "ObjLoader.h"
#include "DataStructure.h"
#include "ScanlineZBuffer.h"
#include "display.h"

int main(int argc, char* argv[]) {
	Object obj;
	ScanlineZBufferProcessor scanlineProcessor;
	string inputfilepath = "models/";
	string objfile = "test.obj";
	int mode = 1;

	//TODO: 添加模式选择功能和模型选择功能。
	inputfilepath += objfile;
	obj.initObject(inputfilepath, WINDOW_WIDTH, WINDOW_HEIGHT, mode);
	//obj.test();
	display(argc, argv, obj, scanlineProcessor);
	
	system("pause");
	return 0;
}
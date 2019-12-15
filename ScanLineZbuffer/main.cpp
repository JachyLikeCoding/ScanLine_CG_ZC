#include "Basic.h"
#include "ObjLoader.h"
#include "DataStructure.h"
#include "ScanlineZBuffer.h"
#include "display.h"



//void draw() {
//	glClear(GL_COLOR_BUFFER_BIT);
//	
//	scanlineProcessor.ScanlineZBuffer(obj);
//
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//	
//	//DRAW FUNCTION:
//	glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, scanlineProcessor.getFrameBuffer());
//	//TODO:调整模型到屏幕中间，旋转模型
//
//
//
//	glutSwapBuffers();
//}

//void reshape(int width, int height) {
//	draw();
//}
//
//void specialkeys(int key, int x, int y) {
//
//}

//void display(int argc, char* argv[]) {
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
//	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
//	glutInitWindowPosition(100, 100);
//	glutCreateWindow("Scanline Z-Buffer by ChierZ");
//	glutReshapeFunc(reshape);
//
//	glutDisplayFunc(draw);
//	glutSpecialFunc(specialkeys);
//	glClearColor(0, 0, 0, 0);
//
//	glutMainLoop();
//}
//void display(int argc, char ** argv, int _eyePos[3], int _centerPos[3], Object &obj, ScanlineZBufferProcessor &Processor);

int main(int argc, char* argv[]) {
	Object obj;
	ScanlineZBufferProcessor scanlineProcessor;
	string inputfilepath = "models/";
	string objfile = "batman.obj";
	int mode = 1;

	//TODO: 添加模式选择功能和模型选择功能。
	inputfilepath += objfile;
	obj.initObject(inputfilepath, WINDOW_WIDTH, WINDOW_HEIGHT, mode);
	obj.test();
	//display(argc, argv, obj, scanlineProcessor);
	
	system("pause");
	return 0;
}
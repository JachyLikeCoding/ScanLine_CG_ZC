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
//	//TODO:����ģ�͵���Ļ�м䣬��תģ��
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

	//TODO: ���ģʽѡ���ܺ�ģ��ѡ���ܡ�
	inputfilepath += objfile;
	obj.initObject(inputfilepath, WINDOW_WIDTH, WINDOW_HEIGHT, mode);
	obj.test();
	//display(argc, argv, obj, scanlineProcessor);
	
	system("pause");
	return 0;
}
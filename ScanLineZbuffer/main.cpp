#include "Basic.h"
#include "ObjLoader.h"
#include "DataStructure.h"
#include "ScanlineZBuffer.h"

Object obj;
ScanlineZBufferProcessor scanlineProcessor;

void draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//DRAW FUNCTION:
	scanlineProcessor.ScanlineZBuffer(obj);
	glutSwapBuffers();
}

void reshape(int width, int height) {
	draw();
}

void specialkeys(int key, int x, int y) {

}

void display(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Scanline Z-Buffer by ChierZ");
	glutReshapeFunc(reshape);

	glutDisplayFunc(draw);
	glutSpecialFunc(specialkeys);
	glClearColor(0, 0, 0, 0);

	glutMainLoop();
}

int main(int argc, char* argv[]) {
	string inputfilepath = "models/";
	string objfile = "test1.obj";
	int mode = 1;
	//TODO: 添加模式选择功能和模型选择功能。
	inputfilepath += objfile;
	obj.initObject(inputfilepath, WINDOW_WIDTH, WINDOW_HEIGHT, mode);

	display(argc, argv);
	scanlineProcessor.ScanlineZBuffer(obj);
	system("pause");
	return 0;
}
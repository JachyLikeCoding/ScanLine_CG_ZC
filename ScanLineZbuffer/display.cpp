#include "display.h"
#include "DataStructure.h"
#include "ScanlineZBuffer.h"

void mouseCallback(int button, int state, int x, int y);
void mouseMotionCallback(int x, int y);
void displayCallback();
void InitGLUT(int argc, char ** argv);
ScanlineZBufferProcessor scanlineProcessor;
Object obj;
bool mouseLeftDown, mouseRightDown;
float mouseX, mouseY;

void InitGLUT(int argc, char ** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);// 显示模式：双缓冲、RGBA
	// 窗口设置
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);      // 窗口尺寸
	glutInitWindowPosition(100, 100);  // 窗口位置
	glutCreateWindow("Scanline Z-Buffer by ChiZhang");   // 窗口标题
}


void displayCallback() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //清除颜色缓冲以及深度缓冲
	//绘制图像
	scanlineProcessor.ScanlineZBuffer(obj, WINDOW_WIDTH);
	vector<GLfloat> framebuffer = scanlineProcessor.getframebuffer();
	glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, &framebuffer[0]);
	glutSwapBuffers();
}

void mouseCallback(int button, int state, int x, int y) {
	mouseX = x;
	mouseY = y;

	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			mouseLeftDown = true;
		}
		else if (state == GLUT_UP)
			mouseLeftDown = false;
	}
	else if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN) {
			mouseRightDown = true;
		}
		else if (state == GLUT_UP)
			mouseRightDown = false;
	}
}

//void mouseMotionCallback(int x, int y) {
//	if (mouseRightDown) {
//		cameraAngleY += (x - mouseX);
//		cameraAngleX += (y - mouseY);
//		mouseX = x;
//		mouseY = y;
//	}
//	if (mouseLeftDown) {
//		cameraDistance += (y - mouseY) * 0.2f;
//		mouseY = y;
//	}
//}

void display(int argc, char ** argv, Object &_obj, ScanlineZBufferProcessor &_Processor){
	//传参，初始化变量
	obj = _obj;
	scanlineProcessor = _Processor;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);// 显示模式：双缓冲、RGBA
	// 窗口设置
	glutInitWindowSize(obj.getWinWidth(), obj.getWinHeight());      // 窗口尺寸
	glutInitWindowPosition(100, 100);  // 窗口位置
	glutCreateWindow("Scanline Z-Buffer by ChiZhang");   // 窗口标题
	//InitGLUT(argc, argv);
	glutDisplayFunc(displayCallback);
	glutMainLoop();
}
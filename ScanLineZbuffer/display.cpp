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
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);// ��ʾģʽ��˫���塢RGBA
	// ��������
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);      // ���ڳߴ�
	glutInitWindowPosition(100, 100);  // ����λ��
	glutCreateWindow("Scanline Z-Buffer by ChiZhang");   // ���ڱ���
}


void displayCallback() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //�����ɫ�����Լ���Ȼ���
	//����ͼ��
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
	//���Σ���ʼ������
	obj = _obj;
	scanlineProcessor = _Processor;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);// ��ʾģʽ��˫���塢RGBA
	// ��������
	glutInitWindowSize(obj.getWinWidth(), obj.getWinHeight());      // ���ڳߴ�
	glutInitWindowPosition(100, 100);  // ����λ��
	glutCreateWindow("Scanline Z-Buffer by ChiZhang");   // ���ڱ���
	//InitGLUT(argc, argv);
	glutDisplayFunc(displayCallback);
	glutMainLoop();
}
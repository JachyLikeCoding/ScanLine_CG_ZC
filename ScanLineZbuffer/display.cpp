#include "display.h"
#include "DataStructure.h"
#include "ScanlineZBuffer.h"


#define MAX_CHAR       128	//128���ַ� 0-127
#define N  999 //��ɫ����ΪС�������3λ
void CALLBACK vertexCallback(GLvoid* vertex);
void CALLBACK beginCallback(GLenum type);
void CALLBACK endCallback();
void CALLBACK errorCallback(GLenum errorCode);
void displayCallback();
void mouseCallback(int button, int state, int x, int y);
void mouseMotionCallback(int x, int y);
void keyboardCallback(int key, int x, int y);
void reshapeCallback(int w, int h);
void timerCallback(int id);
void InitGLUT(int argc, char ** argv);
void InitGL();
void drawString(const char* str);
void setRandomColor();
const int randomCount = 1000;
int eyePos[3], centerPos[3];
bool mouseLeftDown, mouseRightDown;
float mouseX, mouseY;
float cameraDistance;
float cameraAngleX, cameraAngleY;
float r, g, b;
int displayMode = 0;	// ѡ��ʵ�����ģʽ��1->L:line frame  0->F:face
bool showAxis = true;	//�Ƿ���ʾ����ϵ
float random[randomCount];
ScanlineZBufferProcessor scanlineProcessor;
Object obj;



void InitGLUT(int argc, char ** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);// ��ʾģʽ��˫���塢RGBA
	// ��������
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);      // ���ڳߴ�
	glutInitWindowPosition(100, 100);  // ����λ��
	glutCreateWindow("Scanline Z-Buffer by ChiZhang");   // ���ڱ���

	//�ص�����
	glutDisplayFunc(displayCallback);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(mouseMotionCallback);
	glutSpecialFunc(keyboardCallback);
	glutReshapeFunc(reshapeCallback);
	glutTimerFunc(33, timerCallback, 1);
}

void InitLights() {
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat Light_ambient[] = { 0.5, 0.5, 0.5, 1.0 }; //���������
	GLfloat Light_specular[] = { 0.5, 0.5, 0.5, 0.5 };  //���淴�����
	GLfloat Light_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };	//���������
	GLfloat light0_position[] = { 0.0, 17.0, 100.0, 1.0 };//0��ƽ�й⣬1�����Դ

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	//�ƹ�����
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Light_diffuse);   //ɢ�������
	glLightfv(GL_LIGHT0, GL_SPECULAR, Light_specular);  //���淴���
	glLightfv(GL_LIGHT0, GL_AMBIENT, Light_ambient);  //���������

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}


void InitGL() {
	glShadeModel(GL_SMOOTH); // ������Ӱƽ�� 
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // ͸������

	glEnable(GL_DEPTH_TEST); // ������Ȳ��� 
	glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glLineWidth(3.0f);


	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // ��ɫ���� 
	glClearStencil(0);
	glClearDepth(1.0f);          // ������Ȼ��� 
	glDepthFunc(GL_LEQUAL); // ������Ȳ��Ե�����

	//���ù���
	InitLights();

	//�������
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyePos[0], eyePos[1], eyePos[2], centerPos[0], centerPos[1], centerPos[2], -1, 0, 0);
}


void GLDrawSpaceAxes() {
	GLUquadricObj *objCylinder = gluNewQuadric();
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidSphere(0.1, 6, 6);
	glColor3f(0.1f, 0.1f, 0.8f);
	gluCylinder(objCylinder, 0.1, 0.1, 50, 50, 5);//Z
	glTranslatef(0, 0, 50);
	gluCylinder(objCylinder, 0.3, 0.0, 0.6, 50, 5);//Z
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0.5, 50);
	glRotatef(90, 0.0, 1.0, 0.0);
	glRasterPos3f(0.0f, 0.0f, 10.0f);
	drawString("Z");// Print GL Text ToThe Screen
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.1f, 0.8f, 0.1f);
	glRotatef(-90, 1.0, 0.0, 0.0);
	gluCylinder(objCylinder, 0.1, 0.1, 50, 50, 5);//Y
	glTranslatef(0, 0, 50);

	gluCylinder(objCylinder, 0.3, 0.0, 0.6, 50, 5);//Y
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.5, 50, 0);
	glRasterPos3f(0.0f, 10.0f, 0.0f);
	drawString("Y");
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8f, 0.1f, 0.1f);
	glRotatef(90, 0.0, 1.0, 0.0);
	gluCylinder(objCylinder, 0.1, 0.1, 50, 50, 5);//X
	glTranslatef(0, 0, 50);
	gluCylinder(objCylinder, 0.3, 0.0, 0.6, 50, 5);//X
	glPopMatrix();

	glPushMatrix();
	glTranslatef(50, 0.5, 0);
	glRasterPos3f(10.0f, 0.0f, 0.0f);
	drawString("X");
	glPopMatrix();
}


// ASCII�ַ��ܹ�ֻ��0��127��һ��128���ַ�
#define MAX_CHAR       128
void drawString(const char* str) {
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall) { // ��һ�ε��ã�ִ�г�ʼ��
		isFirstCall = 0;// Ϊÿһ��ASCII�ַ�����һ����ʾ�б�
		// ����MAX_CHAR����������ʾ�б���
		lists = glGenLists(MAX_CHAR);
		// ��ÿ���ַ��Ļ������װ����Ӧ����ʾ�б���
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	// ����ÿ���ַ���Ӧ����ʾ�б�����ÿ���ַ�
	for (; *str != '\0'; ++str)
		glCallList(lists + *str);
}

void displayCallback() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //�����ɫ�����Լ���Ȼ���
	glPushMatrix();
	//�������
	glTranslatef(0, 0, cameraDistance);
	glRotatef(cameraAngleX, 1, 0, 0);
	glRotatef(cameraAngleY, 0, 1, 0);

	//����ͼ��

	scanlineProcessor.ScanlineZBuffer(obj);
	vector<GLfloat> framebuffer = scanlineProcessor.getframebuffer();
	/*for (int i = 0; i < framebuffer.size(); i++) {
		cout << framebuffer[i] << ",";
	}*/
	//glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, &framebuffer[0]);

	if (showAxis) {
		GLDrawSpaceAxes();
	}

	glPopMatrix();
	glutSwapBuffers();
}

void setRandomColor() {
	srand(time(NULL));
	for (int i = 0; i < randomCount; i++)
		random[i] = rand() % (N + 1) / (float)(N + 1);
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

void mouseMotionCallback(int x, int y) {
	if (mouseRightDown) {
		cameraAngleY += (x - mouseX);
		cameraAngleX += (y - mouseY);
		mouseX = x;
		mouseY = y;
	}
	if (mouseLeftDown) {
		cameraDistance += (y - mouseY) * 0.2f;
		mouseY = y;
	}
}

void keyboardCallback(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP: displayMode = 1;
		break;
	case GLUT_KEY_DOWN: displayMode = 0;
		break;
	case GLUT_KEY_LEFT: showAxis = false;
		break;
	case GLUT_KEY_RIGHT: showAxis = true;
		break;
	}
}

void timerCallback(int id) {
	glutTimerFunc(50, timerCallback, 1);//glutTimerFunc(������, �ص�����ָ��, ����ֵ);
	glutPostRedisplay();//�ػ�
}

void reshapeCallback(int w, int h) {
	if (h == 0) {
		h = 1;
	}
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)w / h, 1.0f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
}


//����Ļص�����  
void CALLBACK vertexCallback(GLvoid* vertex) {
	GLdouble* pt;
	pt = (GLdouble*)vertex;
	glVertex3dv(pt);
}

void CALLBACK beginCallback(GLenum type) {
	glBegin(type);
}

void CALLBACK endCallback() {
	glEnd();
}

void CALLBACK errorCallback(GLenum errorCode) {
	const GLubyte * estring;
	//��ӡ��������  
	estring = gluErrorString(errorCode);
	fprintf(stderr, "Tessellation Error: %s/n", estring);
	exit(0);
}

void display(int argc, char ** argv, int _eyePos[3], int _centerPos[3], Object &_obj, ScanlineZBufferProcessor &_Processor){
	//���Σ���ʼ������
	obj = _obj;
	scanlineProcessor = _Processor;

	for (int i = 0; i < 3; i++) {
		eyePos[i] = _eyePos[i];
		centerPos[i] = _centerPos[i];
	}
	mouseLeftDown = mouseRightDown = false;
	setRandomColor();

	// ��ʼ��
	InitGLUT(argc, argv);
	InitGL();

	// ֪ͨ��ʼGLUT���ڲ�ѭ��
	glutMainLoop();
}
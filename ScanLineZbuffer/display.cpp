#include "display.h"
#include "DataStructure.h"
#include "ScanlineZBuffer.h"


#define MAX_CHAR       128	//128种字符 0-127
#define N  999 //颜色精度为小数点后面3位
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
int displayMode = 0;	// 选择实体绘制模式：1->L:line frame  0->F:face
bool showAxis = true;	//是否显示坐标系
float random[randomCount];
ScanlineZBufferProcessor scanlineProcessor;
Object obj;



void InitGLUT(int argc, char ** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);// 显示模式：双缓冲、RGBA
	// 窗口设置
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);      // 窗口尺寸
	glutInitWindowPosition(100, 100);  // 窗口位置
	glutCreateWindow("Scanline Z-Buffer by ChiZhang");   // 窗口标题

	//回调函数
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
	GLfloat Light_ambient[] = { 0.5, 0.5, 0.5, 1.0 }; //环境光参数
	GLfloat Light_specular[] = { 0.5, 0.5, 0.5, 0.5 };  //镜面反射参数
	GLfloat Light_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };	//漫反射参数
	GLfloat light0_position[] = { 0.0, 17.0, 100.0, 1.0 };//0：平行光，1：点光源

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	//灯光设置
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Light_diffuse);   //散射光属性
	glLightfv(GL_LIGHT0, GL_SPECULAR, Light_specular);  //镜面反射光
	glLightfv(GL_LIGHT0, GL_AMBIENT, Light_ambient);  //环境光参数

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}


void InitGL() {
	glShadeModel(GL_SMOOTH); // 启用阴影平滑 
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // 透视修正

	glEnable(GL_DEPTH_TEST); // 启用深度测试 
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

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // 黑色背景 
	glClearStencil(0);
	glClearDepth(1.0f);          // 设置深度缓存 
	glDepthFunc(GL_LEQUAL); // 所作深度测试的类型

	//设置光照
	InitLights();

	//设置相机
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


// ASCII字符总共只有0到127，一共128种字符
#define MAX_CHAR       128
void drawString(const char* str) {
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall) { // 第一次调用，执行初始化
		isFirstCall = 0;// 为每一个ASCII字符产生一个显示列表
		// 申请MAX_CHAR个连续的显示列表编号
		lists = glGenLists(MAX_CHAR);
		// 把每个字符的绘制命令都装到对应的显示列表中
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	// 调用每个字符对应的显示列表，绘制每个字符
	for (; *str != '\0'; ++str)
		glCallList(lists + *str);
}

void displayCallback() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //清除颜色缓冲以及深度缓冲
	glPushMatrix();
	//调整相机
	glTranslatef(0, 0, cameraDistance);
	glRotatef(cameraAngleX, 1, 0, 0);
	glRotatef(cameraAngleY, 0, 1, 0);

	//绘制图像

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
	glutTimerFunc(50, timerCallback, 1);//glutTimerFunc(毫秒数, 回调函数指针, 区别值);
	glutPostRedisplay();//重绘
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


//顶点的回调函数  
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
	//打印错误类型  
	estring = gluErrorString(errorCode);
	fprintf(stderr, "Tessellation Error: %s/n", estring);
	exit(0);
}

void display(int argc, char ** argv, int _eyePos[3], int _centerPos[3], Object &_obj, ScanlineZBufferProcessor &_Processor){
	//传参，初始化变量
	obj = _obj;
	scanlineProcessor = _Processor;

	for (int i = 0; i < 3; i++) {
		eyePos[i] = _eyePos[i];
		centerPos[i] = _centerPos[i];
	}
	mouseLeftDown = mouseRightDown = false;
	setRandomColor();

	// 初始化
	InitGLUT(argc, argv);
	InitGL();

	// 通知开始GLUT的内部循环
	glutMainLoop();
}
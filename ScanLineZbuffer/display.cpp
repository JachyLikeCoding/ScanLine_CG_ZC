#include "display.h"


void draw(){
	
	glClear(GL_COLOR_BUFFER_BIT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//DRAW FUNCTION:



	glutSwapBuffers();


	glutMainLoop();
}


void reshape(int width, int height) {
	draw();
}

void specialkeys(int key, int x, int y) {

}

void display(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(WINWIDTH, WINHEIGHT);
	glutReshapeFunc(reshape);

	glutIdleFunc(draw);
	glutSpecialFunc(specialkeys);
	glClearColor(0, 0, 0, 0);

	glutMainLoop();
}
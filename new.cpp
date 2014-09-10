#include<stdio.h>
#include<GL/gl.h>
#include<GL/glut.h>
#include<stdlib.h>

bool *keyStates = new bool[256];
float initial=0;
GLuint cubeList;

void createCube() {
	cubeList = glGenLists(1);
	glNewList(cubeList, GL_COMPILE);
	glPushMatrix();
	glutSolidCube(2);
	glPopMatrix();
	glEndList();
}

void keyOperations() {
	float initial=0;
	float speed = 0.2;
//	printf("%d\n", keyStates['a']);
	if (keyStates['c']) {
//		printf("Button C is pressed\n");
		glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
		glClear (GL_COLOR_BUFFER_BIT);
	}
	if (keyStates['w']) {
	glTranslatef(0.0f, 0.0f, -5.0f);
		initial += 0.2;
	glTranslatef(0.0f, 0.0f, 5.0f);
	}
}

void renderSquare () {
	printf("%f\n", initial);
	glRotatef(initial, 0.0f, 1.0f, 0.0f);
//	glPointSize(20.0f);
//	glBegin(GL_POINTS);
	glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f,-1.0f, 0.0f);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f( 1.0f, 1.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f( 1.0f,-1.0f, 0.0f);
	glEnd();
//	glutWireCube(2);
//	glutSolidCube(1);

	glColor4f(0.0, 1.0, 1.0, 1.0);
	//glutWireSphere(1, 10, 10); 		//radius, slices, stacks
//	glutSolidSphere(1, 10, 10); 		//radius, slices, stacks

//	glutWireCone(1.0, 4.0, 24, 20);		//radius, height, slices, stacks
//	glutSolidCone(1.0, 4.0, 4, 2);		//radius, height, slices, stacks

//	glutWireTorus(1.0d, 2.0d, 40, 100);	//inner radius, outer radius, sides, rings
//	glutSolidTorus(1.0d, 2.0d, 4, 100);	//inner radius, outer radius, sides, rings

//	glutWireTeapot(1.0f);
//	glutSolidTeapot(1.0f);
}

void disAvi() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear (GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	keyOperations();

	glTranslatef(0.0f, 0.0f, -5.0f);
	renderSquare();
	glFlush();
}

void reshape (int width, int height) {
	glViewport (0,0,(GLsizei)width, (GLsizei)height);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)height/(GLfloat)width, 1.0, 100.0);
	glMatrixMode (GL_MODELVIEW);
}

void keyDown (unsigned char key, int x, int y){
	if(key == 27) {exit(0);}
	keyStates[key] = true;
}
void keyUp (unsigned char key, int x, int y) {
	keyStates[key] = false;
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Trial");

	createCube();

	glutDisplayFunc(disAvi);
	glutIdleFunc(disAvi);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);

	glutMainLoop();

	return 0;
}

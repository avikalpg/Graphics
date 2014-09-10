#include <stdio.h>
#include <GL/gl.h>		   // Open Graphics Library (OpenGL) header
#include <GL/glut.h>	   // The GL Utility Toolkit (GLUT) Header

#define KEY_ESCAPE 27



typedef struct {
    int width;
	int height;
	char* title;

	float field_of_view_angle;
	float z_near;
	float z_far;
} glutWindow;

glutWindow win;
bool* activeKey = new bool[256];

float initialz =0;
float changez = 0.2;
float initialy = 0;
float changey = 0.02;

void keyOperations() {
	if (activeKey['w']){
		initialz += changez;
	}
	if (activeKey['s']){
		initialz -= changez;
	}
	if (activeKey['a']){
		initialy -= changey;
	}
	if (activeKey['d']){
		initialy += changey;
	}

}

void display() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		     // Clear Screen and Depth Buffer
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,-10.0f);			
	keyOperations();
	glRotatef(initialy, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, initialz);
	
	/*
	 * Triangle code starts here
	 * 3 verteces, 3 colors.
	 */
	 float x;
	 
	 while(x < 101.0f) {
	 glBegin(GL_QUADS);
	 glColor3f(0.1f,0.1f,0.1f);
	 glVertex3f( 1.0f,-1.0f, x);
	 glVertex3f( 1.0f,-1.0f, -x);
	 glVertex3f( -1.0f,-1.0f, -x);
	 glVertex3f( -1.0f,-1.0f, x);
	 glEnd();
	 x=x+1.0f;
	}
	glutSwapBuffers();
}


void initialize () 
{
    glMatrixMode(GL_PROJECTION);												// select projection matrix
    glViewport(0, 0, win.width, win.height);									// set the viewport
    glMatrixMode(GL_PROJECTION);												// set matrix mode
    glLoadIdentity();															// reset projection matrix
    GLfloat aspect = (GLfloat) win.width / win.height;
	gluPerspective(win.field_of_view_angle, aspect, win.z_near, win.z_far);		// set up a perspective projection matrix
    glMatrixMode(GL_MODELVIEW);													// specify which matrix is the current matrix
    glShadeModel( GL_SMOOTH );
    glClearDepth( 1.0f );														// specify the clear value for the depth buffer
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );						// specify implementation-specific hints
	glClearColor(0.2, 1.0, 0.2, 1.0);											// specify clear values for the color buffers								
}


void keyboard ( unsigned char key, int mousePositionX, int mousePositionY )		
{ 
  switch ( key ) 
  {
    case KEY_ESCAPE:        
      exit ( 0 );   
      break;      
      
    default:
	activeKey[key] = true;
      break;
  }
}

void keyUp (unsigned char key, int x, int y){
	activeKey[key] = false;
}

int main(int argc, char **argv) 
{
	// set window values
	win.width = 640;
	win.height = 480;
//	win.title = "TRACK";
	win.field_of_view_angle = 45;
	win.z_near = 1.0f;
	win.z_far = 500.0f;

	// initialize and run program
	glutInit(&argc, argv);                                      // GLUT initialization
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );  // Display Mode
	glutInitWindowSize(win.width,win.height);					// set window size
	glutCreateWindow("Track");								// create Window
	glutDisplayFunc(display);									// register Display Function
	glutIdleFunc( display );									// register Idle Function
	glutKeyboardFunc( keyboard );
	glutKeyboardUpFunc(keyUp);								// register Keyboard Handler
	initialize();
	glutMainLoop();												// run GLUT mainloop
	return 0;
}

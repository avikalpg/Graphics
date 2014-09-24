#include<GL/gl.h>
#include<GL/glut.h>
#include<stdio.h>
#include<string.h>


//globals


GLuint ObjList;
float yrot;


//other functions and main
//.obj loader code


void loadObj(char *fname)
{
	FILE *fp;
	int read;
	GLfloat x, y, z;
	char ch;
	ObjList=glGenLists(1);
	fp=fopen(fname,"r");
	if (!fp)
	{
		printf("can't open file %s\n", fname);
		exit(1);
	}
	glPointSize(3.0);
	glNewList(ObjList, GL_COMPILE);
	{
		glPushMatrix();
		glBegin(GL_TRIANGLE_STRIP);
		while(!(feof(fp)))
		{
			read=fscanf(fp,"%c %f %f %f",&ch,&x,&y,&z);
			// printf("%c: %f %f %f\n",ch,x,y,z);
			if(read==4&& ch=='v')
			{
				glVertex3f(x,y,z);
			}
		}
		glEnd();
	}
	glPopMatrix();
	glEndList();
	fclose(fp);
}
//.obj loader code ends here
void reshape(int w,int h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective (60, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);
	//glOrtho(-25,25,-2,2,0.1,100);
	glMatrixMode(GL_MODELVIEW);
}


void drawObjList()
{
	glPushMatrix();
	glTranslatef(0,-20.00,-105);
	glColor3f(1.0,0.23,0.27);
	glScalef(0.6,0.6,0.6);
	glRotatef(yrot,0,1,0);
	glCallList(ObjList);
	glPopMatrix();
	yrot=yrot+0.6;
	if(yrot>360)yrot=yrot-360;
}


void display(void)
{
	glClearColor (0.0,0.0,0.0,1.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	drawObjList();
	glutSwapBuffers(); //swap the buffers
}

void keyDown(unsigned char key, int x, int y)
{
	if (key == 27)
	{
		exit(0);
	}
}

int main(int argc,char **argv)
{
	glutInit ( &argc , argv );
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(800,450);
	glutInitWindowPosition(20,20);
	glutCreateWindow("ObjLoader");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(display);
	loadObj("media/box.obj");//replace porsche.obj with radar.obj or any other .obj to display it

	glutKeyboardFunc(keyDown);
	glutMainLoop();
	return 0;
}
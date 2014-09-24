#include <stdio.h>
#include <GL/gl.h>		   // Open Graphics Library (OpenGL) header
#include <GL/glut.h>	   // The GL Utility Toolkit (GLUT) Header
#include <math.h>

#define KEY_ESCAPE 27

GLuint ObjList;

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

float initialx =0;
float changex = 0.1;
float initialz =0;
float changez = 0.1;
float initialy = 0;
float changey = 0.005;

//Initializing variables for texturing
GLuint texture;
struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};
typedef struct Image Image;
#define checkImageWidth 64
#define checkImageHeight 64
GLubyte checkImage[checkImageWidth][checkImageHeight][3];
void makeCheckImage(void){
    int i, j, c;
    for (i = 0; i < checkImageWidth; i++) {
        for (j = 0; j < checkImageHeight; j++) {
            c = ((((i&0x8)==0)^((j&0x8)==0)))*255;
            checkImage[i][j][0] = (GLubyte) c;
            checkImage[i][j][1] = (GLubyte) c;
            checkImage[i][j][2] = (GLubyte) c;
        }
    }
}
int ImageLoad(char *filename, Image *image) {
    FILE *file;
    unsigned long size; // size of the image in bytes.
    unsigned long i; // standard counter.
    unsigned short int planes; // number of planes in image (must be 1)
    unsigned short int bpp; // number of bits per pixel (must be 24)
    char temp; // temporary color storage for bgr-rgb conversion.
    // make sure the file is there.
    if ((file = fopen(filename, "rb"))==NULL){
        printf("File Not Found : %s\n",filename);
        return 0;
    }
    // seek through the bmp header, up to the width/height:
    fseek(file, 18, SEEK_CUR);
    // read the width
    if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
        printf("Error reading width from %s.\n", filename);
        return 0;
    }
    //printf("Width of %s: %lu\n", filename, image->sizeX);
    // read the height
    if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
        printf("Error reading height from %s.\n", filename);
        return 0;
    }
    //printf("Height of %s: %lu\n", filename, image->sizeY);
    // calculate the size (assuming 24 bits or 3 bytes per pixel).
    size = image->sizeX * image->sizeY * 3;
    // read the planes
    if ((fread(&planes, 2, 1, file)) != 1) {
        printf("Error reading planes from %s.\n", filename);
        return 0;
    }
    if (planes != 1) {
        printf("Planes from %s is not 1: %u\n", filename, planes);
        return 0;
    }
    // read the bitsperpixel
    if ((i = fread(&bpp, 2, 1, file)) != 1) {
        printf("Error reading bpp from %s.\n", filename);
        return 0;
    }
    if (bpp != 24) {
        printf("Bpp from %s is not 24: %u\n", filename, bpp);
        return 0;
    }
    // seek past the rest of the bitmap header.
    fseek(file, 24, SEEK_CUR);
    // read the data.
    image->data = (char *) malloc(size);
    if (image->data == NULL) {
        printf("Error allocating memory for color-corrected image data");
        return 0;
    }
    if ((i = fread(image->data, size, 1, file)) != 1) {
        printf("Error reading image data from %s.\n", filename);
        return 0;
    }
    for (i=0;i<size;i+=3) { // reverse all of the colors. (bgr -> rgb)
        temp = image->data[i];
        image->data[i] = image->data[i+2];
        image->data[i+2] = temp;
    }
    // we're done.
    return 1;
}

Image * loadTexture(){
    Image *imageRoad;
    // allocate space for texture
    imageRoad = (Image *) malloc(sizeof(Image));
    if (imageRoad == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    if (!ImageLoad("media/road1.bmp", imageRoad)) {
        exit(1);
    }
    return imageRoad;
}

void keyOperations() {
	if (activeKey['w']){
		initialz -= changez*cos(initialy);
		initialx -= changex*sin(initialy);
	}
	if (activeKey['s']){
		initialz += changez*cos(initialy);
		initialx += changex*sin(initialy);
	}
	if (activeKey['a']){
		initialy += changey;
	}
	if (activeKey['d']){
		initialy -= changey;
	}

}

void loadObj(char *fname)
{
	FILE *fp;
	int read;
	GLfloat x, y, z, xprev1=0, zprev1=0, xprev2=0, zprev2=0;
	char ch;
	ObjList=glGenLists(1);
	fp=fopen(fname,"r");
	if (!fp)
	{
		printf("can't open file %s\n", fname);
		exit(1);
	}
	glPointSize(12.0);
	glNewList(ObjList, GL_COMPILE);
	{
		glPushMatrix();
		glBegin(GL_QUAD_STRIP);
		// read=fscanf(fp,"%c %f %f %f",&ch,&x,&y,&z);
		// // glTexCoord2f(0.0, 0.0);
		// if(read==4&&ch=='v')
		// {
		// 	// glVertex3f(x,y,z);
		// 	xprev1 = x;
		// 	zprev1 = z;
		// }
		// read=fscanf(fp,"%c %f %f %f",&ch,&x,&y,&z);
		// if(read==4&&ch=='v')
		// {
		// 	// glVertex3f(x,y,z);
		// 	xprev2 = x;
		// 	zprev2 = z;
		// }
		while(!(feof(fp)))
		{
			// glTexCoord2f(0.0, 1.0);
			// 	glVertex3f( xprev1,y,zprev1);
			// glTexCoord2f(1.0, 1.0);
			// 	glVertex3f( xprev2,y,zprev2);
			// read=fscanf(fp,"%c %f %f %f",&ch,&x,&y,&z);
			// glTexCoord2f(0.0, 0.0);
			// if(read==4&&ch=='v')
			// {
			// 	glVertex3f(x,y,z);
			// 	xprev1 = x;
			// 	zprev1 = z;
			// }
			read=fscanf(fp,"%c %f %f %f",&ch,&x,&y,&z);
			glTexCoord2f(1.0, 0.0);
			if(read==4&&ch=='v')
			{
				glVertex3f(x,y,z);
				// xprev2 = x;
				// zprev2 = z;
			}
			/*read=fscanf(fp,"%c %f %f %f",&ch,&x,&y,&z);
			if(read==4&&ch=='v')
			{
				glVertex3f(x,y,z);
			}*/
		}
		glEnd();
	}
	glPopMatrix();
	glEndList();
	fclose(fp);
}

void display() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		     // Clear Screen and Depth Buffer
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,-10.0f);			
	keyOperations();
	// glRotatef(initialy, 0.0f, 1.0f, 0.0f);
	gluLookAt(initialx,0,initialz,
		initialx-sin(initialy),0,initialz-cos(initialy),
		0,1,0);
	// glTranslatef(0.0f, 0.0f, initialz);
	
	float x;
	 
	while(x < 52.0f) {
		 glBegin(GL_QUADS);
	//	 glColor3f(0.1f,0.1f,0.1f);
		glTexCoord2f(0.0, 0.0);
		 glVertex3f( 1.0f,-1.0f, x);
		glTexCoord2f(0.0, 1.0);
		 glVertex3f( 1.0f,-1.0f, -x);
		glTexCoord2f(1.0, 1.0);
		 glVertex3f( -1.0f,-1.0f, -x);
		glTexCoord2f(1.0, 0.0);
		 glVertex3f( -1.0f,-1.0f, x);
		 glEnd();
		 x=x+1.0f;
	}
	glPushMatrix();
	glTranslatef(0,-2.00,-105);
	// glColor3f(1.0,0.23,0.27);
	glScalef(0.6,0.6,0.6);
	glCallList(ObjList);
	glPopMatrix();
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

	Image *image1 = loadTexture();
	if(image1 == NULL){
	    printf("Image was not returned from loadTexture\n");
	    exit(0);
	}
	makeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// Create Texture
	//glGenTextures(2, texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);


//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_FLAT);

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

	loadObj("track/race.obj");//replace porsche.obj with radar.obj or any other .obj to display it
	initialize();
	glutMainLoop();												// run GLUT mainloop
	return 0;
}

#include <stdio.h>
#include <GL/gl.h>		   // Open Graphics Library (OpenGL) header
#include <GL/glut.h>	   // The GL Utility Toolkit (GLUT) Header
#include <math.h>
#include <vector>
#include <cmath>
#include <string.h>

#define KEY_ESCAPE 27
#define PI 3.14159
#define FPS 25				//number of frames rendered persecond

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

typedef struct{ 					//All these variables have to be changed for each car
	float maxTurn;
	float maxSpeed;
	float acc;
	float turnControl;
	char* objFileName;
} car;
car myCar;

typedef struct {
	int id;
	float friction;
} raceTrack;
raceTrack currTrack;

bool* activeKey = new bool[256];

float speed=0;
float turn=0;
float initialx =0;
float initialz=0;
float rotY=0;

//Initializing variables for texturing
GLuint texture[4];
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

void loadTexture(){
    Image *image1;
    // allocate space for texture
    image1 = (Image *) malloc(sizeof(Image));
    if (image1 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    if (!ImageLoad("media/road1.bmp", image1)) {
        exit(1);
    }
	// if(image1 == NULL){
	//     printf("Image was not returned from loadTexture\n");
	//     exit(0);
	// }
	makeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// Create Texture
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);

    if (!ImageLoad("media/grass01.bmp", image1)) {
        exit(1);
    }
	// if(image1 == NULL){
	//     printf("Image was not returned from loadTexture\n");
	//     exit(0);
	// }
	makeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// Create Texture
	glGenTextures(2, &texture[1]);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);

	if (!ImageLoad("media/night-sky.bmp", image1)) {
        exit(1);
    }
	// if(image1 == NULL){
	//     printf("Image was not returned from loadTexture\n");
	//     exit(0);
	// }
	makeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// Create Texture
	glGenTextures(3, &texture[2]);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);

/*	if (!ImageLoad("media/background02.bmp", image1)) {
        exit(1);
    }
	// if(image1 == NULL){
	//     printf("Image was not returned from loadTexture\n");
	//     exit(0);
	// }
	makeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// Create Texture
	glGenTextures(3, &texture[3]);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);
*/

//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

}

void keyOperations() {
	if (activeKey['w'] || activeKey['W']){
		if (speed > (-1)*myCar.maxSpeed)
		{
			printf("the speed of the car is not able to go beyond %f, where limit is %s\n", speed, glGetString(GL_VERSION));
			speed -= myCar.acc;
		}
	}
	if (activeKey['s'] || activeKey['S']){
		if (speed < myCar.maxSpeed)
		{
			speed += myCar.acc;
		}
	}
	if (activeKey['a'] || activeKey['A']){
		if (turn > (-1)*myCar.maxTurn)
		{
			turn -= myCar.turnControl;
		}
	}
	if (activeKey['d'] || activeKey['D']){
		if (turn < myCar.maxTurn)
		{
			turn += myCar.turnControl;
		}
	}
	printf("speed = %f, and turn = %f\n => rotation = %f\n", speed, turn, rotY);
	speed -= speed*(currTrack.friction);
	rotY = rotY*180/PI; 
	rotY += (turn*speed)/FPS;
	if (rotY >= 180)
	{
		rotY -= 360;
	}
	else if (rotY < -180)
	{
		rotY += 360;
	}
	if ((speed != 0)&& (turn != 0))
	{
		if (turn*speed > 0)
		{
			turn -= 1*speed;
		}
		else {
			turn += 1*speed;
		}
	}
	rotY = (rotY*PI)/180;
	if (initialz > 250.0f)
	{
		speed = 0;
		initialz = 250.0f;
	}
	if (initialz < -100.0f)
	{
		speed =0;
		initialz = -100.0f;
	}
	if (initialx > 100.0f)
	{
		speed = 0;
		initialx = 100.0f;
	}
	else if (initialx < -100.0f)
	{
		speed =0;
		initialx = -100.0f;
	}
	initialx += speed*sin(rotY);
	initialz += speed*cos(rotY);
}

/*
* we do not need the object
*
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
	glPointSize(2.0);
	float flag1=0, flag2=0;
	glNewList(ObjList, GL_COMPILE);
	{
		glPushMatrix();

		glRotatef(180, 0.0f, 1.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, 10.0f);

		glTranslatef(10.0f, 0.0f, -2.0f);
		// glTranslatef(initialx-sin(rotY),1,initialz-cos(rotY));
		// glTranslatef(-initialx+sin(rotY),0,-initialz+cos(rotY));
		// glRotatef(180, 0.0f, -1.0f, 0.0f);
		glBegin(GL_POINTS);
		while(!(feof(fp)))
		{
			read=fscanf(fp,"%c %f %f %f",&ch,&x,&y,&z);
			if(read==4&&ch=='v')
			{
				glTexCoord2f(flag1, flag2);
				glVertex3f(x,y,z);
				if (flag2==0)
				{
					flag2 =1;
				}
				else {
					flag2 = 0;
				}
				if (flag1==0)
				{
					flag1 =1;
				}
				else {
					flag1 = 0;
				}
			}
		}
		glEnd();
		glPopMatrix();
	}
	glEndList();
	fclose(fp);
}
*/

// void DrawEnclosingSphere(float radius, unsigned int rings, unsigned int sectors)
// {

// }

void display() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		     // Clear Screen and Depth Buffer
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,-10.0f);			
	keyOperations();
	// glRotatef(initialy, 0.0f, 1.0f, 0.0f);
	gluLookAt(initialx,1,initialz,
		initialx-sin(rotY),1,initialz-cos(rotY),
		0,1,0);
	// glTranslatef(0.0f, 0.0f, initialz);
	
	glPushMatrix();
	glTranslatef(6,-2.00,5);
	/*
	* drawing the enclosing box
	*/
	glColor3f(0.5f, 0.8f, 0.5f);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);
	for (int i = -100.0; i < 250.0f; i+=25.0f)
	{
		for (int j = -100.0f; j < 100.0f; j+=25.0f)
		{
			glTexCoord2f(0.0, 0.0);
				glVertex3f(j,-0.0001f, i+25.0f);
			glTexCoord2f(0.0, 1.0);
				glVertex3f(j,-0.0001f,i);
			glTexCoord2f(1.0, 1.0);
				glVertex3f(j+25.0f,-0.0001f,i);
			glTexCoord2f(1.0, 0.0);
				glVertex3f(j+25.0f,-0.0001f, i+25.0f);			
		}
	}
	glEnd();
	glColor3f(0.75f, 0.75f, 0.75f);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	// DrawEnclosingSphere(300, 10, 10);
	glBegin(GL_QUADS);
	for (int i = -100.0; i < 250.0f; i+=25.0f)
	{
		for (int j = -100.0f; j < 100.0f; j+=25.0f)
		{
			glTexCoord2f(0.0, 0.0);
				glVertex3f(j,25.0f, i+25.0f);
			glTexCoord2f(0.0, 1.0);
				glVertex3f(j,25.0f,i);
			glTexCoord2f(1.0, 1.0);
				glVertex3f(j+25.0f,25.0f,i);
			glTexCoord2f(1.0, 0.0);
				glVertex3f(j+25.0f,25.0f, i+25.0f);			
		}
	}
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
	// glBindTexture(GL_TEXTURE_2D, texture[3]);
	// DrawEnclosingSphere(300, 10, 10);
	glBegin(GL_QUADS);
	for (int i = -100.0f; i < 100.0f; i+=25.0f)
	{
		glTexCoord2f(0.0, 0.0);
			glVertex3f(i, 0.0f, -100.0f);
		glTexCoord2f(1.0, 0.0);
			glVertex3f(i + 25.0f, 0.0f, -100.0f);
		glTexCoord2f(1.0, 1.0);
			glVertex3f(i + 25.0f, 25.0f, -100.0f);
		glTexCoord2f(0.0, 1.0);
			glVertex3f(i, 25.0f, -100.0f);
	}
	for (int i = -100.0f; i < 100.0f; i+=25.0f)
	{
		glTexCoord2f(0.0, 0.0);
			glVertex3f(i, 0.0f, 250.0f);
		glTexCoord2f(1.0, 0.0);
			glVertex3f(i + 25.0f, 0.0f, 250.0f);
		glTexCoord2f(1.0, 1.0);
			glVertex3f(i + 25.0f, 25.0f, 250.0f);
		glTexCoord2f(0.0, 1.0);
			glVertex3f(i, 25.0f, 250.0f);
	}
		for (int i = -100.0f; i < 250.0f; i+=25.0f)
	{
		glTexCoord2f(0.0, 0.0);
			glVertex3f(-100.0f, 0.0f,i);
		glTexCoord2f(1.0, 0.0);
			glVertex3f(-100.0f, 0.0f,i+25.0f);
		glTexCoord2f(1.0, 1.0);
			glVertex3f(-100.0f, 25.0f,i+25.0f);
		glTexCoord2f(0.0, 1.0);
			glVertex3f(-100.0f, 25.0f,i);
	}
		for (int i = -100.0f; i < 250.0f; i+=25.0f)
	{
		glTexCoord2f(0.0, 0.0);
			glVertex3f(100.0f, 0.0f,i);
		glTexCoord2f(1.0, 0.0);
			glVertex3f(100.0f, 0.0f,i+25.0f);
		glTexCoord2f(1.0, 1.0);
			glVertex3f(100.0f, 25.0f,i+25.0f);
		glTexCoord2f(0.0, 1.0);
			glVertex3f(100.0f, 25.0f,i);
	}
	glEnd();

	/*
	* drawing the track
	*/
	glColor3f(3.0f,3.0f,3.0f);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	if (currTrack.id == 2) {
		glBegin(GL_QUADS);

		glTexCoord2f(0.0, 0.0);
		 glVertex3f( -69.6982f, 0.0000f, 146.3044f);
		glTexCoord2f(1.0, 0.0);
		 glVertex3f( -44.6982f, 0.0000f, 146.3044f);
		glTexCoord2f(1.0, 1.0);
		 glVertex3f( -44.6982f, 0.0000f, -3.6956f);
		glTexCoord2f(0.0, 1.0);
		 glVertex3f( -69.6982f, 0.0000f, -3.6956f);

		glTexCoord2f(0.0, 0.0);
		 glVertex3f( -69.6982f, 0.0000f, -3.6956f);
		glTexCoord2f(1.0, 0.0);
		 glVertex3f( -44.6982f, 0.0000f, -3.6956f);
		glTexCoord2f(1.0, 1.0);
		 glVertex3f( -41.5202f, 0.0000f, -20.8055f);
		glTexCoord2f(0.0, 1.0);
		 glVertex3f( -65.0125f, 0.0000f, -29.3560f);

		glTexCoord2f(0.0, 0.0);
		 glVertex3f( -65.0125f, 0.0000f, -29.3560f);
		glTexCoord2f(1.0, 0.0);
		 glVertex3f( -41.5202f, 0.0000f, -20.8055f);
		glTexCoord2f(1.0, 1.0);
		 glVertex3f( -32.8378f, 0.0000f, -35.8439f);
		glTexCoord2f(0.0, 1.0);
		 glVertex3f( -51.9889f, 0.0000f, -51.9136f);

		glTexCoord2f(0.0, 0.0);
		 glVertex3f( -51.9889f, 0.0000f, -51.9136f);
		glTexCoord2f(1.0, 0.0);
		 glVertex3f( -32.8378f, 0.0000f, -35.8439f);
		glTexCoord2f(1.0, 1.0);
		 glVertex3f( -19.5355f, 0.0000f, -47.0058f);
		glTexCoord2f(0.0, 1.0);
		 glVertex3f( -32.0355f, 0.0000f, -68.6564f);

		glTexCoord2f(0.0, 0.0);
		 glVertex3f( -32.0355f, 0.0000f, -68.6564f);
		glTexCoord2f(1.0, 0.0);
		 glVertex3f( -19.5355f, 0.0000f, -47.0058f);
		glTexCoord2f(1.0, 1.0);
		 glVertex3f( -3.2180f, 0.0000f, -52.9449f);
		glTexCoord2f(0.0, 1.0);
		 glVertex3f( -7.5592f, 0.0000f, -77.5651f);

		glTexCoord2f(0.0, 0.0);
		 glVertex3f( -7.5592f, 0.0000f, -77.5651f);
		glTexCoord2f(1.0, 0.0);
		 glVertex3f( -3.2180f, 0.0000f, -52.9449f);
		glTexCoord2f(1.0, 1.0);
		 glVertex3f( 14.1469f, 0.0000f, -52.9449f);
		glTexCoord2f(0.0, 1.0);
		 glVertex3f( 18.4881f, 0.0000f, -77.5651f);

		glTexCoord2f(0.0, 0.0);
		 glVertex3f( 18.4881f, 0.0000f, -77.5651f);
		glTexCoord2f(1.0, 0.0);
		 glVertex3f( 14.1469f, 0.0000f, -52.9449f);
		glTexCoord2f(1.0, 1.0);
		 glVertex3f( 30.4644f, 0.0000f, -47.0058f);
		glTexCoord2f(0.0, 1.0);
		 glVertex3f( 42.9644f, 0.0000f, -68.6564f);

		glTexCoord2f(0.0, 0.0);
		 glVertex3f( 42.9644f, 0.0000f, -68.6564f);
		glTexCoord2f(1.0, 0.0);
		 glVertex3f( 30.4644f, 0.0000f, -47.0058f);
		glTexCoord2f(1.0, 1.0);
		 glVertex3f( 43.7667f, 0.0000f, -35.8439f);
		glTexCoord2f(0.0, 1.0);
		 glVertex3f( 62.9178f, 0.0000f, -51.9136f);

		glTexCoord2f(0.0, 0.0);
		 glVertex3f( 62.9178f, 0.0000f, -51.9136f);
		glTexCoord2f(1.0, 0.0);
		 glVertex3f( 43.7667f, 0.0000f, -35.8439f);
		glTexCoord2f(1.0, 1.0);
		 glVertex3f( 52.4491f, 0.0000f, -20.8055f);
		glTexCoord2f(0.0, 1.0);
		 glVertex3f( 75.9414f, 0.0000f, -29.3560f);

		glTexCoord2f(0.0, 0.0);
		 glVertex3f( 75.9414f, 0.0000f, -29.3560f);
		glTexCoord2f(1.0, 0.0);
		 glVertex3f( 52.4491f, 0.0000f, -20.8055f);
		glTexCoord2f(1.0, 1.0);
		 glVertex3f( 55.4644f, 0.0000f, -3.7045f);
		glTexCoord2f(0.0, 1.0);
		 glVertex3f( 80.4644f, 0.0000f, -3.7045f);

			glTexCoord2f(0.0, 0.0);
			 glVertex3f( 80.4644f, 0.0000f, -3.7045f);
			glTexCoord2f(1.0, 0.0);
			 glVertex3f( 55.4644f, 0.0000f, -3.7045f);
			glTexCoord2f(1.0, 1.0);
			 glVertex3f( 55.4316f, 0.0000f, 146.3044f);
			glTexCoord2f(0.0, 1.0);
			 glVertex3f( 80.4316f, 0.0000f, 146.3044f);

		glTexCoord2f(0.0, 0.0);
		 glVertex3f( 80.4316f, 0.0000f, 146.3044f);
		glTexCoord2f(1.0, 0.0);
		 glVertex3f( 55.4316f, 0.0000f, 146.3044f);
		glTexCoord2f(1.0, 1.0);
		 glVertex3f( 52.4491f, 0.0000f, 163.3138f);
		glTexCoord2f(0.0, 1.0);
		 glVertex3f( 75.9414f, 0.0000f, 171.8643f);

		glTexCoord2f(0.0, 0.0);
		 glVertex3f( 75.9414f, 0.0000f, 171.8643f);
		glTexCoord2f(1.0, 0.0);
		 glVertex3f( 52.4491f, 0.0000f, 163.3138f);
		glTexCoord2f(1.0, 1.0);
		 glVertex3f( 43.7667f, 0.0000f, 178.3522f);
		glTexCoord2f(0.0, 1.0);
		 glVertex3f( 62.9178f, 0.0000f, 194.4219f);

		glTexCoord2f(0.0, 0.0);
		 glVertex3f( 62.9178f, 0.0000f, 194.4219f);
		glTexCoord2f(1.0, 0.0);
		 glVertex3f( 43.7667f, 0.0000f, 178.3522f);
		glTexCoord2f(1.0, 1.0);
		 glVertex3f( 30.4644f, 0.0000f, 189.5141f);
		glTexCoord2f(0.0, 1.0);
		 glVertex3f( 42.9644f, 0.0000f, 211.1647f);

		glTexCoord2f(0.0, 0.0);
		 glVertex3f( 42.9644f, 0.0000f, 211.1647f);
		glTexCoord2f(1.0, 0.0);
		 glVertex3f( 30.4644f, 0.0000f, 189.5141f);
		glTexCoord2f(1.0, 1.0);
		 glVertex3f( 14.1468f, 0.0000f, 195.4532f);
		glTexCoord2f(0.0, 1.0);
		 glVertex3f( 18.4880f, 0.0000f, 220.0734f);

		glTexCoord2f(0.0, 0.0);
		 glVertex3f( 18.4880f, 0.0000f, 220.0734f);
		glTexCoord2f(1.0, 0.0);
		 glVertex3f( 14.1468f, 0.0000f, 195.4532f);
		glTexCoord2f(1.0, 1.0);
		 glVertex3f( -3.2180f, 0.0000f, 195.4532f);
		glTexCoord2f(0.0, 1.0);
		 glVertex3f( -7.5592f, 0.0000f, 220.0734f);

		glTexCoord2f(0.0, 0.0);
		 glVertex3f( -7.5592f, 0.0000f, 220.0734f);
		glTexCoord2f(1.0, 0.0);
		 glVertex3f( -3.2180f, 0.0000f, 195.4532f);
		glTexCoord2f(1.0, 1.0);
		 glVertex3f( -19.5356f, 0.0000f, 189.5141f);
		glTexCoord2f(0.0, 1.0);
		 glVertex3f( -32.0356f, 0.0000f, 211.1647f);

		glTexCoord2f(0.0, 0.0);
		 glVertex3f( -32.0356f, 0.0000f, 211.1647f);
		glTexCoord2f(1.0, 0.0);
		 glVertex3f( -19.5356f, 0.0000f, 189.5141f);
		glTexCoord2f(1.0, 1.0);
		 glVertex3f( -32.8378f, 0.0000f, 178.3522f);
		glTexCoord2f(0.0, 1.0);
		 glVertex3f( -51.9889f, 0.0000f, 194.4219f);

		glTexCoord2f(0.0, 0.0);
		 glVertex3f( -51.9889f, 0.0000f, 194.4219f);
		glTexCoord2f(1.0, 0.0);
		 glVertex3f( -32.8378f, 0.0000f, 178.3522f);
		glTexCoord2f(1.0, 1.0);
		 glVertex3f( -41.5202f, 0.0000f, 163.3138f);
		glTexCoord2f(0.0, 1.0);
		 glVertex3f( -65.0125f, 0.0000f, 171.8643f);

		glTexCoord2f(0.0, 0.0);
		 glVertex3f( -65.0125f, 0.0000f, 171.8643f);
		glTexCoord2f(1.0, 0.0);
		 glVertex3f( -41.5202f, 0.0000f, 163.3138f);
		glTexCoord2f(1.0, 1.0);
		 glVertex3f( -44.6982f, 0.0000f, 146.3044f);
		glTexCoord2f(0.0, 1.0);
		 glVertex3f( -69.6982f, 0.0000f, 146.3044f);

		glEnd();
	}
	else if (currTrack.id == 3) {	
		glTranslatef(-40.0f, 0.0f, 100.0f);
		glBegin(GL_QUADS);
		
		glTexCoord2f(0.0, 0.0);
		 glVertex3f( 0.2048f, 0.0000f, 16.3703f);
		glTexCoord2f(0.0, 1.0);
		 glVertex3f( 0.2048f, 0.0000f, 86.3703f);
		glTexCoord2f(1.0, 1.0);
		 glVertex3f( 25.2048f, 0.0000f, 86.3703f);
		glTexCoord2f(1.0, 0.0);
		 glVertex3f( 25.2048f, 0.0000f, 16.3703f);

		glTexCoord2f(0.0, 0.0);
		 glVertex3f( 29.8147f, 0.0000f, -53.3839f);
		glTexCoord2f(0.0, 1.0);
		 glVertex3f( 0.2048f, 0.0000f, 16.3703f);
		glTexCoord2f(1.0, 1.0);
		 glVertex3f( 25.2048f, 0.0000f, 16.3703f);
		glTexCoord2f(1.0, 0.0);
		 glVertex3f( 54.8147f, 0.0000f, -53.3839f);

		glTexCoord2f(0.0, 0.0);
		 glVertex3f( 29.8148f, 0.0000f, -123.4015f);
		glTexCoord2f(0.0, 1.0);
		 glVertex3f( 29.8147f, 0.0000f, -53.3839f);
		glTexCoord2f(1.0, 1.0);
		 glVertex3f( 54.8147f, 0.0000f, -53.3839f);
		glTexCoord2f(1.0, 0.0);
		 glVertex3f( 54.8148f, 0.0000f, -123.4015f);

		glTexCoord2f(0.0, 0.0);
		 glVertex3f( -4.2518f, 0.0000f, -193.4343f);
		glTexCoord2f(0.0, 1.0);
		 glVertex3f( 29.8148f, 0.0000f, -123.4015f);
		glTexCoord2f(1.0, 1.0);
		 glVertex3f( 54.8148f, 0.0000f, -123.4015f);
		glTexCoord2f(1.0, 0.0);
		 glVertex3f( 20.7482f, 0.0000f, -193.4343f);

		 glEnd();
	}
	else {
		currTrack.id++;
	}

	/*
	* Loading the object - car
	*/
	glTranslatef(initialx-sin(rotY),1,initialz-cos(rotY));
	glColor3f(1.0,0.23,0.27);
	glScalef(0.6,0.6,0.6);
	glRotatef(rotY, 0.0f, 1.0f, 0.0f);
	glCallList(ObjList);
	glRotatef(rotY, 0.0f, -1.0f, 0.0f);
	// glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	
	// glTranslatef(-(initialx-sin(rotY)),0,-(initialz-cos(rotY)));

	glPopMatrix();
	/*
	* Trying to do some HUD here
	*/
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, win.width, win.height, 0.0, -1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);

	glClear(GL_DEPTH_BUFFER_BIT);

	glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex2f(0.0, 0.0);
		glVertex2f(win.width, 0.0);
		glVertex2f(win.width, 100.0);
		glVertex2f(0.0, 100.0);
	glEnd();

	/*
	* Trying to write some stuff inside the HUD created.
	*/
	// char* myCharString = "Yo babes";
	// glutStrokeCharacter(GLUT_STROKE_ROMAN, 1/*myCharString*/);
	char quote[80];
	glScalef(0.4, 0.4, 0.5);
	glTranslatef(0.0f,150.0f, 0.0f);
	glRotatef(180, 1.0, 0.0, 0.0);
	sprintf(quote, "Speed = %.0f       Turn Angle=%.0f", -50*speed, turn);
    int lenghOfQuote = (int)strlen(quote);
    glPushMatrix();
    // glTranslatef(-(lenghOfQuote*37), -(l*200), 0.0);
    for (int i = 0; i < lenghOfQuote; i++)
    {
    	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
        // glColor3f((UpwardsScrollVelocity/10)+300+(l*10),(UpwardsScrollVelocity/10)+300+(l*10),0.0);
        glutStrokeCharacter(GLUT_STROKE_ROMAN, quote[i]);
    }
    glPopMatrix();

	//Make sure we can render in 3d again
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
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
	glClearColor(0.1, 0.2, 0.5, 1.0);											// specify clear values for the color buffers								
	loadTexture();
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
    
    case 't':
    	currTrack.id ++;
    	currTrack.id = currTrack.id %4;
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
	win.width = 1380;
	win.height = 600;
//	win.title = "TRACK";
	win.field_of_view_angle = 45;
	win.z_near = 1.0f;
	win.z_far = 500.0f;

	myCar.maxTurn = 65;
	myCar.maxSpeed = 4.0;
	myCar.acc = 0.02;
	myCar.turnControl = 1.6;
	myCar.objFileName = "media/avi.obj";

	currTrack.id = 2;
	currTrack.friction = 0.01;

	// initialize and run program
	glutInit(&argc, argv);                                      // GLUT initialization
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );  // Display Mode
	glutInitWindowSize(win.width,win.height);					// set window size
	glutCreateWindow("Car Simulator");									// create Window
	glutDisplayFunc(display);									// register Display Function
	glutIdleFunc( display );									// register Idle Function
	glutKeyboardFunc( keyboard );
	glutKeyboardUpFunc(keyUp);								// register Keyboard Handler

	// loadObj(myCar.objFileName);								//replace porsche.obj with radar.obj or any other .obj to display it
	initialize();
	glutMainLoop();											// run GLUT mainloop
	return 0;
}

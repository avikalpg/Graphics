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

float length(float array[]){
	float d;

}

/* Copied code of the Phong's model
*	Starts here 
*/
struct Lighting
{
    float Diffuse[3];
    float Specular[3];
};
 
struct PointLight
{
	float position[3];
	float diffuseColor[3];
	float  diffusePower;
	float specularColor[3];
	float  specularPower;
};
 
Lighting GetPointLight( PointLight light, float *pos3D, float *viewDir, float *normal )
{
	int i;
	Lighting OUT;
	if( light.diffusePower > 0 )
	{
		float lightDir[3]; //3D position in space of the surface
		for(i=0; i<3; i++){
			lightDir[i] = light.position[i] - pos3D[i];
		}
		float distance = length( lightDir );
		lightDir = lightDir / distance; // = normalize( lightDir );
		distance = distance * distance; //This line may be optimised using Inverse square root
 
		//Intensity of the diffuse light. Saturate to keep within the 0-1 range.
		float NdotL = dot( normal, lightDir );
		float intensity = saturate( NdotL );
 
		// Calculate the diffuse light factoring in light color, power and the attenuation
		OUT.Diffuse = intensity * light.diffuseColor * light.diffusePower / distance;
 
		//Calculate the half vector between the light vector and the view vector.
		//This is faster than calculating the actual reflective vector.
		float3 H = normalize( lightDir + viewDir );
 
		//Intensity of the specular light
		float NdotH = dot( normal, H );
		intensity = pow( saturate( NdotH ), specularHardness );
 
		//Sum up the specular light factoring
		OUT.Specular = intensity * light.specularColor * light.specularPower / distance; 
	}
	return OUT;
}

/*	Ends here */




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
	glRotatef(45, 0.5f, 1.0f, 0.0f);
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

	glColor4f(0.0, 0.2, 0.5, 1.0);
	//glutWireSphere(1, 10, 10); 		//radius, slices, stacks
//	glutSolidSphere(1, 10, 10); 		//radius, slices, stacks

//	glutWireCone(1.0, 4.0, 24, 20);		//radius, height, slices, stacks
	glutSolidCone(1.0, 4.0, 24, 2);		//radius, height, slices, stacks

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

	glTranslatef(0.0f, 0.0f, -10.0f);
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

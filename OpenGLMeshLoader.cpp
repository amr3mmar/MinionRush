#include <stdlib.h>
#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>
#include <math.h>

int WIDTH = 1280;
int HEIGHT = 720;

GLuint tex;
char title[] = "Minion Rush";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 100;

//  Normal Minion
float ynormal = 1.5;
float znormal = 30;
bool normal_up = true;
int normal_angle = 0;
bool move_normal = false;
// Pirate Minion
float xPirate = 0.0;
float yPirate = 0.2;
float tPirate = 0;
int swordAngle = 0;
int sword_swing = 0;
bool move_pirate = false;


float cam_angle = 0.0;
int stop_rotation = 0;

float zground1 = 0;
float zground2 = -40;
float zground3 = -80;
float zground4 = 40;


class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};

Vector Eye(0.04, 5, 25);
Vector At(0, 0, 0);
Vector Up(0, 1, 0);

int cameraZoom = 0;

// Model Variables
Model_3DS model_house;
Model_3DS model_tree;

// Textures
GLTexture tex_ground;

//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT0);

	// Define Light source 0 ambient light
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Finally, define light source 0 position in World Space
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************************************************************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************************************************************************//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	//*******************************************************************************************//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************************************************************************//

	InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}

//=======================================================================
// Render Ground Function
//=======================================================================
void RenderGround()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, -20);
	glTexCoord2f(5, 0);
	glVertex3f(20, 0, -20);
	glTexCoord2f(5, 5);
	glVertex3f(20, 0, 20);
	glTexCoord2f(0, 5);
	glVertex3f(-20, 0, 20);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

//=======================================================================
// Display Function
//=======================================================================
void pirateMinion() {

	GLUquadricObj *p = gluNewQuadric();


	glPushMatrix();
	glTranslated(0, 4, 0);



	// Body
	glPushMatrix();
	glColor3d(1, 1, 0);
	glTranslated(0.0, 0, 0.0);
	glScaled(0.7, 0.7, 0.7);
	gluQuadricDrawStyle(p, GLU_FILL);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(p, 2.0, 2.0, 3, 100, 100);
	glPopMatrix();


	// Bottom
	glPushMatrix();
	glColor3d(0, 0, 0);
	glTranslated(0.0, -1.8, 0.0);
	glScaled(0.7, 0.7, 0.7);
	gluQuadricDrawStyle(p, GLU_FILL);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(p, 1.95, 1.95, 3.3, 100, 100);
	glPopMatrix();

	//belt 
	glPushMatrix();
	glColor3d(0.447059, 0.164706, 0.164706);
	glTranslated(0, -2.4, -0.08);
	glScaled(0.7, 0.7, 0.7);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(p, 2.1, 2.10, 0.6, 100, 100);
	glPopMatrix();

	// X sign
	glPushMatrix();
	glColor3d(1, 1, 1);
	glTranslated(-0.7, -3.6, 1.1);
	glScaled(0.2, 0.2, 0.2);
	gluQuadricDrawStyle(p, GLU_FILL);
	glRotatef(90.0, 0, 1.0f, 0.7f);
	gluCylinder(p, 0.6, 0.6, 6, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3d(1, 1, 1);
	glTranslated(-0.7, -3.05, 1.2);
	glScaled(0.2, 0.2, 0.2);
	gluQuadricDrawStyle(p, GLU_FILL);
	glRotatef(90.0, 0, 1.0f, -0.5f);
	gluCylinder(p, 0.6, 0.6, 6, 100, 100);
	glPopMatrix();


	//head 
	glPushMatrix();
	glColor3d(0, 0, 0);
	glTranslated(0, 0, 0.0);
	glScaled(0.7, 0.7, 0.7);
	glutSolidSphere(2.0, 100, 100);
	glPopMatrix();



	glPushMatrix();
	glScaled(0.8, 0.8, 0.8);
	glTranslated(0, -0.5, 0);
	//mask 
	glPushMatrix();
	glColor3d(0.447059, 0.164706, 0.164706);
	glTranslated(0, -0.1, 1.3);
	glScaled(0.35, 0.35, 0.35);
	gluCylinder(p, 2.5, 2.5, 2, 100, 100);
	glPopMatrix();

	//mask thickness 
	glPushMatrix();
	glColor3d(0.447059, 0.164706, 0.164706);
	glTranslated(0, -0.1, 2);
	glScaled(0.35, 0.35, 0.35);
	gluDisk(p, 2.2, 3.0, 100, 100);
	glPopMatrix();

	//Eye white
	glPushMatrix();
	glColor3d(1, 1, 1);
	glTranslated(0, -0.15, 1);
	glScaled(0.3, 0.34, 0.3);
	glutSolidSphere(3.0, 100, 100);
	glPopMatrix();

	//Eye colour
	//brown colour 0.447059, 0.164706, 0.164706
	glPushMatrix();
	glColor3d(0, 0, 0);
	glTranslated(0.2, -0.2, 1.6);
	glScaled(0.3, 0.34, 0.3);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	//Eye ball
	glPushMatrix();
	glColor3d(0, 0, 0);
	glTranslated(0.22, -0.25, 1.9);
	glScaled(0.3, 0.34, 0.3);
	glutSolidSphere(0.5, 100, 100);
	glPopMatrix();

	glPopMatrix();


	glPushMatrix();
	glRotated(45, 1, 1.2, 0.75);
	glTranslated(0, 0.8, 0);

	//left arm
	glPushMatrix();
	glColor3d(1, 1, 0);
	glTranslated(-1, -1.6, 0.0);
	glRotated(-90, 0, 1, 0);
	glScaled(0.15, 0.15, 0.15);
	gluCylinder(p, 1.5, 1.3, 10, 100, 100);
	glPopMatrix();

	//left gloves
	glPushMatrix();
	glColor3d(0, 0, 0);
	glTranslated(-2.4, -1.6, 0.0);
	glRotated(-90, 0, 1, 0);
	glScaled(0.15, 0.15, 0.15);
	gluCylinder(p, 1.3, 1.3, 3.2, 100, 100);
	glPopMatrix();

	//left hand
	glPushMatrix();
	glColor3d(0, 0, 0);
	glTranslated(-3, -1.6, 0.0);
	glRotated(-90, 0, 1, 0);
	glScaled(0.15, 0.15, 0.15);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	glPopMatrix();


	//right arm
	glPushMatrix();
	glColor3d(1, 1, 0);
	glTranslated(1, -1.6, 0.0);
	glRotated(90, 0, 1, 0);
	glScaled(0.15, 0.15, 0.15);
	gluCylinder(p, 1.5, 1.3, 12, 100, 100);
	glPopMatrix();

	//right gloves
	glPushMatrix();
	glColor3d(1, 1, 0);
	glTranslated(1, -1.6, 0.0);
	glRotated(90, 0, 1, 0);
	glScaled(0.15, 0.15, 0.15);
	gluCylinder(p, 1.5, 1.3, 12, 100, 100);
	glPopMatrix();

	//right hand
	glPushMatrix();
	glColor3d(0.5, 0.5, 0.5);
	glTranslated(2.8, -1.6, 0.0);
	glRotated(90, 0, 1, 0);
	glScaled(0.15, 0.15, 0.15);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glRotated(swordAngle, 1, 0, 0);
	glRotated(sword_swing, 0, 1, 0);

	//sword base
	glPushMatrix();
	glColor3d(0.8, 0.5, 0.2);
	glTranslated(3, -1.6, 0.0);
	glScaled(0.15, 0.15, 0.15);
	glutSolidSphere(2, 100, 100);
	glPopMatrix();

	//sword blade
	glPushMatrix();
	glColor3d(0.5, 0.5, 0.5);
	glTranslated(3, -1.5, 0.0);
	glRotated(-90, 1, 0, 0.5);
	glScaled(0.15, 0.15, 0.15);
	gluCylinder(p, 0.8, 0.2, 25, 100, 100);
	glPopMatrix();

	glPopMatrix();

	// left leg
	glPushMatrix();
	glColor3d(0, 0, 0);
	glTranslated(-0.6, -4, 0.0);
	glScaled(0.35, 0.35, 0.35);
	glRotated(90, 1, 0, 0);
	gluCylinder(p, 1.1, 1.1, 3, 100, 100);
	glPopMatrix();

	// right leg
	glPushMatrix();
	glColor3d(0, 0, 0);
	glTranslated(0.6, -4, 0.0);
	glScaled(0.35, 0.35, 0.35);
	glRotated(90, 1, 0, 0);
	gluCylinder(p, 1.1, 1.1, 3, 100, 100);
	glPopMatrix();

	//left foot
	glPushMatrix();
	glColor3d(0.447059, 0.164706, 0.164706);
	glTranslated(-0.6, -5.1, 0.0);
	glScaled(0.35, 0.35, 0.35);
	glutSolidSphere(1.1, 100, 100);
	glPopMatrix();

	//right foot
	glPushMatrix();
	glColor3d(0.447059, 0.164706, 0.164706);
	glTranslated(0.6, -5.1, 0.0);
	glScaled(0.35, 0.35, 0.35);
	glutSolidSphere(1.1, 100, 100);
	glPopMatrix();

	glPopMatrix();

}
void normalMinion() {
	GLUquadricObj *p = gluNewQuadric();

	glPushMatrix();
	glTranslated(0, 4, 0);

	// Body
	glPushMatrix();
	glColor3d(1, 1, 0);
	glTranslated(0.0, 0, 0.0);
	glScaled(0.7, 0.7, 0.7);
	gluQuadricDrawStyle(p, GLU_FILL);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(p, 2.0, 2.0, 3, 100, 100);
	glPopMatrix();

	// Bottom
	glPushMatrix();
	glColor3d(0, 0, 1);
	glTranslated(0.0, -1.8, 0.0);
	glScaled(0.7, 0.7, 0.7);
	gluQuadricDrawStyle(p, GLU_FILL);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(p, 1.95, 1.95, 3.3, 100, 100);
	glPopMatrix();

	//head 
	glPushMatrix();
	glColor3d(1, 1, 0);
	glTranslated(0, 0, 0.0);
	glScaled(0.7, 0.7, 0.7);
	glutSolidSphere(2.0, 100, 100);
	glPopMatrix();

	//strap 
	glPushMatrix();
	glColor3d(0.5, 0.5, 0.5);
	glTranslated(0, 0, -0.08);
	glScaled(0.7, 0.7, 0.7);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(p, 2.1, 2.1, 0.6, 100, 100);
	glPopMatrix();

	//mask 
	glPushMatrix();
	glColor3d(0.5, 0.5, 0.5);
	glTranslated(0, -0.1, 1.0);
	glScaled(0.35, 0.35, 0.35);
	gluCylinder(p, 2.5, 2.5, 2, 100, 100);
	glPopMatrix();

	//mask thickness 
	glPushMatrix();
	glColor3d(0.5, 0.5, 0.5);
	glTranslated(0, -0.1, 1.7);
	glScaled(0.35, 0.35, 0.35);
	gluDisk(p, 2.2, 3.0, 100, 100);
	glPopMatrix();

	//Eye white
	glPushMatrix();
	glColor3d(1, 1, 1);
	glTranslated(0, -0.15, 0.7);
	glScaled(0.3, 0.34, 0.3);
	glutSolidSphere(3.0, 100, 100);
	glPopMatrix();

	//Eye colour
	glPushMatrix();
	glColor3d(0.447059, 0.164706, 0.164706);
	glTranslated(0, -0.15, 1.3);
	glScaled(0.3, 0.34, 0.3);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	//Eye ball
	glPushMatrix();
	glColor3d(0, 0, 0);
	glTranslated(0, -0.15, 1.6);
	glScaled(0.3, 0.34, 0.3);
	glutSolidSphere(0.5, 100, 100);
	glPopMatrix();

	//left arm
	glPushMatrix();
	glColor3d(1, 1, 0);
	glTranslated(-1, -1.6, 0.0);
	glRotated(-90, 0, 1, 0);
	glScaled(0.15, 0.15, 0.15);
	gluCylinder(p, 1.5, 1.3, 13, 100, 100);
	glPopMatrix();

	//left hand
	glPushMatrix();
	glColor3d(0.5, 0.5, 0.5);
	glTranslated(-3, -1.6, 0.0);
	glRotated(-90, 0, 1, 0);
	glScaled(0.15, 0.15, 0.15);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();


	//right arm
	glPushMatrix();
	glColor3d(1, 1, 0);
	glTranslated(1, -1.6, 0.0);
	glRotated(90, 0, 1, 0);
	glScaled(0.15, 0.15, 0.15);
	gluCylinder(p, 1.5, 1.3, 12, 100, 100);
	glPopMatrix();

	//right hand
	glPushMatrix();
	glColor3d(0.5, 0.5, 0.5);
	glTranslated(2.8, -1.6, 0.0);
	glRotated(90, 0, 1, 0);
	glScaled(0.15, 0.15, 0.15);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	// left leg
	glPushMatrix();
	glColor3d(0, 0, 1);
	glTranslated(-0.6, -4, 0.0);
	glScaled(0.35, 0.35, 0.35);
	glRotated(90, 1, 0, 0);
	gluCylinder(p, 1.1, 1.1, 3, 100, 100);
	glPopMatrix();

	// right leg
	glPushMatrix();
	glColor3d(0, 0, 1);
	glTranslated(0.6, -4, 0.0);
	glScaled(0.35, 0.35, 0.35);
	glRotated(90, 1, 0, 0);
	gluCylinder(p, 1.1, 1.1, 3, 100, 100);
	glPopMatrix();

	//left foot
	glPushMatrix();
	glColor3d(0.5, 0.5, 0.5);
	glTranslated(-0.6, -5.1, 0.0);
	glScaled(0.35, 0.35, 0.35);
	glutSolidSphere(1.1, 100, 100);
	glPopMatrix();

	//right foot
	glPushMatrix();
	glColor3d(0.5, 0.5, 0.5);
	glTranslated(0.6, -5.1, 0.0);
	glScaled(0.35, 0.35, 0.35);
	glutSolidSphere(1.1, 100, 100);
	glPopMatrix();

	glPopMatrix();

}




void setupLights() {
	GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
	GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat shininess[] = { 50 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	GLfloat lightIntensity[] = { 0.7f, 0.7f, 1, 1.0f };
	GLfloat lightPosition[] = { -7.0f, 6.0f, 3.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
}

void myDisplay(void)
{
	//setupLights();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);

	// Draw Ground 1
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glTranslated(0,0,zground1);
	RenderGround();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//Draw Ground 2
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glTranslated(0,0,zground2);
	RenderGround();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//Draw Ground 3
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glTranslated(0, 0, zground3);
	RenderGround();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//Draw Ground 4
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glTranslated(0, 0, zground4);
	RenderGround();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	// Draw Models
	glPushMatrix();
	glColor3f(1, 1, 0);
	glScaled(0.45, 0.45, 0.45);
	glTranslated(1, 1.4, -20);
	glRotated(180, 0, 1, 0);
	pirateMinion();
	glPopMatrix();

	glPushMatrix();
	glScaled(0.4, 0.4, 0.4);
	glColor3f(1, 1, 1);
	glTranslated(1.65, ynormal, znormal);
	glRotated(180, 0, 1, 0);
	normalMinion();
	glPopMatrix();

	glColor3f(1, 1, 1);

	//sky box
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	GLUquadricObj * qobj;
	qobj = gluNewQuadric();
	glScaled(0.5, 0.5, 0.5);
	glTranslated(50, 0, 0);
	glRotated(90, 1, 0, 1);
	glBindTexture(GL_TEXTURE_2D, tex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 100, 100, 100);
	gluDeleteQuadric(qobj);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
}

//Rotate camera
void camRot(int value) {
	if (stop_rotation == 1)
		return;

	float radius = 24.8;
	cam_angle += 0.02f;
	Eye.x = radius * sin(cam_angle);
	Eye.z = radius * cos(cam_angle);


	glLoadIdentity();	//Clear Model_View Matrix


	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);	//Setup Camera with modified parameters
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glutPostRedisplay();
}
void camRotInverse(int value) {
	if (stop_rotation == 1)
		return;

	float radius = 24.8;
	cam_angle -= 0.02f;
	Eye.x = radius * sin(cam_angle);
	Eye.z = radius * cos(cam_angle);


	glLoadIdentity();	//Clear Model_View Matrix


	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);	//Setup Camera with modified parameters
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glutPostRedisplay();
}

//Animate Ground
void animateGround(int value) {
	if (zground1 > 80)
		zground1 = -79.9;
	if (zground2 > 80)
		zground2 = -79.9;
	if (zground3 > 80)
		zground3 = -79.9;
	if (zground4 > 80)
		zground4 = -79.9;

	zground1 += 0.1;
	zground2 += 0.1;
	zground3 += 0.1;
	zground4 += 0.1;
	glutPostRedisplay();
	glutTimerFunc(0.004 * 1000, animateGround, 0);
}

void animate() {
	camRot(0);
}
//=======================================================================
// Keyboard Function
//=======================================================================
void myKeyboard(unsigned char button, int x, int y)
{
	switch (button)
	{
	case 'x':
		stop_rotation = 1;
		///std::cout << "You Lost\n";
		break;
	case 'c':
		stop_rotation = 0;
		camRot(0);
		break;
	case 'd':
		camRot(0);
		break;
	case 'a':
		camRotInverse(0);
		break;
	case 'u':
		stop_rotation = 1;
		glLoadIdentity();	//Clear Model_View Matrix
		gluLookAt(20, 41, 20, At.x, At.y, At.z, Up.x, Up.y, Up.z);	//Setup Camera with modified parameters
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

//=======================================================================
// Mouse Function
//=======================================================================
void myMouse(int button, int state, int x, int y)
{
	y = HEIGHT - y;

	if (state == GLUT_DOWN)
	{
		cameraZoom = y;
	}
}

//=======================================================================
// Reshape Function
//=======================================================================
void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
}

//=======================================================================
// Assets Loading Function
//=======================================================================
void LoadAssets()
{
	// Loading Model files
	model_house.Load("Models/house/house.3DS");
	model_tree.Load("Models/tree/Tree1.3ds");

	// Loading texture files
	tex_ground.Load("Textures/ground.bmp");
	loadBMP(&tex, "Textures/sky4-jpg.bmp", true);
}

//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(0, 0);

	//glutTimerFunc(0, camRot, 0);
	glutTimerFunc(0.02 * 1000, animateGround, 0);

	glutCreateWindow(title);

	glutDisplayFunc(myDisplay);

	glutKeyboardFunc(myKeyboard);

	//glutMotionFunc(myMotion);

	glutMouseFunc(myMouse);

	glutReshapeFunc(myReshape);

	myInit();

	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}
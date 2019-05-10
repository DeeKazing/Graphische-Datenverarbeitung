#include <GL/glut.h>

//the view changing parameter
const GLfloat ViewAngleOffset = 0.01;
//const GLfloat ViewAngleLROffset=10*ViewAngleOffset;
//const GLfloat ZoomAngleOffset=0.5;
//const GLfloat ZoomRatioOffset=0.5;
const GLfloat ZoomRadiusOffset = 0.05;

//the view init parameter
const GLfloat ViewStartRadius = 5;
const GLfloat ViewStartTheta = 60;
const GLfloat ViewStartPhi = 20;

//const GLfloat ViewStartRatio=1;

//model start state parameter
const GLfloat StartAxisAnglex = 0;
const GLfloat StartAxisAngley = 0;
const GLfloat StartAxisAnglez = 0;

//the view volume init parameter
const GLfloat SIZE = 1000;            // To deal with the view volume
const GLfloat NEAR_Z = 0.001;
const GLfloat FAR_Z = 100.0;

//color advance offset
const GLint MaximumColorNumber = 10;
const GLint ColorAdvanceStep = 10;
const GLint ColorStartIndex = 3;
const GLfloat ColorAdvanceOffset = 0.09;
const GLfloat DefaultAxisLength = 10;

//constant PI
const GLfloat PI = 3.14159265;        // An excessively abused used constant !!

//cockpit relative size
const GLfloat CockpitWidth = 1.5;
const GLfloat CockpitHeight = 1.0;
const GLfloat Front_Rear_Ratio = 0.15;


//skid relative size
const GLfloat SkidSupportHeight = 0.5;
const GLfloat SkidSupportDistance = 2;
const GLfloat SkidWidth = 10;
const GLfloat SkidRadius = 0.1;
const GLfloat SkidJointOffset = 0.8;
const GLfloat BoomLength = 10;
const GLfloat TailRotorSupportRadius = 0.3;
/*const */ GLfloat TailRotorSupportOffset = TailRotorSupportRadius * 0.5;

//utility function for drawing models
void drawOval(GLfloat x, GLfloat y, GLfloat z);
void drawSphere(float fRadius, GLint slices, GLint rings);
void drawCylinder(float fTopR, float fBottomR, float fHeight);
void drawDisk(GLdouble innerRadius, GLdouble outerRadius, GLint slices, GLint rings,
	GLdouble startAngle, GLdouble sweepAngle);
void drawShearCylinder(GLfloat topRad, GLfloat bottomRad, GLfloat height, GLfloat angle);
void drawPatialTorus(GLfloat rCrossSection, GLfloat rAxial, GLfloat sweepAngle);

//small helper functions
GLfloat deg2rad(GLfloat degree);
void setColor();// a little fancy random color generator
void initColorArray();



//model helper functions for each module

void drawSkidSupport(const GLfloat SkidSupportDistance, const GLfloat SkidSupportHeight);


void divider_x(void(*pfunc)(), GLfloat start, GLfloat end, GLint num);
void divider_y(void(*pfunc)(), GLfloat down, GLfloat up, GLint num);
void drawRotor();
void drawHelicopter();

void drawScene();

//setup functions
void init(void);
void showMenu(void);
void showReferenceAxis(void);


//callback functions
void keyboardCallbackProc(unsigned char key, int x, int y);
void specialCallbackProc(int key, int x, int y);
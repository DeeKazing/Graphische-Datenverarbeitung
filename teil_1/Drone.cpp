/****************************************************************************************/
/*
/* Assignment3 : Building a helicopter model.
/* Points to notice:
/*        1.  Implemented helicopter forward/backward motion and both first-person,
			third-person camera are followed with the helicopter motion.
		2.  Yaw,pitch and roll operations are smoothed by using sin/cos functions.
		3.  Yaw,pitch and roll are limited in a limited angle so that no flipping view
			occurs.
		4.  All other functions of previous assignment are preserved except that rotation
			around x/y/z axis are replaced by yaw/pitch/roll. Helicopter are located in
			new position such that rotation axis is in pilot's position in cockpit.
		5.  To implement moving third-person view camera left/right about cockpit of
			helicopter is a little bit tricky because if you don't bookkeep if last operation
			is same kind of operation, you have no way to make viewer convince that you are
			moving left/right. Therefore I used a buffer to store information of last
			operation. If last operation is exactly this kind of operation, I will use
			the very first dat of both "theta and radius". Otherwise refresh these data.
		6.  There is little change in modeling helicopter from previous assignment.
		7.  The usage is exactly like menu in function "showmenu".
				printf("
 Use w/W to toggle between wireframe and shaded model.");
				printf("
 Use o/O to toggle between ortho and perspective views.");
				printf("
 Use a/A to show/hide the reference axis <x = RED, y = GREEN, z = BLUE>.");
				printf("
 Use the f/F,b/B keys to approach and leave model.");
				printf("
 Use arrow up/down keys for camera rotations up/down about origin.");
				printf("
 Use arrow left/right keys for camera rotations left/right about origin.");

				printf("
 Use s/S keys to start/stop spinning of rotor of helicopter");
				printf("
 Use 1 keys to toggle first-person camera view.");
				printf("
 Use 3 keys to toggle third-person camera view.");

				printf("
 Use p/P keys to pitch helicopter and first-person view camera.");

				printf("
 Use y/Y keys to yaw helicopter and first-person view camera.");
				printf("
 Use r/R keys to roll helicopter and first-person view camera.");

				printf("
 Use t/T keys to move helicopter backward/forward along x-axis \
					and first-person and third-person view camera.");

				printf("
 Use h/H keys to move helicopter left/right along z-axis \
					and first-person and third-person view camera.");

				printf("
 Use i/I key to move third-person view camera up/down about \
					cockpit of helicopter
");

				printf("
 Use k/K key to rotate third-person view camera up/down about \
					cockpit of helicopter
");

				printf("
 Use j/J key to move third-person view camera left/right about \
					cockpit of helicopter
");

				printf("
 Use l/L key to rotate third-person view camera left/right about \
					cockpit of helicopter
");

				printf("
 Use 2/8 key to handle vertical elevation (YLP)
");

				printf("
 Use d/D to restore all default states.");
				printf("
 Use m/M to see the menu again.");

				printf("
 Use escape key to exit.
");

/* DATED 31th, oct, 2005
	Qingzhe Huang 5037735    nickhuang99@hotmail.com
/*
/****************************************************************************************/

#include <GL/glut.h>            // This is assuming that you have glut.h set in your include path.
#include <stdio.h>            // Other necessary program related includes.
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "drone.h"

/*
GLdouble xy[4] = {0.0, 0.0, 1.0, 0.0};
GLdouble yz[4] = {1.0, 0.0, 0.0, 0.0};
GLdouble zx[4] = {0.0, 1.0, 0.0, 0.0};
*/
int curColorIndex = ColorStartIndex;
const GLfloat MaxRotatingAngle = 30;
const GLfloat MovementSpeed = 0.3;
GLfloat tailRotorRotatingAngle = 0;
GLfloat mainRotorRotatingAngle = 0;
GLfloat mainRotorSpinSpeed = 0, tailRotorSpinSpeed = 0;
bool bCamera = true;
GLfloat oldLeftTheta = 0, oldLeftRadius = 0, lastLeftTheta = 0, lastLeftRadius = 0,
totalLeftAngleOffset = 0;
GLfloat oldRightTheta = 0, oldRightRadius = 0, lastRightTheta = 0, lastRightRadius = 0,
totalRightAngleOffset = 0;

//float x_Angle = StartAxisAnglex;        // The rotation angles about x, y and z axis.
//float y_Angle = StartAxisAngley;
//float z_Angle = StartAxisAnglez;

GLfloat yaw = 0, pitch = 0, roll = 0;
GLfloat viewPortw, viewPorth;
//GLfloat ratio=ViewStartRatio;

bool bAxis;            // To show or not to show the reference axis.

bool bWire;            // To wireframe/not.

GLfloat fovy = 90.0;        // For Perspective projections.
int H = SIZE, W = SIZE;
GLfloat fViewVol;            // For Orthographic projections.
enum { PERSPECTIVE, ORTHOGRAPHIC } ePrjn = PERSPECTIVE;

GLfloat move_x = 0, move_z = 0, move_y = 0;

//the polar coordinate parameter
GLfloat radius = ViewStartRadius, theta = deg2rad(ViewStartTheta), phi = deg2rad(ViewStartPhi);

GLfloat colorArray[MaximumColorNumber][3];


GLfloat deg2rad(GLfloat degree)
{
	return degree / 360.0 * 2 * PI;
}
//this is just set color from color table initialized at beginning.
void setColor()
{
	curColorIndex++;
	curColorIndex %= MaximumColorNumber;
	glColor3f(colorArray[curColorIndex][0], colorArray[curColorIndex][1],
		colorArray[curColorIndex][2]);
}

/*
void setColor()
{
	GLfloat r,g,b;
	curColorIndex++;
	if (curColorIndex>ColorAdvanceStep)
	{
		r=1;
	}
	else
	{
		r=(curColorIndex%ColorAdvanceStep)*ColorAdvanceOffset;
	}
	if (curColorIndex>2*ColorAdvanceStep)
	{
		g=1;
	}
	else
	{
		g=curColorIndex%(2*ColorAdvanceStep)*ColorAdvanceOffset;
	}
	if (curColorIndex>3*ColorAdvanceStep)
	{
		b=1;
	}
	else
	{
		b=curColorIndex%(ColorAdvanceStep*3)*ColorAdvanceOffset;
	}

	glColor3f(r,g,b);
}
*/

void initColorArray()
{
	GLfloat base[3] = { 0.8,0.5,0.2 };

	for (int i = 0; i < MaximumColorNumber; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			base[j] += 0.1;
			if (base[j] > 1.0)
			{
				base[j] -= 1.0;
			}
			colorArray[i][j] = base[j];
		}
	}
}




/****************************************************************************************/
void showMenu(void)
// Self Explanatory.
{
	printf("



		COMP471 ASSIGNMENT3 HELICOPTER. ");
		printf("
			-------------------------------------------------------------- - ");
			printf("
				Operations: ");
				printf("
					Use w / W to toggle between wireframe and shaded model.");
					printf("
						Use o / O to toggle between ortho and perspective views.");
						printf("
							Use a / A to show / hide the reference axis <x = RED, y = GREEN, z = BLUE>.");
							printf("
								Use the f / F, b / B keys to approach and leave model.");
								printf("
									Use arrow up / down keys for camera rotations up / down about origin.");
									printf("
										Use arrow left / right keys for camera rotations left / right about origin.");

										printf("
											Use s / S keys to start / stop spinning of rotor of helicopter");
											printf("
												Use 1 keys to toggle first - person camera view.");
												printf("
													Use 3 keys to toggle third - person camera view.");

													printf("
														Use p / P keys to pitch helicopter and first - person view camera.");

														printf("
															Use y / Y keys to yaw helicopter and first - person view camera.");
															printf("
																Use r / R keys to roll helicopter and first - person view camera.");

																printf("
																	Use t / T keys to move helicopter backward / forward along x - axis \
																	and first - person and third - person view camera.");

																	printf("
																		Use h / H keys to move helicopter left / right along z - axis \
																		and first - person and third - person view camera.");

																		printf("
																			Use i / I key to move third - person view camera up / down about \
																			cockpit of helicopter
																			");

																			printf("
																				Use k / K key to rotate third - person view camera up / down about \
																				cockpit of helicopter
																				");

																				printf("
																					Use j / J key to move third - person view camera left / right about \
																					cockpit of helicopter
																					");

																					printf("
																						Use l / L key to rotate third - person view camera left / right about \
																						cockpit of helicopter
																						");

																						printf("
																							Use 2 / 8 key to handle vertical elevation(YLP)
																							");

																							printf("
																								Use d / D to restore all default states.");
																								printf("
																									Use m / M to see the menu again.");

																									printf("
																										Use escape key to exit.
																										");

}

/****************************************************************************************/
void init(void)
// Initializes the gl Graphics env and the program variables.
{
	glMatrixMode(GL_PROJECTION);
	// Set the current openGL matrix to GL_PROJECTION ie projection matrix.
	glLoadIdentity();
	// Load identitiy values in the above.
	if (ePrjn == PERSPECTIVE)
	{
		gluPerspective(fovy, (GLfloat)W / (GLfloat)H, NEAR_Z, FAR_Z);
		// This sets the view volume to be a Perspective one.
	}
	else
	{
		fViewVol = 50.0;
		glOrtho(-fViewVol, fViewVol, -fViewVol, fViewVol, NEAR_Z, FAR_Z);
		// This sets the view volume to be a Orthographic one.
	}


	glMatrixMode(GL_MODELVIEW);
	// Change the current matrix mode to Model-View matrix.
	glLoadIdentity();


	glClearColor(0.5, 0.5, 0.5, 0.0);
	// set the background color to black.

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	bAxis = true;

	bWire = false;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	showMenu();

	initColorArray();
}


/****************************************************************************************/
void keyboardCallbackProc(unsigned char key, int x, int y)
// This is the callback procedure for capturing OpenGL Keyboard events.
{
	GLfloat temp;


	switch (key)
	{
	case '1':
		bCamera = false;
		break;
	case '3':
		bCamera = true;
		break;
	case 'd':
	case 'D':
		pitch = yaw = roll = 0;
		move_x = move_z = 0;
		break;
	case 'a':                // Show axes.
	case 'A':
		bAxis = !bAxis;
		break;

	case 'O':                //Change to orthographic projections
		ePrjn = ORTHOGRAPHIC;
		init();
		break;
	case 'o':
		ePrjn = PERSPECTIVE;
		init();
		break;
	case 'C':
	case 'c':                // Clear viewing parameters and return to initial view.
		init();
		break;

	case 'f':
	case 'F':
		if (ePrjn == PERSPECTIVE)
		{
			if (radius > ZoomRadiusOffset)
			{
				radius -= ZoomRadiusOffset;
			}
		}
		break;
	case 'b':
	case 'B':
		if (ePrjn == PERSPECTIVE)
		{
			radius += ZoomRadiusOffset;
		}
		break;

	case 'M':
	case 'm':
		showMenu();
		break;
	case 'w':
	case 'W':
		if (bWire == false)
		{
			bWire = true;    // Wireframe model.
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			bWire = false;    // Solid model.
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		break;
	case 'P':                //Change to perspective projections    
	case 'p':
		pitch += 2;
		break;
	case 'r':
	case 'R':
		roll += 1;
		break;
	case 'y':
	case 'Y':
		yaw += 2;
		break;
	case 't':
		move_x += MovementSpeed;
		break;
	case 'T':
		move_x -= MovementSpeed;
		break;
	case 'h':
		move_z += MovementSpeed;
		break;
	case 'H':
		move_z -= MovementSpeed;
		break;
	case 's':
		mainRotorSpinSpeed = 15;
		tailRotorSpinSpeed = 20;
		break;
	case 'S':
		mainRotorSpinSpeed = 0;
		tailRotorSpinSpeed = 0;
		break;
	case 'i':
		temp = theta;
		theta -= ViewAngleOffset;
		if (theta <= 0)
		{
			theta = temp;
		}
		else
		{
			radius = radius * sin(temp) / sin(theta);
		}
		break;
	case 'I':
		temp = theta;
		theta += ViewAngleOffset;
		if (theta >= PI)
		{
			theta = temp;
		}
		else
		{
			radius = radius * sin(temp) / sin(theta);
		}
		break;
	case 'k':
		temp = theta;
		theta -= ViewAngleOffset;
		if (theta <= 0)
		{
			theta = temp;
		}
		break;
	case 'K':
		temp = theta;
		theta += ViewAngleOffset;
		if (theta >= PI)
		{
			theta = temp;
		}
		break;
	case 'j':
		//this is the first time or last operation is NOT this operation
		if (oldLeftTheta == 0 && oldLeftRadius == 0 || lastLeftTheta != theta || lastLeftRadius != radius)
		{
			oldLeftTheta = theta;
			oldLeftRadius = radius;
			totalLeftAngleOffset = ViewAngleOffset;
		}
		else
		{
			totalLeftAngleOffset += ViewAngleOffset;
		}

		//temp=radius;
		phi -= ViewAngleOffset;
		theta = atan(tan(oldLeftTheta) / cos(totalLeftAngleOffset));
		radius = oldLeftRadius * cos(oldLeftTheta) / cos(theta);
		lastLeftTheta = theta;
		lastLeftRadius = radius;
		break;
	case 'J':
		//this is the first time or last operation is NOT this operation
		if (oldRightTheta == 0 && oldRightRadius == 0 || lastRightTheta != theta || lastRightRadius != radius)
		{
			oldRightTheta = theta;
			oldRightRadius = radius;
			totalRightAngleOffset = ViewAngleOffset;
		}
		else
		{
			//if last time is same operation, we increment angle offset
			totalRightAngleOffset += ViewAngleOffset;
		}

		phi += ViewAngleOffset;
		theta = atan(tan(oldRightTheta) / cos(totalRightAngleOffset));
		radius = oldRightRadius * cos(oldRightTheta) / cos(theta);
		lastRightTheta = theta;
		lastRightRadius = radius;
		break;
	case 'l':
		phi -= ViewAngleOffset;
		break;
	case 'L':
		phi += ViewAngleOffset;

	case '8':
		move_y++;
		break;
	case '2':
		if (move_y > 0)
			move_y--;
		break;


		break;
	case 27:                //ESCAPE Code for exiting program.
		exit(1);
		break;
	}
	glutPostRedisplay();    // Direct the glut system to redisplay the screen.
}

/****************************************************************************************/
// This is the callback procedure for capturing special charater events.
void specialCallbackProc(int key, int x, int y)
{
	GLfloat temp;
	switch (key)
	{
	case GLUT_KEY_LEFT:        // Rotate the model about the y-axis.
		phi -= ViewAngleOffset;
		break;

	case GLUT_KEY_RIGHT:
		phi += ViewAngleOffset;
		break;

	case GLUT_KEY_UP:        // Rotate the model about the x-axis.
		temp = theta;
		theta -= ViewAngleOffset;
		if (theta <= 0)
		{
			theta = temp;
		}
		break;

	case GLUT_KEY_DOWN:
		temp = theta;
		theta += ViewAngleOffset;
		if (theta >= PI)
		{
			theta = temp;
		}
		break;
	}
	glutPostRedisplay();
}


/****************************************************************************************/
void reShapeCallbackProc(int w, int h)
// This is the callback procedure for capturing reShape event for window resizing.
{
	glViewport(0, 0, w, h);
	// Set the viewport to current window size.
	W = w;
	H = h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (ePrjn == PERSPECTIVE)
	{
		gluPerspective(fovy, (GLfloat)W / (GLfloat)H, NEAR_Z, FAR_Z);
	}
	else
	{
		if (w > h)
		{
			W = (fViewVol * w) / h;
			H = fViewVol;
		}
		else
		{
			W = fViewVol;
			H = (fViewVol * h) / w;
		}
		glOrtho(-W, W, -H, H, NEAR_Z, FAR_Z);
	}                    // Change the view volume to maintain the aspect ratio wrt to viewport.

	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}


/****************************************************************************************/
void drawSphere(float fRadius, GLint slices, GLint rings)
// Used to generate a Sphere shape.
{
	GLUquadricObj* pObj;
	pObj = gluNewQuadric();
	// Creates a new quadrics object and returns a pointer to it.

	gluQuadricDrawStyle(pObj, GLU_FILL);

	gluSphere(pObj, fRadius, slices, rings);
	// Draw the sphere with a radius : fRadius.
	gluDeleteQuadric(pObj);
	// Free the Quadric

}

/****************************************************************************************/
// Used to generate a cylinder shape.
void drawCylinder(float fTopR, float fBottomR, float fHeight)
{
	GLUquadricObj* pObj;
	// To keep the original texture intact we need to set the current color to WHITE.

	pObj = gluNewQuadric();
	// Creates a new quadrics object and returns a pointer to it.
	gluQuadricDrawStyle(pObj, GLU_FILL);

	gluCylinder(pObj, fTopR, fBottomR, fHeight, 20, 20);
	// Draw the cylinder with a radius : fRadius.
	gluDeleteQuadric(pObj);
	// Free the Quadric

}

/****************************************************************************************/

// Used to generate a cylinder shape.
void drawDisk(GLdouble innerRadius, GLdouble outerRadius, GLint slices, GLint rings,
	GLdouble startAngle, GLdouble sweepAngle)
{
	GLUquadricObj* pObj;

	pObj = gluNewQuadric();
	// Creates a new quadrics object and returns a pointer to it.
	gluQuadricDrawStyle(pObj, GLU_FILL);

	gluPartialDisk(pObj, innerRadius, outerRadius, slices, rings, startAngle, sweepAngle);
	// Draw the cylinder with a radius : fRadius.
	gluDeleteQuadric(pObj);
	// Free the Quadric
}

/****************************************************************************************/
void showReferenceAxis(void)
// Draw the reference axis
{
	if (bAxis)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		// Pushes attributes like current color to attribute stack.
		glBegin(GL_LINES);
		// X axis red
		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(DefaultAxisLength, 0, 0);
		// Y axis green
		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, DefaultAxisLength, 0);
		// Z axis blue
		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, DefaultAxisLength);
		glEnd();
		glPopAttrib();
		// // Pops attributes like current color from the attribute stack and sets as current.
	}
}

/******************************************************************************/

void drawScene()
{
	int i;
	const GLfloat GroundHeight = -4;
	const GLfloat TrafficSignLength = 1, TrafficSignWidth = 0.5;
	const GLfloat Sign_Y_Offset = 0;
	const GLfloat View_Size = 100;
	const GLfloat BallonSize = 0.5;

	glPushMatrix();

	// YLP 27/04/2013 : add vertical elevation
	glTranslatef(0, -move_y, 0);

	glColor4f(0.0, 0.8, 0.0, 1);
	glLineWidth(2);
	for (i = 0; i < 100; i++)
	{
		glBegin(GL_LINES);
		glVertex3f(-View_Size, GroundHeight,
			-View_Size + i * TrafficSignLength * 2);
		glVertex3f(View_Size, GroundHeight,
			-View_Size + i * TrafficSignLength * 2);

		glVertex3f(-View_Size + i * TrafficSignLength * 2, GroundHeight, -View_Size);
		glVertex3f(-View_Size + i * TrafficSignLength * 2, GroundHeight, View_Size);
		glEnd();
	}

	glColor4f(1, 0, 0.7, 1);
	glPushMatrix();
	glTranslatef(-20, 2, 6);
	for (i = 0; i < 10; i++)
	{
		glTranslatef(TrafficSignLength * 8, 0, 0);

		glScalef(1, 1.3, 1);
		glColor4f(1, 0.9 - i * 0.1, 0.1 + i * 0.1, 1);
		glutSolidSphere(BallonSize, 30, 30);
		glBegin(GL_LINES);
		glVertex3f(0, -BallonSize / 1.3, 0);
		glVertex3f(0, -BallonSize * 2, 0);
		glEnd();

	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-20, 2, -6);
	for (i = 0; i < 10; i++)
	{
		glTranslatef(TrafficSignLength * 8, 0, 0);
		glScalef(1, 1.3, 1);
		glColor4f(1, 0.1 + i * 0.1, 0.9 - i * 0.1, 1);
		glutSolidSphere(BallonSize, 30, 30);
		glBegin(GL_LINES);
		glVertex3f(0, -BallonSize / 1.3, 0);
		glVertex3f(0, -BallonSize * 2, 0);
		glEnd();
	}
	glPopMatrix();
	glTranslatef(-10, 2, 0);
	glColor4f(0.1, 0.6, 0.9, 0.9);
	glScalef(1, 1.3, 1);
	glutSolidSphere(BallonSize, 30, 30);
	glBegin(GL_LINES);
	glVertex3f(0, -BallonSize / 1.3, 0);
	glVertex3f(0, -BallonSize * 2, 0);
	glEnd();

	glPopMatrix();

}

/******************************************************************************/


void drawHalfOval(GLfloat x, GLfloat y, GLfloat z)
{
	GLdouble xz_clip[4] = { 0, 1, 0, 0 };
	glPushMatrix();
	glClipPlane(GL_CLIP_PLANE0, xz_clip);
	glEnable(GL_CLIP_PLANE0);
	glScalef(x, y, z);
	glutSolidSphere(1.0, 20, 20);
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();
}

void divider_x(void(*pfunc)(), GLfloat start, GLfloat end, GLint num)
{
	GLdouble left_clip[4], right_clip[4];
	GLfloat step = (end - start) / num;

	glPushMatrix();
	left_clip[0] = 1;
	left_clip[1] = left_clip[2] = right_clip[1] = right_clip[2] = 0;
	left_clip[3] = -start;
	right_clip[0] = -1;
	right_clip[3] = start + step;

	glEnable(GL_CLIP_PLANE4);
	glEnable(GL_CLIP_PLANE5);
	do
	{
		setColor();
		glClipPlane(GL_CLIP_PLANE4, left_clip);
		glClipPlane(GL_CLIP_PLANE5, right_clip);
		pfunc();
		left_clip[3] += step;
		right_clip[3] += step;
	} while (right_clip[3] < end);
	glDisable(GL_CLIP_PLANE4);
	glDisable(GL_CLIP_PLANE5);
	glPopMatrix();
}

/******************************************************************************/
//this is a little funny function such that it divides an object into several
//partitions along y-axis. This is purely for fun

void divider_y(void(*pfunc)(), GLfloat down, GLfloat up, GLint num)
{
	GLdouble up_clip[4], down_clip[4];
	GLfloat step = (up - down) / num;

	glPushMatrix();
	down_clip[1] = 1;
	down_clip[0] = down_clip[2] = up_clip[0] = up_clip[2] = 0;
	down_clip[3] = -down;
	up_clip[1] = -1;
	up_clip[3] = down + step;

	glEnable(GL_CLIP_PLANE2);
	glEnable(GL_CLIP_PLANE3);
	do
	{
		setColor();
		glClipPlane(GL_CLIP_PLANE2, up_clip);
		glClipPlane(GL_CLIP_PLANE3, down_clip);
		pfunc();
		up_clip[3] += step;
		down_clip[3] += step;
	} while (up_clip[3] < up);
	glDisable(GL_CLIP_PLANE2);
	glDisable(GL_CLIP_PLANE3);
	glPopMatrix();
}

/******************************************************************************/


void drawWindow()
{
	GLdouble front_clip[4] = { -1, 0, 0, CockpitWidth * Front_Rear_Ratio };

	glPushMatrix();

	//setColor();
	glColor4f(0.8, 0.4, 0.6, 0.6);
	glClipPlane(GL_CLIP_PLANE1, front_clip);
	glEnable(GL_CLIP_PLANE1);
	glEnable(GL_BLEND);

	glDepthMask(GL_FALSE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	drawHalfOval(CockpitWidth, CockpitHeight, 1.0);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glDisable(GL_CLIP_PLANE1);

	glPopMatrix();
}

//draw the cockpit of helicopter
void drawCockpit()
{
	void drawWindow();

	GLdouble rear_clip[4] = { 1, 0, 0, -CockpitWidth * Front_Rear_Ratio };

	glPushMatrix();
	glPushMatrix();
	//draw lower part of cockpit
	glRotatef(180, 0, 0, 1);
	setColor();
	drawHalfOval(CockpitWidth, 0.5 * CockpitHeight, 1.0);
	glPopMatrix();
	glPushMatrix();
	//draw window part

	glClipPlane(GL_CLIP_PLANE1, rear_clip);
	glEnable(GL_CLIP_PLANE1);
	//draw upper-rear part of cockpit
	setColor();

	//front_clip[3]=CockpitWidth*0.8;
	drawHalfOval(CockpitWidth, CockpitHeight, 1.0);
	glDisable(GL_CLIP_PLANE1);
	//divider_x(drawWindow, -CockpitWidth, CockpitWidth*Front_Rear_Ratio, 3);
	drawWindow();

	glPopMatrix();


	glPopMatrix();
}

void drawOval(GLfloat x, GLfloat y, GLfloat z)
{
	glPushMatrix();
	//setColor();
	glScalef(x, y, z);
	glutSolidSphere(1.0, 20, 20);
	glPopMatrix();
}

void drawBlade()
{
	const GLfloat BladeWidth = 0.1;
	const GLfloat BladeHeight = 0.08;
	const GLfloat BladeLength = 1.0;//always 1.0
	glPushMatrix();
	//setColor();
	glTranslatef(0, 0, BladeLength);
	drawOval(BladeWidth, BladeHeight, BladeLength);
	glPopMatrix();
}

void drawMainRotor()
{
	const GLfloat MainRotorAxisHeight = 0.4;
	const GLfloat MainRotorAxisRadius = 0.05;

	glPushMatrix();
	setColor();
	glPushMatrix();
	//draw rotor axis
	glRotatef(-90, 1, 0, 0);
	drawCylinder(MainRotorAxisRadius, MainRotorAxisRadius, MainRotorAxisHeight);
	glPopMatrix();
	setColor();
	glTranslatef(0, MainRotorAxisHeight, 0);
	drawRotor();

	glPopMatrix();
}

void drawShearCylinder(GLfloat topRad, GLfloat bottomRad, GLfloat height, GLfloat angle)
{
	//by default the angle is 45
	GLfloat matrix[16] =
	{
		1,0,0,0,
		1,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	//matrix[4]=cos(angle);
	//setColor();
	glPushMatrix();
	glMultMatrixf(matrix);
	glRotatef(-90, 1, 0, 0);
	glScalef(bottomRad, bottomRad, height);
	drawCylinder(topRad, bottomRad, height);

	glPopMatrix();
}

void drawSkidSupport(const GLfloat SkidSupportDistance, const GLfloat SkidSupportHeight)
{

	const GLfloat SkidSupportRadius = 0.3;

	glPushMatrix();

	glPushMatrix();
	glTranslatef(-SkidSupportDistance / 2.0, 0, SkidSupportDistance / 2.0);
	//glRotatef(-90, 1,0,0);
	drawShearCylinder(SkidSupportRadius, SkidSupportRadius, SkidSupportHeight, 45);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(SkidSupportDistance / 2.0, 0, SkidSupportDistance / 2.0);
	drawShearCylinder(SkidSupportRadius, SkidSupportRadius, SkidSupportHeight, 45);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(SkidSupportDistance / 2.0, 0, -SkidSupportDistance / 2.0);
	drawShearCylinder(SkidSupportRadius, SkidSupportRadius, SkidSupportHeight, 45);
	glPopMatrix();
	glTranslatef(-SkidSupportDistance / 2.0, 0, -SkidSupportDistance / 2.0);
	drawShearCylinder(SkidSupportRadius, SkidSupportRadius, SkidSupportHeight, 45);
	glPopMatrix();
}


void drawSkidBottom(const GLfloat SkidRadius, const GLfloat SkidJointOffset,
	const GLfloat SkidSupportDistance)
{
	const GLfloat ratio = 1;
	GLfloat SkidLength = SkidSupportDistance + SkidJointOffset * 2;

	glPushMatrix();
	setColor();
	glRotatef(90, 0, 1, 0);
	glScalef(ratio * 1.7, ratio, 1);
	drawCylinder(SkidRadius / ratio, SkidRadius / ratio, SkidLength);
	glPopMatrix();
}

void drawBoom()
{
	const GLfloat BoomJointRadius = BoomLength / 3;
	const GLfloat BoomTopRadius = BoomLength / 10;
	const GLfloat BoomBaseHeight = 1;
	const GLfloat BoomBaseRadius = BoomLength / 2;
	glPushMatrix();
	setColor();

	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	drawCylinder(BoomJointRadius, BoomTopRadius, BoomLength);
	glPopMatrix();
	setColor();
	glTranslatef(-BoomBaseHeight, 0, 0);
	glRotatef(90, 0, 1, 0);
	drawCylinder(BoomBaseRadius, BoomJointRadius, BoomBaseHeight);
	glPopMatrix();
}

void drawTailRotor()
{
	const GLfloat ratio = 0.3;
	glPushMatrix();
	//setColor();
	glScalef(ratio, ratio * 5, ratio);
	glRotatef(tailRotorRotatingAngle, 0, 1, 0);
	drawMainRotor();

	glPopMatrix();
}
void drawTailBalancer()
{
	glPushMatrix();
	glScalef(4, 4, 0.1);
	drawShearCylinder(TailRotorSupportRadius, TailRotorSupportRadius * 0.5,
		TailRotorSupportRadius, 45);
	glPopMatrix();
}

void drawTailBoom()
{
	//these helper functions will only be used by this function
	void drawBoom();
	void drawTailRotor();
	void drawTailBalancer();

	const GLfloat ratio = 0.2;
	glPushMatrix();
	glPushMatrix();
	glScalef(ratio, ratio, ratio);
	divider_x(drawBoom, 0, BoomLength, 20);
	//drawBoom();
	glPopMatrix();
	glPushMatrix();
	//draw the tail 
	glTranslatef(BoomLength * ratio + TailRotorSupportOffset, 0, 0);
	setColor();
	glutSolidSphere(TailRotorSupportRadius, 20, 20);
	glPopMatrix();

	glPushMatrix();

	glTranslatef(BoomLength * ratio + TailRotorSupportOffset,
		0, TailRotorSupportRadius);
	glScalef(0.8, 0.8, 0.8);
	glRotatef(90, 1, 0, 0);
	drawTailRotor();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(BoomLength * ratio + TailRotorSupportOffset,
		TailRotorSupportRadius - 0.1, 0);
	drawTailBalancer();

	glPopMatrix();
	glTranslatef(BoomLength * ratio + TailRotorSupportOffset,
		-TailRotorSupportRadius + 0.1, 0);
	glRotatef(180, 0, 1, 0);
	glRotatef(180, 0, 0, 1);
	drawTailBalancer();


	glPopMatrix();
}

void drawPatialTorus(GLfloat rCrossSection, GLfloat rAxial, GLfloat sweepAngle)
{
	GLdouble clipPlane0[4] = { 1,0,0,0 };//y=0 plane
	GLdouble clipPlane1[4] = { tan(sweepAngle), 1, 0, 0 }; //y=x*tan(angle) plane

	glPushMatrix();
	//setColor();
	glRotatef(180, 0, 0, 1);
	glTranslatef(0, -rAxial, 0);
	glClipPlane(GL_CLIP_PLANE0, clipPlane0);
	glClipPlane(GL_CLIP_PLANE1, clipPlane1);
	glEnable(GL_CLIP_PLANE0);
	glEnable(GL_CLIP_PLANE1);
	glutSolidTorus(rCrossSection, rAxial, 40, 40);
	glDisable(GL_CLIP_PLANE0);
	glDisable(GL_CLIP_PLANE1);
	glPopMatrix();
}

void drawSkid()
{
	//helper function only be called inside this
	void drawSkidBottom(const GLfloat SkidRadius, const GLfloat SkidJointOffset,
		const GLfloat SkidSupportDistance);

	glPushMatrix();

	drawSkidSupport(SkidSupportDistance, SkidSupportHeight);
	glPushMatrix();
	glTranslatef(-SkidSupportDistance / 2.0 - SkidJointOffset,
		-SkidRadius, SkidSupportDistance / 2.0);
	drawSkidBottom(SkidRadius, SkidJointOffset, SkidSupportDistance);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-SkidSupportDistance / 2.0 - SkidJointOffset,
		-SkidRadius, -SkidSupportDistance / 2.0);
	drawSkidBottom(SkidRadius, SkidJointOffset, SkidSupportDistance);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-SkidSupportDistance + 0.2, -SkidRadius, -SkidSupportDistance / 2.0);
	drawPatialTorus(SkidRadius + 0.02, 2.3, 40);
	glPopMatrix();
	glTranslatef(-SkidSupportDistance + 0.2, -SkidRadius, SkidSupportDistance / 2.0);
	drawPatialTorus(SkidRadius + 0.02, 2.3, 40);
	glPopMatrix();

}


void drawRotor()
{
	void drawBlade();

	glPushMatrix();
	drawBlade();
	glRotatef(120, 0, 1, 0);
	drawBlade();
	glRotatef(120, 0, 1, 0);
	drawBlade();
	glPopMatrix();
}


//I don't want other application accidentally have a name conflict
//with my helper function, so I declare those function inside its calling
//function
void drawHelicopter()
{
	//the major module of helicopter
	void drawCockpit();
	void drawTailBoom();
	void drawMainRotor();
	void drawSkid();

	curColorIndex = ColorStartIndex;
	glPushMatrix();


	glPushMatrix();
	//glRotatef(180, 0, 1, 0);
	drawCockpit();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(CockpitWidth - 0.08, CockpitHeight / 5, 0);
	//glRotatef(180, 0, 1, 0);
	glScalef(0.8, 0.4, 0.4);
	drawTailBoom();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, CockpitHeight, 0);
	glRotatef(mainRotorRotatingAngle, 0, 1, 0);
	drawMainRotor();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, -SkidSupportHeight, 0);
	glScalef(0.6, 1, 0.5);
	drawSkid();
	glPopMatrix();

	glPopMatrix();

}


/****************************************************************************************/
void displayCallbackProc(void)
// This is the callback procedure for capturing OpenGL Display events.
// All the 'happening' things happen here :)
{
	GLfloat eye_x, eye_y, eye_z;
	const GLfloat X_Offset = 1.5;
	const GLfloat Y_Offset = -1;
	const GLfloat EyeSightLength = 5;
	//GLfloat eye_x1, eye_y1, eye_z1;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	mainRotorRotatingAngle += mainRotorSpinSpeed;
	tailRotorRotatingAngle += tailRotorSpinSpeed;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//glTranslatef(0, 0, -20.0);    // Translate the object by 20 units in -ve z-direction.
	//glScalef(4, 4, 4);


/************************************************************
The view transformation
*************************************************************/
//theta_rad=deg2rad(theta);
//phi_rad=deg2rad(phi);
	eye_x = radius * sin(theta) * sin(phi);
	eye_y = radius * cos(theta);
	eye_z = radius * sin(theta) * cos(phi);

	//glPushMatrix();

	if (bCamera)
	{
		gluLookAt(eye_x + move_x, eye_y, eye_z + move_z, move_x, 0, move_z, 0, 1, 0);
		glTranslatef(-eye_x, -eye_y, -eye_z);
		showReferenceAxis();
		drawScene();
		//translate helicopter a little back so that pilot is at rotation axis
		glTranslatef(X_Offset + move_x, Y_Offset, move_z);

		glScalef(2, 2, 2);

		//    glRotatef(x_Angle, 1.0, 0.0, 0.0);    // Rotate the object by x_Angle about x-axis
		//    glRotatef(y_Angle, 0.0, 1.0, 0.0);    // Rotate the object by y_Angle about y-axis
		//    glRotatef(z_Angle, 0.0, 0.0, 1.0);    // Rotate the object by z_Angle about z-axis

		glRotatef(sin(deg2rad(yaw)) * MaxRotatingAngle, 0, 1, 0);
		glRotatef(sin(deg2rad(pitch)) * MaxRotatingAngle / 2, 0, 0, 1);
		glRotatef(sin(deg2rad(roll)) * MaxRotatingAngle, 1, 0, 0);
		drawHelicopter();

		glTranslatef(eye_x, eye_y, eye_z);

	}
	else
	{
		gluLookAt(move_x, 0, move_z, -1 + move_x, -sin(deg2rad(pitch)), move_z - sin(deg2rad(yaw)),
			sin(sin(deg2rad(pitch)) * PI / 6), cos(sin(deg2rad(pitch)) * PI / 6),
			sin(sin(deg2rad(roll))) * PI / 6);
		//glTranslatef(-move_x, -eye_y, -eye_z);
		showReferenceAxis();
		drawScene();
		glTranslatef(X_Offset + move_x, Y_Offset, move_z);

		glScalef(2, 2, 2);

		//    glRotatef(x_Angle, 1.0, 0.0, 0.0);    // Rotate the object by x_Angle about x-axis
		//    glRotatef(y_Angle, 0.0, 1.0, 0.0);    // Rotate the object by y_Angle about y-axis
		//    glRotatef(z_Angle, 0.0, 0.0, 1.0);    // Rotate the object by z_Angle about z-axis

		glRotatef(sin(deg2rad(yaw)) * MaxRotatingAngle, 0, 1, 0);
		glRotatef(sin(deg2rad(pitch)) * MaxRotatingAngle / 2, 0, 0, 1);
		glRotatef(sin(deg2rad(roll)) * MaxRotatingAngle, 1, 0, 0);
		drawHelicopter();
	}


	//the following sequence is HIGHLY important because
	//you want the object to rotate w.r.t. to original world 
	//origin and you need to transform it between world coordinate and
	//view-coordinate




	glutSwapBuffers();    // Use of double buffering to avoid flicker.
	glutPostRedisplay();
}

/****************************************************************************************/
int main(int argc, char* argv[])
// The main program.
{

	/* All customary glut env initializations. */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(SIZE, SIZE);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("A primitive model of a helicopter ! ~nick");

	/* glut env initializations done. */


	init();                // Initialize the openGL env. variables and the application global variables.


	/* Callback registrations with the OpenGL env are done here */

	glutDisplayFunc(displayCallbackProc);
	glutKeyboardFunc(keyboardCallbackProc);
	glutSpecialFunc(specialCallbackProc);
	glutReshapeFunc(reShapeCallbackProc);

	/* Callback registrations done.*/

	glutMainLoop();
	// Inside glutMainLoop(); all the mouse/KB events pertaining to the 
	// application window are dispatched. This loop is never exited so the  
	// statements after glutMainLoop(); are never executed !
	return 1;
}
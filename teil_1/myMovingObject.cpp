#include <iostream> 
#include <GL/freeglut.h>
#include <GL/SOIL.h>
#include <iostream>
#include <string>

std::string paths = "";
float RotorRotation = 0;
GLUquadric* quad;
GLuint groundTexture;
float eyeY = 1.0;
float camereEyeRot = 0;
GLfloat myx = 0.0, myy = 0.0, myz = 0.0, heliRotationTEST = 0.0;
void helirotation(float*);
// source(eyeX, eyeY, eyeZ), target(centerX, centerY, centerZ), up(upX, upY, upZ)
//viewer
int forward_tilt = 0, left_tilt = 0, right_tilt = 0;
void moveHeli();

void init() {
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	quad = gluNewQuadric();
	groundTexture = SOIL_load_OGL_texture("boden.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS
		| SOIL_FLAG_INVERT_Y
		| SOIL_FLAG_NTSC_SAFE_RGB
		| SOIL_FLAG_COMPRESS_TO_DXT);
	glBindTexture(GL_TEXTURE_2D, groundTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void RenderScene() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLineWidth(4.0);
	// source(eyeX, eyeY, eyeZ), target(centerX, centerY, centerZ), up(upX, upY, upZ)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, 1.0, 1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//myx
	gluLookAt(0, 3, -1, myx, myy, myz, 0.0, 10.0, 0.0);
	glutWireCube(0.2);
	glRotatef(camereEyeRot, 0.0, 1.0, 0.0);

	//BODEN
	glPushMatrix();
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_POLYGON); //GL_QUADS
	glTexCoord2f(10, 0);
	glVertex3f(-5, -1, -5.);
	glTexCoord2f(10, 10);
	glVertex3f(5, -1, -5.);
	glTexCoord2f(0.0, 10);
	glVertex3f(5, -1, 5.);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-5, -1, 5.);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//BODEN-ENDE
	
	//HAUS ODER PILZ JE NACHDEM 4*
	glPushMatrix();
	glTranslatef(3.5, -0.5, 0);
	glColor3f(1, 1, 1);
	glutSolidCube(0.5);
	glColor3f(1, 0, 0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glTranslatef(0,0,0.2);
	gluCylinder(gluNewQuadric(), 0.5, 0.0, 0.5, 32, 32);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3.5, -0.5, 0);
	glColor3f(1, 1, 1);
	glutSolidCube(0.5);
	glColor3f(1, 0, 0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glTranslatef(0, 0, 0.2);
	gluCylinder(gluNewQuadric(), 0.5, 0.0, 0.5, 32, 32);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.5,-3.5 );
	glColor3f(1, 1, 1);
	glutSolidCube(0.5);
	glColor3f(1, 0, 0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glTranslatef(0, 0, 0.2);
	gluCylinder(gluNewQuadric(), 0.5, 0.0, 0.5, 32, 32);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1, -0.5, 3.5);
	glColor3f(1, 1, 1);
	glutSolidCube(0.5);
	glColor3f(1, 0, 0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glTranslatef(0, 0, 0.2);
	gluCylinder(gluNewQuadric(), 0.5, 0.0, 0.5, 32, 32);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1, -0.5, 3.5);
	glColor3f(1, 1, 1);
	glutSolidCube(0.5);
	glColor3f(1, 0, 0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glTranslatef(0, 0, 0.2);
	gluCylinder(gluNewQuadric(), 0.5, 0.0, 0.5, 32, 32);
	glPopMatrix();

	//HAUS ODER PILZ JE NACHDEM


	//Heli-Bewegen
	glTranslatef(myx, myy, myz);
	glRotatef(heliRotationTEST, 0.0, 1.0, 0.0);
	moveHeli();
	//Heli-Bewegen-ENDE

	//HELI KÖRPER
	glPushMatrix();
	glColor3f(0, 0, 1);
	glTranslatef(0, -0.08f, 0.15f);
	glScalef(2, 1, 2);
	glutSolidSphere(0.1, 16, 16);
	glPopMatrix();
	//HELIKÖRPER ENDE

	//HELIKÖRPER ZYLINDER
	glPushMatrix();
	glColor3f(0.7f, 0.7f, 0.7f);
	glTranslatef(0, -0.08f, -0.25f);
	glScalef(2, 1, 2.2f);
	glutSolidCylinder(0.101, 0.2, 16, 16);
	glPopMatrix();
	//HELIKÖRPER ZYLINDER ENDE


	//HELI BEINE
	glPushMatrix();
	glColor4f(0, 0, 0, 0);
	glEnable(GL_LINE_SMOOTH);
	glBegin(GL_LINES);

	glVertex3f(0.2f, -0.28f, 0.28f);
	glVertex3f(0.2f, -0.28f, -0.28f);

	glVertex3f(0.2f, -0.28f, 0.28f);
	glVertex3f(0.2f, -0.24f, 0.32f);

	glVertex3f(0.2f, -0.28f, -0.28f);
	glVertex3f(0.2f, -0.24f, -0.32f);

	glVertex3f(0, 0.0f, 0.2f);
	glVertex3f(0.2f, -0.28f, 0.2f);

	glVertex3f(0, 0.0, -0.2f);
	glVertex3f(0.2f, -0.28f, -0.2f);

	glVertex3f(-0.2f, -0.28f, 0.28f);
	glVertex3f(-0.2f, -0.28f, -0.28f);

	glVertex3f(-0.2f, -0.28f, 0.28f);
	glVertex3f(-0.2f, -0.24f, 0.32f);

	glVertex3f(-0.2f, -0.28f, -0.28f);
	glVertex3f(-0.2f, -0.24f, -0.32f);

	glVertex3f(0, 0.0, 0.2f);
	glVertex3f(-0.2f, -0.28f, 0.2f);

	glVertex3f(0, 0.0, -0.2f);
	glVertex3f(-0.2f, -0.28f, -0.2f);

	glEnd();
	glPopMatrix();
	//HELI BEINE

	//ROTOR STANGE
	glPushMatrix();
	glColor3f(0.2f, 0.2f, 0.2f);
	glTranslatef(0.0f, 0.15f, 0.0f);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glutSolidCylinder(0.01, 0.2, 16, 16);
	glPopMatrix();
	//ROTOR STANGE ENDE

	//ROTOR BLÄTTER
	glTranslatef(0.0f, 0.13f, 0.0f);
	glRotatef(RotorRotation, 0.0, 1.0, 0.0);

	glPushMatrix();
	glColor3f(0, 0, 0);
	glScalef(2, 0.01f, 0.05f);
	glutSolidCube(0.4);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glScalef(2, 0.01f, 0.05f);
	glutSolidCube(0.4);
	glPopMatrix();
	//ROTOR BLÄTTER ENDE

	glutSwapBuffers();
	glFlush();

}

void Reshape(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	// left, right, bottom, top, near, far
	glOrtho(-1.0, 1.0, -1.0, 1.0, 0.0, 10.0);

	// fovY, aspect, zNear, zFar
	// gluPerspective(45.0, 1.0, 0.1, 2.0);
	glMatrixMode(GL_MODELVIEW);
}

void keyPressed(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
		myz = myz + 0.1f;
		std::cout << myz << std::endl;
		break;
	case 's':
		myz = myz - 0.1f;
		std::cout << myz << std::endl;
		break;
	case 'a':
		myx = myx + 0.1f;
		std::cout << myx << std::endl;
		break;
	case 'd':
		myx = myx - 0.1f;
		std::cout << myx << std::endl;
		break;

		/*case 'q':
			camereEyeRot += 5;
			break;
		case 'e':
			camereEyeRot -= 5;
			break;
	*/
	}
}


void rausfliegen() {
	if (myz <= 4.5) {
		myz += 0.1f;
		return;
	}
	paths = "0";
	helirotation(&heliRotationTEST);
}


void moveHeli() {
	if (paths == "") {
		rausfliegen();
	}

	if (paths == "0") {
		if (myx <= 4.5) {
			myx += 0.1f;
		}
		else {
			paths = "1";
			helirotation(&heliRotationTEST);
		}
	}
	if (paths == "1") {
		if (myz >= -4.5) {
			myz -= 0.1f;
		}
		else {
			paths = "2";
			helirotation(&heliRotationTEST);
		}
	}
	if (paths == "2") {
		if (myx >= -4.5) {
			myx -= 0.1f;
		}
		else {
			paths = "3";
			helirotation(&heliRotationTEST);
		}
	}
	if (paths == "3") {
		if (myz <= 4.5) {
			myz += 0.1f;
		}
		else {
			paths = "0";
			helirotation(&heliRotationTEST);
		}
	}
}

void rotation(float* f, float by) {
	*f += by;
	if (*f > 360.0f) {
		*f = 0.0f;
	}
}

void helirotation(float* f) {
	if (paths == "0")
		* f = 90;
	if (paths == "1")
		* f = 180;
	if (paths == "2")
		* f = 270;
	if (paths == "3")
		* f = 360;
}

void animate(int value) {
	rotation(&RotorRotation, 20);
	// RenderScene aufrufen
	glutPostRedisplay();
	// Timer wieder registrieren - Animate wird so nach 10 msec mit value+=1 aufgerufen.
	int wait_msec = 10;
	glutTimerFunc(wait_msec, animate, ++value);
}


int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Feridun; Timo");   // Fenster-Erzeugung
	glutKeyboardFunc(keyPressed);
	glutDisplayFunc(RenderScene);         // Zeichenfunktion bekannt machen
	glutReshapeFunc(Reshape);
	// TimerCallback registrieren; wird nach 10 msec aufgerufen mit Parameter 0  
	glutTimerFunc(10, animate, 0);
	init();
	glutMainLoop();
	return 0;
}
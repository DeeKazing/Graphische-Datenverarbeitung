#include <iostream> 
#include <GL/freeglut.h>
#include <GL/SOIL.h>

float biggestRotationAngle = 0;
float bigRotationAngle = 0;
float rotationAngle = 0;
float textureRepeats = 7;
GLUquadric* quad;
GLuint groundTexture;
float eyeY = 1.0;

void init() {
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	float lightPos[] = { 5.0, 5.0, 5.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_COLOR_MATERIAL);

	quad = gluNewQuadric();
	groundTexture = SOIL_load_OGL_texture("boden.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	glBindTexture(GL_TEXTURE_2D, groundTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void renderScene() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// source(eyeX, eyeY, eyeZ), target(centerX, centerY, centerZ), up(upX, upY, upZ)
	gluLookAt(1.0, eyeY, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//glColor3f(0.0, 0.0, 0.0);

	glRotatef(biggestRotationAngle, 0.0, 1.0, 0.0);

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	//glColor3f(1.0, 1.0, 1.0);
	glTexCoord2f(textureRepeats, 0);
	glVertex3f(-3.0, -2.0, -3.0);
	glTexCoord2f(textureRepeats, textureRepeats);
	glVertex3f(3.0, -2.0, -3.0);
	glTexCoord2f(0.0, textureRepeats);
	glVertex3f(3.0, -2.0, 3.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-3.0, -2.0, 3.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glRotatef(bigRotationAngle, 0.0, 1.0, 0.0);
	glTranslatef(0.7, 0.5, 0.0);

	glPushMatrix();
	glColor3f(0.9, 0.9, 0.9);
	glTranslatef(0.0, -0.13, 0.0);
	glScalef(1.3, 1.0, 1.0);
	glutSolidSphere(0.05, 32, 32);
	//gluSphere(quad, 0.05, 32, 32);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5, 0.5, 0.5);
	glTranslatef(0.0, 0.12, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glutSolidCylinder(0.01, 0.3, 32.0, 32.0);
	glPopMatrix();

	glTranslatef(0.0, 0.1, 0.0);
	glRotatef(rotationAngle, 0.0, 1.0, 0.0);

	glPushMatrix();
	glColor3f(0.9, 0.9, 0.0);
	glScalef(1.3, 0.03, 0.01);
	glutSolidCube(0.4);
	glPopMatrix();
	glPushMatrix();
	glColor3f(0.7, 0.7, 0.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glScalef(1.3, 0.07, 0.01);
	glutSolidCube(0.4);
	glPopMatrix();


	glutSwapBuffers();
}

void reshape(int width, int height) {
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
		eyeY += 0.3;
		break;
	case 's':
		eyeY -= 0.3;
		break;
	}
}

void incrementAngle(float* f, float by) {
	*f += by;
	if (*f > 360.0f) {
		*f = 0.0f;
	}
}

void animate(int value) {
	incrementAngle(&biggestRotationAngle, -0.05);
	incrementAngle(&bigRotationAngle, 0.8);
	incrementAngle(&rotationAngle, 25);
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
	glutCreateWindow("Name_1; Selim");
	glutKeyboardFunc(keyPressed);
	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshape);
	// TimerCallback registrieren; wird nach 10 msec aufgerufen mit Parameter 0  
	glutTimerFunc(10, animate, 0);
	init();
	glutMainLoop();
	return 0;
}
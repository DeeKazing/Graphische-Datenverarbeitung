// GD-Praktikum:   teil_1.cpp  (Teil 1: Start-Programm)
// Hergenroether / Groch    Last Update: 05.07.2014

#include <iostream> 
#include <GL/freeglut.h>         //lädt alles für OpenGL
#include "Wuerfel.h"

void Init()
{
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	// Hier finden jene Aktionen statt, die zum Programmstart einmalig 
	// durchgeführt werden müssen
}

void RenderScene() //Zeichenfunktion
{
	glLoadIdentity();   // Aktuelle Model-/View-Transformations-Matrix zuruecksetzen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Hier befindet sich der Code der in jedem Frame ausgefuehrt werden muss
	//Oranger Hintergrund
	glClearColor(1.2, 0.6, 0.0, 1.0);

	gluLookAt(0., 0., 1., 0., 0., 0., 0., 1., 0.);
	//gluLookAt(Kammeraposition[eyex, eyey, eyez],Blickpunkt [centerx, centery, centerz], Normale der Kamera[upx, upy, upz];
	//glTranslatef(0., 0., -1.);
	//glRotatef(angle, x ,y, z)
	glRotatef(45., 0., 0., -1.); //-1 wegen den farben wie auf abbildung 4
	glPushMatrix();	//Matrix wird auf dem Stack gesichert
	glTranslatef(0.1, 0, 0);//multipliziert die aktuelle Matrix mit einer verschiebungsmatrix
	glScalef(0.5, 0.25, 0.25);//bewirkt eine Verzerrung oder Spiegelung des koordinatensystems
	Wuerfel(0.4);
	glPopMatrix();	//Matrix wird vom Stack geholt und gesetzt

	glPushMatrix();
	glTranslatef(0.3, 0, 0);
	glScalef(0.5, 0.125, 0.125);
	Wuerfel(0.4);
	glPopMatrix();

	glFlush();//BUFFER LEEREN

	glutSwapBuffers();
}

void Reshape(int width, int height)
{
	// Matrix für Transformation: Frustum->viewport
	glMatrixMode(GL_PROJECTION);
	// Aktuelle Transformations-Matrix zuruecksetzen
	glLoadIdentity();
	// Viewport definieren
	glViewport(0, 0, width, height);
	// Frustum definieren (siehe unten)
	/*Die near und far Werte beschreiben den Abstand der Near- und Far-
	Clippingplane vom aktuellen Standpunkt der Kamera in Blickrichtung der
	Kamera. Werden für near und far negative Werte gesetzt, dann liegt das
	aufgespannte Frustum hinter der Kamera!*/

	//glOrtho(left, right, bottom, top, near, far );
	//glOrtho(-1., 1., -1., 1., 0.0, 3.0);

	// gluPerspective(senkr. Oeffnungsw., Seitenverh., zNear, zFar);
	gluPerspective(45., 1., 0.1, 2.0);

	// Matrix für Modellierung/Viewing
	glMatrixMode(GL_MODELVIEW);
	// Hier finden die Reaktionen auf eine Veränderung der Größe des 
	// Graphikfensters statt
}

void Animate(int value)
{
	// Hier werden Berechnungen durchgeführt, die zu einer Animation der Szene  
	// erforderlich sind. Dieser Prozess läuft im Hintergrund und wird alle 
	// 1000 msec aufgerufen. Der Parameter "value" wird einfach nur um eins 
	// inkrementiert und dem Callback wieder uebergeben. 
	std::cout << "value=" << value << std::endl;
	// RenderScene aufrufen
	glutPostRedisplay();
	// Timer wieder registrieren - Animate wird so nach 10 msec mit value+=1 aufgerufen.
	int wait_msec = 10;
	glutTimerFunc(wait_msec, Animate, ++value);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);                // GLUT initialisieren
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);         // Fenster-Konfiguration
	glutCreateWindow("Feridun; Timo");   // Fenster-Erzeugung
	glutDisplayFunc(RenderScene);         // Zeichenfunktion bekannt machen
	glutReshapeFunc(Reshape);
	// TimerCallback registrieren; wird nach 10 msec aufgerufen mit Parameter 0  
	glutTimerFunc(10, Animate, 0);
	Init();
	glutMainLoop();
	return 0;
}

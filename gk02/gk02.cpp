#include "stdafx.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
// Definiujemy stał o wartoci liczby PI
#define GL_PI 3.1415f
// Wielko obrotów
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
// Wywoływana w celu przerysowania sceny
void RenderScene(void)
{
	GLfloat x, y, z, angle; // Zmienne przechowujące współrzędne i kąty
	GLfloat sizes[2]; // Do zapisania dopuszczalnego zakresu wielkości
	GLfloat step; // Zapamiętuje obsługiwany przyrost rozmiaru
	GLfloat curSize; // Zapamętuje aktualny rozmiar

					 // Wyczyszczenie okna aktualnym kolorem czyszczącym
	glClear(GL_COLOR_BUFFER_BIT);
	// Zapisanie stanu macierzy i wykonanie obrotu
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	glGetFloatv(GL_POINT_SIZE_RANGE, sizes);
	glGetFloatv(GL_POINT_SIZE_GRANULARITY, &step);
	// Wywoła tylko raz, przed rysowaniem wszystkich punktów
	curSize = sizes[0];

	z = -200.0f;
	for (angle = 0.0f; angle <= (2.0f*GL_PI)*8.0f; angle += 0.1f)
	{
		glPointSize(curSize);
		glBegin(GL_POINTS);
		x = 50.0f*sin(angle);
		y = 50.0f*cos(angle);
		// Określenie punktu i przesunięcie współrzędnej Z
		glVertex3f(x, y, z);
		z += 0.4f;
		curSize += step;
		glEnd();
	}
	// Zakończenie rysowania punktów

	// Odtworzenie macierzy przekształce
	glPopMatrix();
	// Wykonanie poleceń rysowania
	glutSwapBuffers();
}
// Ta funkcja wykonuje wszystkie konieczne inicjalizacje kontekstu renderowania
void SetupRC()
{
	// Czarne tło
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Będziemy rysować kolorem zielonym
	glColor3f(1.0f, 0.6f, 0.0f);
}
void SpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
		xRot -= 5.0f;
	if (key == GLUT_KEY_DOWN)
		xRot += 5.0f;
	if (key == GLUT_KEY_LEFT)
		yRot -= 5.0f;
	if (key == GLUT_KEY_RIGHT)
		yRot += 5.0f;
	if (key > 356.0f)
		xRot = 0.0f;
	if (key < -1.0f)
		xRot = 355.0f;
	if (key > 356.0f)
		yRot = 0.0f;
	if (key < -1.0f)
		yRot = 355.0f;
	// Odświeżenie zawartości okna
	glutPostRedisplay();
}
void ChangeSize(int w, int h)
{
	GLfloat nRange = 100.0f;
	// Zabezpieczenie przed dzieleniem przez zero
	if (h == 0)
		h = 1;
	// Ustalenie wymiarów widoku na zgodnych z wymiarami okna
	glViewport(0, 0, w, h);
	// Ponowne ustawienie stosu macierzy rzutowania
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Utworzenie przestrzeni ograniczającej (lewo, prawo, dół, góra, blisko, daleko)
	if (w <= h)
		glOrtho(-nRange, nRange, -nRange * h / w, nRange*h / w, -nRange, nRange);
	else
		glOrtho(-nRange * w / h, nRange*w / h, -nRange, nRange, -nRange, nRange);
	// Ponowne ustawienie stosu macierzy rzutowania
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Points Example");
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);
	glutDisplayFunc(RenderScene);
	SetupRC();
	glutMainLoop();
	return 0;
}
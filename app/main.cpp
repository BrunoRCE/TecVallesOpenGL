#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define GL_PI 3.1415f
#define PI 3.14159265f
#define DEF_dollyStepSize 1.0
#define DEF_angleSensitivity 1.0

float LightPos0[] = {0.75f, 1.0f, 1.0f, 0.0f};
float LightAmb0[] = {0.75f, 0.75f, 0.75f, 1.0f};
float LightDif0[] = {1.0f, 1.0f, 1.0f, 1.0f};
float LightSpc0[] = {1.0f, 1.0f, 1.0f, 1.0f};

GLfloat camX = 0;
GLfloat camY = 0;
GLfloat camZ = -80.0;
GLfloat camYaw = 0;
GLfloat camPitch = 0.0;

int isInMouseDrag = false;
int mouseIsInverted = true;
int viewPortCenterX = -1;
int viewPortCenterY = -1;
int oldCursorID = 0;
int oldCursorX = 0;
int oldCursorY = 0;

float cuadricep = 0, cuadricep2 = 0, c1 = 0, c2 = 0, camina = 100;

char truco = ' ';

float avanza1 = 150.0, avanza2 = 65, w = 0, t = 0.5;

int refreshMillis = 10;

void Tiempo(int value)
{
	glutPostRedisplay();
	glutTimerFunc(refreshMillis, Tiempo, 0);
}

void enterMouseDrag(int x, int y)
{
	if (isInMouseDrag)
		return;
	isInMouseDrag = true;
	if (viewPortCenterX < 0)
	{
		viewPortCenterX = glutGet(GLUT_WINDOW_WIDTH) / 2;
		viewPortCenterY = glutGet(GLUT_WINDOW_HEIGHT) / 2;
	}
	oldCursorID = glutGet(GLUT_WINDOW_CURSOR);
	oldCursorX = x;
	oldCursorY = y;
	glutSetCursor(GLUT_CURSOR_NONE);
	glutWarpPointer(viewPortCenterX, viewPortCenterY);
}

void exitMouseDrag(int x, int y)
{
	if (!isInMouseDrag)
		return;
	isInMouseDrag = false;
	glutSetCursor(oldCursorID);
	glutWarpPointer(oldCursorX, oldCursorY);
}

void clampCamera()
{
	if (camPitch > 90.0)
		camPitch = 90.0;
	else if (camPitch < -90.0)
		camPitch = -90.0;
	while (camYaw < 0.0)
		camYaw += 360.0;
	while (camYaw >= 360.0)
		camYaw -= 360.0;
}

void dollyCamera(GLfloat dollyBy, GLfloat dollyAngle)
{
	GLfloat actualAngleInRadians;
	actualAngleInRadians = ((camYaw + dollyAngle) * M_PI / 180.0);
	camX -= sin(actualAngleInRadians) * dollyBy * DEF_dollyStepSize;
	camZ -= cos(actualAngleInRadians) * dollyBy * DEF_dollyStepSize;
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		// Movimiento hacia adelante
	case 's':
	case 'S':
		dollyCamera(DEF_dollyStepSize, 0.0);
		break;

		// Movimiento hacia atras
	case 'w':
	case 'W':
		dollyCamera(DEF_dollyStepSize, 180.0);
		break;

		// Strafe hacia la izquierda
	case 'd':
	case 'D':
		dollyCamera(DEF_dollyStepSize, 90.0);
		break;

		// Strafe derecha
	case 'a':
	case 'A':
		dollyCamera(DEF_dollyStepSize, 270.0);
		break;

		// Toggle 'inverted' mouse.
	case 'i':
	case 'I':
		mouseIsInverted = !mouseIsInverted;
		break;

	case 'x':

		truco = 'x';

		break;

		//Esc salir
	case 27:
		exitMouseDrag(0, 0);
		exit(0);
		break;
	}
}

void specialFunc(int key, int x, int y)
{
	switch (key)
	{
	//giro  a la izquierda
	case GLUT_KEY_LEFT:
		camYaw += 1.0;
		clampCamera();
		break;

	// giro a la derecha.
	case GLUT_KEY_RIGHT:
		camYaw -= 1.0;
		clampCamera();
		break;

	// mirar hacia arriba.
	case GLUT_KEY_UP:
		camPitch += 1.0;
		clampCamera();
		break;

	// mirar hacia abajo
	case GLUT_KEY_DOWN:
		camPitch -= 1.0;
		clampCamera();
		break;
	}
}

void mouseFunc(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (!isInMouseDrag)
			enterMouseDrag(x, y);
		else
			exitMouseDrag(x, y);
	}
}

void allMotionFunc(int x, int y)
{
	int deltaX, deltaY;

	if (!isInMouseDrag)
		return;
	deltaX = x - viewPortCenterX;
	deltaY = y - viewPortCenterY;
	if (deltaX == 0 && deltaY == 0)
		return;
	glutWarpPointer(viewPortCenterX, viewPortCenterY);
	camYaw -= DEF_angleSensitivity * deltaX;
	camPitch -= DEF_angleSensitivity * deltaY * (mouseIsInverted ? -1.0 : 1.0);
	clampCamera();
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	if (!h)
		return;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	viewPortCenterX = w / 2;
	viewPortCenterY = h / 2;
}

void cilindro(int X, int Y, GLfloat h, GLfloat LADOS, GLfloat radio)
{
	GLfloat z, angle;

	int Contador = 0;

	GLfloat Puntos1[100][3];
	GLfloat Puntos2[100][3];

	glBegin(GL_POLYGON);

	//glColor3f(1,0,0);

	z = h;
	//radio = 5;
	Contador = 0;
	//printf ("\n Puntos Primer Poligono ");
	for (angle = 0.0f; angle <= (2.0f * GL_PI); angle += (2.0f * GL_PI) / LADOS)
	{
		Puntos1[Contador][0] = radio * sin(angle) + X;
		Puntos1[Contador][2] = radio * cos(angle) + Y;
		Puntos1[Contador][1] = z;
		// glTexCoord2f((0.5*sin(angle)+0.5),(0.5*cos(angle)+0.5));
		glVertex3f(Puntos1[Contador][0], Puntos1[Contador][1], Puntos1[Contador][2]);
		Contador++;
	}
	glEnd();

	//glColor3f(0,1,0);

	//printf ("\n Puntos Segundo Poligono ");
	glBegin(GL_POLYGON);

	z = -10;
	//radio = 5;
	Contador = 0;
	for (angle = 0.0f; angle <= (2.0f * GL_PI); angle += (2.0f * GL_PI) / LADOS)
	{
		Puntos2[Contador][0] = radio * sin(angle) + X;
		Puntos2[Contador][2] = radio * cos(angle) + Y;
		Puntos2[Contador][1] = z;
		//glTexCoord2f((0.5*sin(angle)+0.5),(0.5*cos(angle)+0.5));
		glVertex3f(Puntos2[Contador][0], Puntos2[Contador][1], Puntos2[Contador][2]);
		Contador++;
	}
	glEnd();

	//glColor3f(0.5,0.5,0.5);

	//GLfloat IncrementoLado = 1.0/LADOS;
	//GLfloat OffsetLado = 0.0;

	//GLfloat SigLado = IncrementoLado;

	for (Contador = 0; Contador < LADOS; Contador++)
	{
		glBegin(GL_POLYGON);
		//glColor3f(1.0,1.0,1.0/(Contador+1));
		int Temporal = 0;
		if (Contador == (int)(LADOS - 1))
			Temporal = 0;
		else
			Temporal = Contador + 1;

		//glTexCoord2f(OffsetLado, 1.0f);
		glVertex3f(Puntos1[Contador][0], Puntos1[Contador][1], Puntos1[Contador][2]);
		//glTexCoord2f(SigLado, 1.0f);
		glVertex3f(Puntos1[Temporal][0], Puntos1[Temporal][1], Puntos1[Temporal][2]);
		//glTexCoord2f(SigLado, 0.0f);
		glVertex3f(Puntos2[Temporal][0], Puntos2[Temporal][1], Puntos2[Temporal][2]);
		//glTexCoord2f(OffsetLado, 0.0f);
		glVertex3f(Puntos2[Contador][0], Puntos2[Contador][1], Puntos2[Contador][2]);
		glEnd();
	}
}

void sol()
{

	glColor3f(0.96, 0.96, 0.10);

	glutSolidSphere(10, 100, 100);
}

void pasto(float x, float z)
{

	//glBegin(GL_LINES);
	glBegin(GL_QUADS);

	glColor3f(0.02, 0.42, 0.02);

	glVertex3f(0, -8, 0);
	glVertex3f(x, -8, 0);

	glVertex3f(0, -8, z);
	glVertex3f(x, -8, z);

	glVertex3f(0, -8, 0);
	glVertex3f(0, -8, z);

	glVertex3f(x, -8, 0);
	glVertex3f(x, -8, z);

	glEnd();
}

void llanta(float x, float y, float z)
{
	GLfloat ballRadius = 3;

	glTranslatef(x, y, z);
	int numSegments = 100, i;
	GLfloat angle;

	ballRadius = 2.7;

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1, 1, 1);
	glVertex2f(0.0f, 0.0f); // centro del círculo

	for (i = 0; i <= numSegments; i++)
	{
		angle = i * 2.0f * PI / numSegments;
		glVertex2f(cos(angle) * ballRadius, sin(angle) * ballRadius);
	}
	glEnd();

	ballRadius = 2;

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0, 0, 0);
	glVertex2f(0.0f, 0.0f); // centro del círculo

	for (i = 0; i <= numSegments; i++)
	{
		angle = i * 2.0f * PI / numSegments;
		glVertex2f(cos(angle) * ballRadius, sin(angle) * ballRadius);
	}
	glEnd();
}

void mural()
{
	glBegin(GL_QUADS);

	glColor3f(1, 1, 1);

	glVertex3f(-14, 0, 0);
	glVertex3f(0, 0, 0);

	glVertex3f(0, 0, 0);
	glVertex3f(0, 30, 0); //cara enfrente

	glVertex3f(-14, 0, 0);
	glVertex3f(-14, 30, 0);

	glVertex3f(-14, 30, 0);
	glVertex3f(0, 30, 0);

	glVertex3f(-14, 0, -2);
	glVertex3f(0, 0, -2);

	glVertex3f(0, 0, -2);
	glVertex3f(0, 30, -2); //cara atras

	glVertex3f(-14, 0, -2);
	glVertex3f(-14, 30, -2);

	glVertex3f(-14, 30, -2);
	glVertex3f(0, 30, -2);

	glVertex3f(-14, 30, 0);
	glVertex3f(-14, 30, -2);

	glVertex3f(-14, 0, 0);
	glVertex3f(-14, 0, -2);
	//cara lado1
	glVertex3f(-14, 0, 0);
	glVertex3f(-14, 30, 0);

	glVertex3f(-14, 0, -2);
	glVertex3f(-14, 30, -2);

	glVertex3f(0, 30, 0);
	glVertex3f(0, 30, -2);

	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, -2);
	//cara lado2
	glVertex3f(0, 0, 0);
	glVertex3f(0, 30, 0);

	glVertex3f(0, 0, -2);
	glVertex3f(0, 30, -2);

	glVertex3f(-14, 30, 0);
	glVertex3f(-14, 30, -2);

	glVertex3f(0, 30, 0);
	glVertex3f(0, 30, -2);
	//cara arriba
	glVertex3f(-14, 30, 0);
	glVertex3f(0, 30, 0);

	glVertex3f(-14, 30, -2);
	glVertex3f(0, 30, -2);

	glVertex3f(-14, 0, 0);
	glVertex3f(-14, 0, -2);

	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, -2);
	//cara arriba
	glVertex3f(-14, 0, 0);
	glVertex3f(0, 0, 0);

	glVertex3f(-14, 0, -2);
	glVertex3f(0, 0, -2);

	glEnd();

	/////////////////////////////

	glColor3f(1, 0, 0);

	glBegin(GL_QUADS);

	glVertex3f(-14.1, 0, 0);
	glVertex3f(-14.1, 30, 0);

	glVertex3f(-17, 0, 0);
	glVertex3f(-17, 30, 0);
	//lado1
	glVertex3f(-14.1, 0, 0);
	glVertex3f(-17, 0, 0);

	glVertex3f(-14.1, 30, 0);
	glVertex3f(-17, 30, 0);

	glVertex3f(-14.1, 0, -2);
	glVertex3f(-14.1, 30, -2);

	glVertex3f(-17, 0, -2);
	glVertex3f(-17, 30, -2);
	//lado2
	glVertex3f(-14.1, 0, -2);
	glVertex3f(-17, 0, -2);

	glVertex3f(-14.1, 30, -2);
	glVertex3f(-17, 30, -2);

	glVertex3f(-14.1, 0, 0);
	glVertex3f(-14.1, 0, -2);

	glVertex3f(-14.1, 30, 0);
	glVertex3f(-14.1, 30, -2);
	//cara1
	glVertex3f(-14.1, 0, 0);
	glVertex3f(-14.1, 30, 0);

	glVertex3f(-14.1, 0, -2);
	glVertex3f(-14.1, 30, -2);

	glVertex3f(-17, 0, 0);
	glVertex3f(-17, 0, -2);

	glVertex3f(-17, 30, 0);
	glVertex3f(-17, 30, -2);
	//cara2
	glVertex3f(-17, 0, 0);
	glVertex3f(-17, 30, 0);

	glVertex3f(-17, 0, -2);
	glVertex3f(-17, 30, -2);

	glVertex3f(3, 0, 0);
	glVertex3f(3, 30, 0);

	glVertex3f(0.1, 0, 0);
	glVertex3f(0.1, 30, 0);
	//cara1
	glVertex3f(3, 0, 0);
	glVertex3f(0.1, 0, 0);

	glVertex3f(3, 30, 0);
	glVertex3f(0.1, 30, 0);

	glVertex3f(3, 0, -2);
	glVertex3f(3, 30, -2);

	glVertex3f(0.1, 0, -2);
	glVertex3f(0.1, 30, -2);
	//cara2
	glVertex3f(3, 0, -2);
	glVertex3f(0.1, 0, -2);

	glVertex3f(3, 30, -2);
	glVertex3f(0.1, 30, -2);

	///////////////////////////////////////////////////////

	glVertex3f(0.1, 30, 0);
	glVertex3f(0.1, 30, -2);

	glVertex3f(0.1, 0, 0);
	glVertex3f(0.1, 0, -2);
	//cara lado2
	glVertex3f(0.1, 0, 0);
	glVertex3f(0.1, 30, 0);

	glVertex3f(0.1, 0, -2);
	glVertex3f(0.1, 30, -2);

	glVertex3f(3, 30, 0);
	glVertex3f(3, 30, -2);

	glVertex3f(3, 0, 0);
	glVertex3f(3, 0, -2);
	//cara lado2
	glVertex3f(3, 0, 0);
	glVertex3f(3, 30, 0);

	glVertex3f(3, 0, -2);
	glVertex3f(3, 30, -2);

	glEnd();

	//////////////////////////////Sombra
	glBegin(GL_QUADS);

	glColor3f(0.035, 0.21, 0.03);

	glVertex3f(-17, 0.1, -2);
	glVertex3f(3, 0.1, -2);

	glVertex3f(-17, 0.1, -40);
	glVertex3f(3, 0.1, -40);

	glVertex3f(-17, 0.1, -2);
	glVertex3f(-17, 0.1, -40);

	glVertex3f(3, 0.1, -2);
	glVertex3f(3, 0.1, -40);

	glEnd();
}

void arbol()
{

	int X = 10;
	int Y = 10;
	GLfloat LADOS = 30;
	GLfloat z, angle;
	GLfloat radio;
	int Contador = 0;

	GLfloat Puntos1[100][3];
	GLfloat Puntos2[100][3];

	;

	glBegin(GL_POLYGON);

	glColor3f(0.6, 0.4, 0.2);

	z = 10;
	radio = 3;
	Contador = 0;
	//printf ("\n Puntos Primer Poligono ");
	for (angle = 0.0f; angle <= (2.0f * PI); angle += (2.0f * PI) / LADOS)
	{
		Puntos1[Contador][0] = radio * sin(angle) + X;
		Puntos1[Contador][2] = radio * cos(angle) + Y;
		Puntos1[Contador][1] = z;
		// glTexCoord2f((0.5*sin(angle)+0.5),(0.5*cos(angle)+0.5));
		glVertex3f(Puntos1[Contador][0], Puntos1[Contador][1], Puntos1[Contador][2]);
		Contador++;
	}
	glEnd();

	glColor3f(0, 0, 0);

	//printf ("\n Puntos Segundo Poligono ");
	glBegin(GL_POLYGON);

	z = -10;
	radio = 3;
	Contador = 0;
	for (angle = 0.0f; angle <= (2.0f * PI); angle += (2.0f * PI) / LADOS)
	{
		Puntos2[Contador][0] = radio * sin(angle) + X;
		Puntos2[Contador][2] = radio * cos(angle) + Y;
		Puntos2[Contador][1] = z;
		//glTexCoord2f((0.5*sin(angle)+0.5),(0.5*cos(angle)+0.5));
		glVertex3f(Puntos2[Contador][0], Puntos2[Contador][1], Puntos2[Contador][2]);
		Contador++;
	}
	glEnd();

	glColor3f(0.6, 0.4, 0.2);

	for (Contador = 0; Contador < LADOS; Contador++)
	{
		glBegin(GL_POLYGON);
		//glColor3f(1.0,1.0,1.0/(Contador+1));
		int Temporal = 0;
		if (Contador == (int)(LADOS - 1))
			Temporal = 0;
		else
			Temporal = Contador + 1;

		//glTexCoord2f(OffsetLado, 1.0f);
		glVertex3f(Puntos1[Contador][0], Puntos1[Contador][1], Puntos1[Contador][2]);
		//glTexCoord2f(SigLado, 1.0f);
		glVertex3f(Puntos1[Temporal][0], Puntos1[Temporal][1], Puntos1[Temporal][2]);
		//glTexCoord2f(SigLado, 0.0f);
		glVertex3f(Puntos2[Temporal][0], Puntos2[Temporal][1], Puntos2[Temporal][2]);
		//glTexCoord2f(OffsetLado, 0.0f);
		glVertex3f(Puntos2[Contador][0], Puntos2[Contador][1], Puntos2[Contador][2]);
		glEnd();
	}

	glPushMatrix();
	glTranslatef(8, 18, 5);
	glColor3f(0.04, 0.49, 0.04);
	glutSolidSphere(6, 30, 30);
	glPopMatrix();

	//hojas 1
	glPushMatrix();
	glColor3f(0.035, 0.6, 0.035);
	glTranslatef(15, 15, 9);	//lados,Altura
	glutSolidSphere(6, 30, 30); //Gorda la bolita
	glPopMatrix();

	//hojas 2
	glPushMatrix();
	glColor3f(0.04, 0.49, 0.04);
	glTranslatef(15, 22, 9);	//lados,Altura
	glutSolidSphere(6, 30, 30); //Gorda la bolita
	glPopMatrix();

	//hojas 3
	glPushMatrix();
	glColor3f(0.035, 0.6, 0.035);
	glTranslatef(8, 25, 9);		//lados,Altura
	glutSolidSphere(6, 30, 30); //Gorda la bolita
	glPopMatrix();

	//hojas 4
	glPushMatrix();
	glColor3f(0.04, 0.49, 0.04);
	glTranslatef(2.5, 22, 9);	//lados,Altura
	glutSolidSphere(6, 30, 30); //Gorda la bolita
	glPopMatrix();

	//hojas 5
	glPushMatrix();
	glColor3f(0.035, 0.6, 0.035);
	glTranslatef(2.5, 15, 9);	//lados,Altura
	glutSolidSphere(6, 30, 30); //Gorda la bolita
	glPopMatrix();

	//hojas 6
	glPushMatrix();
	glColor3f(0.04, 0.49, 0.04);
	glTranslatef(9.5, 12, 9);	//lados,Altura
	glutSolidSphere(6, 30, 30); //Gorda la bolita
	glPopMatrix();
}

void pared_blanca()

{
	glBegin(GL_QUADS);

	glColor3f(0.94, 0.96, 0.79);

	glVertex3f(0, 0, 0);
	glVertex3f(0, 15, 0);

	glVertex3f(-65, 0, 0);
	glVertex3f(-65, 15, 0);
	//cuadrado1 frente
	glVertex3f(0, 0, 0);
	glVertex3f(-65, 0, 0);

	glVertex3f(0, 15, 0);
	glVertex3f(-65, 15, 0);

	glVertex3f(0, 0, -2);
	glVertex3f(0, 15, -2);

	glVertex3f(-65, 0, -2);
	glVertex3f(-65, 15, -2);
	//cuadrado2 atras
	glVertex3f(0, 0, -2);
	glVertex3f(-65, 0, -2);

	glVertex3f(0, 15, -2);
	glVertex3f(-65, 15, -2);

	glVertex3f(-65, 0, 0);
	glVertex3f(-65, 0, -2);

	glVertex3f(-65, 15, 0);
	glVertex3f(-65, 15, -2);
	//cuadrado3 lado
	glVertex3f(-65, 0, 0);
	glVertex3f(-65, 15, 0);

	glVertex3f(-65, 0, -2);
	glVertex3f(-65, 15, -2);

	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, -2);

	glVertex3f(0, 15, 0);
	glVertex3f(0, 15, -2);
	//cuadrado4 lado
	glVertex3f(0, 0, 0);
	glVertex3f(0, 15, 0);

	glVertex3f(0, 0, -2);
	glVertex3f(0, 15, -2);

	glEnd();
}

void edificio_redes()
{

	glPushMatrix();
	glTranslatef(0, 0, -100);
	pared_blanca();
	glPopMatrix();

	glBegin(GL_QUADS);

	glColor3f(0.94, 0.96, 0.79);

	glVertex3f(0, 0, 0);
	glVertex3f(0, 15, 0);

	glVertex3f(-65, 0, 0);
	glVertex3f(-65, 15, 0);
	//cuadrado1 frente
	glVertex3f(0, 0, 0);
	glVertex3f(-65, 0, 0);

	glVertex3f(0, 15, 0);
	glVertex3f(-65, 15, 0);

	glVertex3f(0, 0, -2);
	glVertex3f(0, 15, -2);

	glVertex3f(-65, 0, -2);
	glVertex3f(-65, 15, -2);
	//cuadrado2 atras
	glVertex3f(0, 0, -2);
	glVertex3f(-65, 0, -2);

	glVertex3f(0, 15, -2);
	glVertex3f(-65, 15, -2);

	glVertex3f(-65, 0, 0);
	glVertex3f(-65, 0, -2);

	glVertex3f(-65, 15, 0);
	glVertex3f(-65, 15, -2);
	//cuadrado3 lado
	glVertex3f(-65, 0, 0);
	glVertex3f(-65, 15, 0);

	glVertex3f(-65, 0, -2);
	glVertex3f(-65, 15, -2);

	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, -2);

	glVertex3f(0, 15, 0);
	glVertex3f(0, 15, -2);
	//cuadrado4 lado
	glVertex3f(0, 0, 0);
	glVertex3f(0, 15, 0);

	glVertex3f(0, 0, -2);
	glVertex3f(0, 15, -2);

	glEnd();

	glBegin(GL_QUADS);

	glColor3f(1, 0, 0);

	glVertex3f(-60, 0, -2);
	glVertex3f(-60, 15, -2);

	glVertex3f(-60, 0, -100);
	glVertex3f(-60, 15, -100); //cuadro lado1

	glVertex3f(-60, 0, -2);
	glVertex3f(-60, 0, -100);

	glVertex3f(-60, 15, -2);
	glVertex3f(-60, 15, -100);

	glVertex3f(-3, 0, -2);
	glVertex3f(-3, 15, -2);

	glVertex3f(-3, 0, -100);
	glVertex3f(-3, 15, -100);
	//cuadro lado2
	glVertex3f(-3, 0, -2);
	glVertex3f(-3, 0, -100);

	glVertex3f(-3, 15, -2);
	glVertex3f(-3, 15, -100);

	glVertex3f(-60, 0, -2);
	glVertex3f(-3, 0, -2);

	glVertex3f(-60, 15, -2);
	glVertex3f(-3, 15, -2);
	//cuadro enfrente
	glVertex3f(-3, 0, -2);
	glVertex3f(-3, 15, -2);

	glVertex3f(-60, 0, -2);
	glVertex3f(-60, 15, -2);

	glVertex3f(-60, 0, -100);
	glVertex3f(-3, 0, -100);

	glVertex3f(-60, 15, -100);
	glVertex3f(-3, 15, -100);
	//cuadro atras
	glVertex3f(-3, 0, -100);
	glVertex3f(-3, 15, -100);

	glVertex3f(-60, 0, -100);
	glVertex3f(-60, 15, -100);

	glVertex3f(-60, 15, -2);
	glVertex3f(-3, 15, -2);

	glVertex3f(-60, 15, -100);
	glVertex3f(-3, 15, -100);

	glVertex3f(-3, 15, -2); //cuadro arriba
	glVertex3f(-3, 15, -100);

	glVertex3f(-60, 15, -2);
	glVertex3f(-60, 15, -100);

	glColor3f(1, 1, 1);
	glVertex3f(-60, 0, -2);
	glVertex3f(-3, 0, -2);

	glVertex3f(-60, 0, -100);
	glVertex3f(-3, 0, -100);

	glVertex3f(-3, 0, -2); //cuadro arriba
	glVertex3f(-3, 0, -100);

	glVertex3f(-60, 0, -2);
	glVertex3f(-60, 0, -100);

	glEnd();
}

void cuadrado(float x, float y, float z)
{

	//ancho 3  largo -10   profundidad -2

	glBegin(GL_QUADS);

	glVertex3f(0, 0, 0);
	glVertex3f(0, y, 0);

	glVertex3f(x, 0, 0);
	glVertex3f(x, y, 0);
	//cuadrado enfrente
	glVertex3f(0, 0, 0);
	glVertex3f(x, 0, 0);

	glVertex3f(0, y, 0);
	glVertex3f(x, y, 0);

	glVertex3f(0, 0, z);
	glVertex3f(0, y, z);

	glVertex3f(x, 0, z);
	glVertex3f(x, y, z);
	//cuadrado atras
	glVertex3f(0, 0, z);
	glVertex3f(x, 0, z);

	glVertex3f(0, y, z);
	glVertex3f(x, y, z);

	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, z);

	glVertex3f(0, y, 0);
	glVertex3f(0, y, z);
	//cuadrado lado1
	glVertex3f(0, 0, 0);
	glVertex3f(0, y, 0);

	glVertex3f(0, 0, z);
	glVertex3f(0, y, z);

	glVertex3f(x, 0, 0);
	glVertex3f(x, 0, z);

	glVertex3f(x, y, 0);
	glVertex3f(x, y, z);
	//cuadrado lado2
	glVertex3f(x, 0, 0);
	glVertex3f(x, y, 0);

	glVertex3f(x, 0, z);
	glVertex3f(x, y, z);

	glVertex3f(0, 0, 0);
	glVertex3f(x, 0, 0);

	glVertex3f(0, 0, z);
	glVertex3f(x, 0, z);
	//cuadrado arriba
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, z);

	glVertex3f(x, 0, 0);
	glVertex3f(x, 0, z);

	glVertex3f(0, y, 0);
	glVertex3f(x, y, 0);

	glVertex3f(0, y, z);
	glVertex3f(x, y, z);
	//cuadrado abajo
	glVertex3f(0, y, 0);
	glVertex3f(0, y, z);

	glVertex3f(x, y, 0);
	glVertex3f(x, y, z);

	glEnd();
}

void Edificio_nuevo()
{

	glColor3f(0.79, 0.12, 0.12);

	glBegin(GL_QUADS);

	glVertex3f(-50, 30, 0); //horizontal arriba//
	glVertex3f(50, 30, 0);

	glVertex3f(-50, 0, 0); //horizontal abajo//
	glVertex3f(50, 0, 0);
	//========CUADRADO ENFRENTE
	glVertex3f(-50, 30, 0); // vertical izquierda//
	glVertex3f(-50, 0, 0);

	glVertex3f(50, 30, 0); //vertical derecha//
	glVertex3f(50, 0, 0);

	//otro cuadro con profundidad//

	glVertex3f(-50, 30, 50); //horizontal arriba//
	glVertex3f(50, 30, 50);

	glVertex3f(-50, 0, 50); //horizontal abajo//
	glVertex3f(50, 0, 50);
	//======CUADRADO ATRAS
	glVertex3f(-50, 30, 50); // vertical izquierda//
	glVertex3f(-50, 0, 50);

	glVertex3f(50, 30, 50); //vertical derecha//
	glVertex3f(50, 0, 50);

	//4 lineas de union//

	glVertex3f(-50, 30, 50);
	glVertex3f(-50, 30, 0);

	glVertex3f(-50, 0, 50);
	glVertex3f(-50, 0, 0);
	//========cuadrado lado1
	glVertex3f(-50, 30, 50);
	glVertex3f(-50, 0, 50);

	glVertex3f(-50, 30, 0);
	glVertex3f(-50, 0, 0);

	glVertex3f(50, 30, 50);
	glVertex3f(50, 30, 0);

	glVertex3f(50, 0, 50);
	glVertex3f(50, 0, 0);
	//========cuadrado lado2
	glVertex3f(50, 30, 50);
	glVertex3f(50, 0, 50);

	glVertex3f(50, 30, 0);
	glVertex3f(50, 0, 0);

	glVertex3f(-50, 30, 0);
	glVertex3f(-50, 30, 50);

	glVertex3f(50, 30, 0);
	glVertex3f(50, 30, 50);

	glVertex3f(-50, 30, 0);
	glVertex3f(50, 30, 0);

	glVertex3f(-50, 30, 50);
	glVertex3f(50, 30, 50);

	glEnd();

	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);

	glVertex3f(-10, 0, 51);
	glVertex3f(-10, 30, 51);

	glVertex3f(-20, 0, 51);
	glVertex3f(-20, 30, 51);

	glVertex3f(-30, 0, 51);
	glVertex3f(-30, 30, 51);

	glVertex3f(-40, 0, 51);
	glVertex3f(-40, 30, 51);

	glVertex3f(-10, 30, 51);
	glVertex3f(-20, 30, 51);

	glVertex3f(-10, 0, 51);
	glVertex3f(-20, 0, 51);

	glVertex3f(-30, 30, 51);
	glVertex3f(-40, 30, 51);

	glVertex3f(-30, 0, 51);
	glVertex3f(-40, 0, 51);
	//
	glVertex3f(10, 0, 51);
	glVertex3f(10, 30, 51);

	glVertex3f(20, 0, 51);
	glVertex3f(20, 30, 51);

	glVertex3f(30, 0, 51);
	glVertex3f(30, 30, 51);

	glVertex3f(40, 0, 51);
	glVertex3f(40, 30, 51);

	glVertex3f(10, 30, 51);
	glVertex3f(20, 30, 51);

	glVertex3f(10, 0, 51);
	glVertex3f(20, 0, 51);

	glVertex3f(30, 30, 51);
	glVertex3f(40, 30, 51);

	glVertex3f(30, 0, 51);
	glVertex3f(40, 0, 51);

	////////////////////hacia atrass////////////7777

	glVertex3f(-10, 0, -1);
	glVertex3f(-10, 30, -1);

	glVertex3f(-20, 0, -1);
	glVertex3f(-20, 30, -1);

	glVertex3f(-30, 0, -1);
	glVertex3f(-30, 30, -1);

	glVertex3f(-40, 0, -1);
	glVertex3f(-40, 30, -1);

	glVertex3f(-10, 30, -1);
	glVertex3f(-20, 30, -1);

	glVertex3f(-10, 0, -1);
	glVertex3f(-20, 0, -1);

	glVertex3f(-30, 30, -1);
	glVertex3f(-40, 30, -1);

	glVertex3f(-30, 0, -1);
	glVertex3f(-40, 0, -1);
	//
	glVertex3f(10, 0, -1);
	glVertex3f(10, 30, -1);

	glVertex3f(20, 0, -1);
	glVertex3f(20, 30, -1);

	glVertex3f(30, 0, -1);
	glVertex3f(30, 30, -1);

	glVertex3f(40, 0, -1);
	glVertex3f(40, 30, -1);

	glVertex3f(10, 30, -1);
	glVertex3f(20, 30, -1);

	glVertex3f(10, 0, -1);
	glVertex3f(20, 0, -1);

	glVertex3f(30, 30, -1);
	glVertex3f(40, 30, -1);

	glVertex3f(30, 0, -1);
	glVertex3f(40, 0, -1);

	glEnd();

	////PUERTA///
	glBegin(GL_QUADS);
	glColor3f(0, 0, 0);

	glVertex3f(-5, 0, 51);
	glVertex3f(-5, 15, 51);

	glVertex3f(5, 0, 51);
	glVertex3f(5, 15, 51);

	glVertex3f(-5, 0, 51);
	glVertex3f(5, 0, 51);

	glVertex3f(-5, 15, 51);
	glVertex3f(5, 15, 51);

	glEnd();
}

void persona2()
{
	//=========movimiento cuadricep y rodilla

	if (c1 == 0)
	{
		if (cuadricep < 60)
		{
			cuadricep += 3;
		}
		if (cuadricep == 60)
		{
			c1 = 60;
		}

		//////////////////////

		if (cuadricep2 > 0)
		{
			cuadricep2 -= 3;
		}
		if (cuadricep2 == 0)
		{
			c2 = 0;
		}
	}

	if (c1 == 60)
	{
		if (cuadricep > 0)
		{
			cuadricep -= 3;
		}
		if (cuadricep == 0)
		{
			c1 = 0;
		}

		//////////////////

		if (cuadricep2 < 60)
		{
			cuadricep2 += 3;
		}
		if (cuadricep2 == 60)
		{
			c2 = 60;
		}
	}

	//cabeza
	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(1, 7.5, 0);
	glutSolidSphere(1.5, 100, 100);
	glPopMatrix();

	//////cuello
	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(1, 14, 0);
	cilindro(0, 0, -8, 30, 0.5);
	glPopMatrix();

	//======hombros
	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(-1.3, 5, 0);
	glutSolidSphere(0.7, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(3.3, 5, 0);
	glutSolidSphere(0.7, 100, 100);
	glPopMatrix();

	//========biceps
	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(-1.3, 12.6, 0);
	cilindro(0, 0, -8, 30, 0.5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(3.3, 12.6, 0);
	cilindro(0, 0, -8, 30, 0.5);
	glPopMatrix();

	//========codos
	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(-1.3, 2.5, 0);
	glutSolidSphere(0.6, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(3.3, 2.5, 0);
	glutSolidSphere(0.6, 100, 100);
	glPopMatrix();

	//=====antebrazos
	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(-1.3, 10.1, 0);
	cilindro(0, 0, -8, 30, 0.5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(3.3, 10.1, 0);
	cilindro(0, 0, -8, 30, 0.5);
	glPopMatrix();

	//========manos
	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(-1.3, 0, 0);
	glutSolidSphere(0.6, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(3.3, 0, 0);
	glutSolidSphere(0.6, 100, 100);
	glPopMatrix();

	//========torax
	glPushMatrix();
	glTranslatef(-0.7, 0.4, 0.5);
	glColor3f(1, 0, 0);
	cuadrado(3.4, 5, -1);
	glPopMatrix();

	//========truza
	glPushMatrix();
	glTranslatef(0.5, -0.5, 0.5);
	glColor3f(0.26, 0.29, 0.49);
	cuadrado(1, 1, -1);
	glPopMatrix();
	//=========pierna1

	//Cadera
	glPushMatrix();

	glTranslatef(0, 0, 0);
	glutSolidSphere(0.7, 100, 100);
	glPopMatrix();

	//Cuadriceps
	glPushMatrix();

	glRotatef(-cuadricep, 1, 0, 0);
	glTranslatef(0, 7, 0);
	cilindro(0, 0, -7, 30, 0.6);
	glPopMatrix();

	//Rodilla
	glPushMatrix();

	glRotatef(-cuadricep, 1, 0, 0);
	glTranslatef(0, -3, 0);
	glutSolidSphere(0.7, 100, 100);

	//espinilla

	glRotatef(cuadricep, 3, 0, 0);
	glTranslatef(0, 7, 0);
	cilindro(0, 0, -7, 30, 0.6);

	//zapatos
	glColor3f(0, 0, 0);
	glTranslatef(0.7, -10, -0.4);
	cuadrado(-1.5, -0.4, 2);
	glPopMatrix();

	//==================pierna2

	//Cadera
	glPushMatrix();
	glColor3f(0.26, 0.29, 0.49);
	glTranslatef(2.1, 0, 0);
	glutSolidSphere(0.7, 100, 100);
	glPopMatrix();

	//Cuadriceps
	glPushMatrix();
	glColor3f(0.26, 0.29, 0.49);
	glRotatef(-cuadricep2, 1, 0, 0);
	glTranslatef(2.1, 7, 0);
	cilindro(0, 0, -7, 30, 0.6);
	glPopMatrix();

	//Rodilla
	glPushMatrix();

	glRotatef(-cuadricep2, 1, 0, 0);
	glTranslatef(2.1, -3, 0);
	glutSolidSphere(0.7, 100, 100);

	//espinilla

	glRotatef(cuadricep2, 3, 0, 0);
	glTranslatef(0, 7, 0);
	cilindro(0, 0, -7, 30, 0.6);

	//zapatos
	glColor3f(0, 0, 0);
	glTranslatef(0.7, -10, -0.4);
	cuadrado(-1.5, -0.4, 2);
	glPopMatrix();
}

void Isis()
{
	//cabeza
	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(0, 0, 0);
	glutSolidSphere(2, 100, 100);
	glPopMatrix();

	//ojos

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(-0.8, 0.2, 1.7);
	glutSolidSphere(0.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(-0.8, 0.2, 1.9);
	glutSolidSphere(0.2, 100, 100);
	glPopMatrix();

	//////ojo2

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(0.8, 0.2, 1.7);
	glutSolidSphere(0.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0.8, 0.2, 1.9);
	glutSolidSphere(0.2, 100, 100);
	glPopMatrix();

	//cabello

	glPushMatrix();
	glColor3f(0.93, 0.65, 0.13); //amarillo
	glTranslatef(0, 0.7, 0);
	glutSolidSphere(1.8, 100, 100);
	glPopMatrix();

	glPushMatrix();

	glTranslatef(0, 0, -0.5);
	glutSolidSphere(1.9, 100, 100);
	glPopMatrix();

	glPushMatrix();

	glTranslatef(0.8, 0.4, 0);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	//
	glPushMatrix();
	glColor3f(0.93, 0.65, 0.13);
	glTranslatef(-0.8, 0.4, 0);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.93, 0.65, 0.13);
	glTranslatef(-0.8, 0, 0);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.93, 0.65, 0.13);
	glTranslatef(-0.8, -0.1, 0);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	//

	glPushMatrix();
	glColor3f(0.93, 0.65, 0.13);
	glTranslatef(0.8, 0.4, 0);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.93, 0.65, 0.13);
	glTranslatef(0.8, 0, 0);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.93, 0.65, 0.13);
	glTranslatef(0.8, -0.1, 0);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.93, 0.65, 0.13);
	glTranslatef(0.8, -0.4, 0);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.93, 0.65, 0.13);
	glTranslatef(-0.8, -0.4, 0);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	//

	glPushMatrix();
	glColor3f(0.93, 0.65, 0.13);
	glTranslatef(0.8, -0.7, -0.5);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.93, 0.65, 0.13);
	glTranslatef(0.8, -0.9, -0.5);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.93, 0.65, 0.13);
	glTranslatef(0.8, -1.2, -0.5);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.93, 0.65, 0.13);
	glTranslatef(0.8, -1.2, -0.5);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.93, 0.65, 0.13);
	glTranslatef(0.8, -1.4, -0.5);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	//

	glPushMatrix();
	glColor3f(0.93, 0.65, 0.13);
	glTranslatef(-0.8, -0.7, -0.5);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.93, 0.65, 0.13);
	glTranslatef(-0.8, -0.9, -0.5);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.93, 0.65, 0.13);
	glTranslatef(-0.8, -1.2, -0.5);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.93, 0.65, 0.13);
	glTranslatef(-0.8, -1.2, -0.5);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.93, 0.65, 0.13);
	glTranslatef(-0.8, -1.4, -0.5);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.93, 0.65, 0.13);
	glTranslatef(0.8, -1.8, -0.6);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.93, 0.65, 0.13);
	glTranslatef(0.8, -2, -0.6);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.93, 0.65, 0.13);
	glTranslatef(0.8, -2.4, -0.6);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	////

	glPushMatrix();
	glColor3f(0.93, 0.65, 0.13);
	glTranslatef(-0.8, -1.8, -0.6);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.93, 0.65, 0.13);
	glTranslatef(-0.8, -2, -0.6);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.93, 0.65, 0.13);
	glTranslatef(-0.8, -2.4, -0.6);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	//////cuello
	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(0, 7.5, 0);
	cilindro(0, 0, -8, 30, 1);
	glPopMatrix();

	//torax

	glBegin(GL_QUADS);
	glColor3f(0.03, 0.92, 0.54);

	glVertex3f(-2, -2.6, 1);
	glVertex3f(2, -2.6, 1);

	glVertex3f(-2, -2.6, -1);
	glVertex3f(2, -2.6, -1);
	//cuadrado arriba
	glVertex3f(-2, -2.6, 1);
	glVertex3f(-2, -2.6, -1);

	glVertex3f(2, -2.6, 1);
	glVertex3f(2, -2.6, -1);

	glVertex3f(-2, -7.5, 1);
	glVertex3f(2, -7.5, 1);

	glVertex3f(-2, -7.5, -1);
	glVertex3f(2, -7.5, -1);
	//cuadrado abajo
	glVertex3f(-2, -7.5, 1);
	glVertex3f(-2, -7.5, -1);

	glVertex3f(2, -7.5, 1);
	glVertex3f(2, -7.5, -1);

	glVertex3f(-2, -2.6, 1);
	glVertex3f(-2, -7.5, 1);

	glVertex3f(2, -2.6, 1);
	glVertex3f(2, -7.5, 1);
	//cuadrado frente
	glVertex3f(-2, -2.6, 1);
	glVertex3f(2, -2.6, 1);

	glVertex3f(-2, -7.5, 1);
	glVertex3f(2, -7.5, 1);

	glVertex3f(-2, -2.6, -1);
	glVertex3f(-2, -7.5, -1);

	glVertex3f(2, -2.6, -1);
	glVertex3f(2, -7.5, -1);
	//cuadrado atras
	glVertex3f(-2, -2.6, -1);
	glVertex3f(2, -2.6, -1);

	glVertex3f(-2, -7.5, -1);
	glVertex3f(2, -7.5, -1);

	glVertex3f(-2, -2.6, 1);
	glVertex3f(-2, -2.6, -1);

	glVertex3f(-2, -7.5, 1);
	glVertex3f(-2, -7.5, -1);
	//cuadrado lado1
	glVertex3f(-2, -2.6, -1);
	glVertex3f(-2, -7.5, -1);

	glVertex3f(-2, -2.6, 1);
	glVertex3f(-2, -7.5, 1);

	glVertex3f(2, -2.6, 1);
	glVertex3f(2, -2.6, -1);

	glVertex3f(2, -7.5, 1);
	glVertex3f(2, -7.5, -1);
	//cuadrado lado2
	glVertex3f(2, -2.6, -1);
	glVertex3f(2, -7.5, -1);

	glVertex3f(2, -2.6, 1);
	glVertex3f(2, -7.5, 1);

	glEnd();

	//////////////////////////truza
	glColor3f(0.51, 0.77, 0.89);

	glBegin(GL_QUADS);

	glVertex3f(-0.8, -7.5, 1);
	glVertex3f(0.8, -7.5, 1);

	glVertex3f(-0.8, -8.5, 1);
	glVertex3f(0.8, -8.5, 1);
	//CUADRADO FRENTE
	glVertex3f(-0.8, -7.5, 1);
	glVertex3f(-0.8, -8.5, 1);

	glVertex3f(0.8, -7.5, 1);
	glVertex3f(0.8, -8.5, 1);

	glVertex3f(-0.8, -7.5, -1);
	glVertex3f(0.8, -7.5, -1);

	glVertex3f(-0.8, -8.5, -1);
	glVertex3f(0.8, -8.5, -1);
	//CUADRADO ATRAS
	glVertex3f(-0.8, -7.5, -1);
	glVertex3f(-0.8, -8.5, -1);

	glVertex3f(0.8, -7.5, -1);
	glVertex3f(0.8, -8.5, -1);

	glVertex3f(-0.8, -7.5, 1);
	glVertex3f(-0.8, -7.5, -1);

	glVertex3f(-0.8, -8.5, 1);
	glVertex3f(-0.8, -8.5, -1);
	//cuadrado lado1
	glVertex3f(-0.8, -7.5, 1);
	glVertex3f(-0.8, -8.5, 1);

	glVertex3f(-0.8, -7.5, -1);
	glVertex3f(-0.8, -8.5, -1);

	glVertex3f(0.8, -7.5, 1);
	glVertex3f(0.8, -7.5, -1);

	glVertex3f(0.8, -8.5, 1);
	glVertex3f(0.8, -8.5, -1);
	//cuadrado lado2
	glVertex3f(0.8, -7.5, 1);
	glVertex3f(0.8, -8.5, 1);

	glVertex3f(0.8, -7.5, -1);
	glVertex3f(0.8, -8.5, -1);

	glEnd();

	//HOMBROS

	glPushMatrix();
	glColor3f(0.03, 0.92, 0.54);
	glTranslatef(-2.7, -3.2, 0);
	glutSolidSphere(0.7, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.03, 0.92, 0.54);
	glTranslatef(2.7, -3.2, 0);
	glutSolidSphere(0.7, 100, 100);
	glPopMatrix();

	//articulaciones union torax pierna
	glColor3f(0.51, 0.77, 0.89);

	glPushMatrix();

	glTranslatef(1.5, -8.3, 0);
	glutSolidSphere(0.8, 100, 100);
	glPopMatrix();

	glPushMatrix();

	glTranslatef(-1.5, -8.3, 0);
	glutSolidSphere(0.8, 100, 100);
	glPopMatrix();

	//biceps

	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(2.7, 4.4, 0);
	cilindro(0, 0, -8, 30, 0.5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(-2.7, 4.4, 0);
	cilindro(0, 0, -8, 30, 0.5);
	glPopMatrix();

	//codos

	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(-2.7, -5.8, 0);
	glutSolidSphere(0.6, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(2.7, -5.8, 0);
	glutSolidSphere(0.6, 100, 100);
	glPopMatrix();

	//antebrazos

	glPushMatrix();
	glTranslatef(2.7, 2, 0);
	cilindro(0, 0, -8, 30, 0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.7, 2, 0);
	cilindro(0, 0, -8, 30, 0.5);
	glPopMatrix();

	//manos

	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(2.7, -8.4, 0);
	glutSolidSphere(0.6, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(-2.7, -8.4, 0);
	glutSolidSphere(0.6, 100, 100);
	glPopMatrix();

	//cuadricep
	glColor3f(0.51, 0.77, 0.89);
	glPushMatrix();
	glTranslatef(1.5, -0.8, 0);
	cilindro(0, 0, -8, 30, 0.6);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, -0.8, 0);
	cilindro(0, 0, -8, 30, 0.6);
	glPopMatrix();

	//rodillas
	glColor3f(0.51, 0.77, 0.89);

	glPushMatrix();

	glTranslatef(1.5, -11, 0);
	glutSolidSphere(0.7, 100, 100);
	glPopMatrix();

	glPushMatrix();

	glTranslatef(-1.5, -11, 0);
	glutSolidSphere(0.7, 100, 100);
	glPopMatrix();

	//espinilla
	glColor3f(0.51, 0.77, 0.89);
	glPushMatrix();
	glTranslatef(-1.5, -4, 0);
	cilindro(0, 0, -7, 30, 0.6);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, -4, 0);
	cilindro(0, 0, -7, 30, 0.6);
	glPopMatrix();

	glPushMatrix();
	//zapatos

	glBegin(GL_QUADS);

	glColor3f(0.97, 0.18, 0.58);

	glVertex3f(-0.8, -14.5, -0.8);
	glVertex3f(-2.4, -14.5, -0.8);

	glVertex3f(-0.8, -14.5, 2);
	glVertex3f(-2.4, -14.5, 2);
	//zapato1
	glVertex3f(-0.8, -14.5, -0.8);
	glVertex3f(-0.8, -14.5, 2);

	glVertex3f(-2.4, -14.5, -0.8);
	glVertex3f(-2.4, -14.5, 2);

	glVertex3f(0.8, -14.5, -0.8);
	glVertex3f(2.4, -14.5, -0.8);

	glVertex3f(0.8, -14.5, 2);
	glVertex3f(2.4, -14.5, 2);
	//zapato
	glVertex3f(0.8, -14.5, -0.8);
	glVertex3f(0.8, -14.5, 2);

	glVertex3f(2.4, -14.5, -0.8);
	glVertex3f(2.4, -14.5, 2);

	glEnd();
	glPopMatrix();
}

void Dark()
{
	//cabeza
	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(0, 0, 0);
	glutSolidSphere(2, 100, 100);
	glPopMatrix();

	//ojos

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(-0.8, 0.2, 1.7);
	glutSolidSphere(0.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(-0.8, 0.2, 1.9);
	glutSolidSphere(0.2, 100, 100);
	glPopMatrix();

	//////ojo2

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(0.8, 0.2, 1.7);
	glutSolidSphere(0.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0.8, 0.2, 1.9);
	glutSolidSphere(0.2, 100, 100);
	glPopMatrix();

	//barba

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0, -1, 2);
	glutSolidTorus(0.2, 0.5, 30.0, 50.0);
	glPopMatrix();

	//////cuello
	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(0, 7.5, 0);
	cilindro(0, 0, -8, 30, 1);
	glPopMatrix();

	//torax

	glBegin(GL_QUADS);
	glColor3f(0, 0, 0);

	glVertex3f(-2, -2.6, 1);
	glVertex3f(2, -2.6, 1);

	glVertex3f(-2, -2.6, -1);
	glVertex3f(2, -2.6, -1);
	//cuadrado arriba
	glVertex3f(-2, -2.6, 1);
	glVertex3f(-2, -2.6, -1);

	glVertex3f(2, -2.6, 1);
	glVertex3f(2, -2.6, -1);

	glVertex3f(-2, -7.5, 1);
	glVertex3f(2, -7.5, 1);

	glVertex3f(-2, -7.5, -1);
	glVertex3f(2, -7.5, -1);
	//cuadrado abajo
	glVertex3f(-2, -7.5, 1);
	glVertex3f(-2, -7.5, -1);

	glVertex3f(2, -7.5, 1);
	glVertex3f(2, -7.5, -1);

	glVertex3f(-2, -2.6, 1);
	glVertex3f(-2, -7.5, 1);

	glVertex3f(2, -2.6, 1);
	glVertex3f(2, -7.5, 1);
	//cuadrado frente
	glVertex3f(-2, -2.6, 1);
	glVertex3f(2, -2.6, 1);

	glVertex3f(-2, -7.5, 1);
	glVertex3f(2, -7.5, 1);

	glVertex3f(-2, -2.6, -1);
	glVertex3f(-2, -7.5, -1);

	glVertex3f(2, -2.6, -1);
	glVertex3f(2, -7.5, -1);
	//cuadrado atras
	glVertex3f(-2, -2.6, -1);
	glVertex3f(2, -2.6, -1);

	glVertex3f(-2, -7.5, -1);
	glVertex3f(2, -7.5, -1);

	glVertex3f(-2, -2.6, 1);
	glVertex3f(-2, -2.6, -1);

	glVertex3f(-2, -7.5, 1);
	glVertex3f(-2, -7.5, -1);
	//cuadrado lado1
	glVertex3f(-2, -2.6, -1);
	glVertex3f(-2, -7.5, -1);

	glVertex3f(-2, -2.6, 1);
	glVertex3f(-2, -7.5, 1);

	glVertex3f(2, -2.6, 1);
	glVertex3f(2, -2.6, -1);

	glVertex3f(2, -7.5, 1);
	glVertex3f(2, -7.5, -1);
	//cuadrado lado2
	glVertex3f(2, -2.6, -1);
	glVertex3f(2, -7.5, -1);

	glVertex3f(2, -2.6, 1);
	glVertex3f(2, -7.5, 1);

	glEnd();

	//////////////////////////truza
	glColor3f(0, 0, 0);

	glBegin(GL_QUADS);

	glVertex3f(-0.8, -7.5, 1);
	glVertex3f(0.8, -7.5, 1);

	glVertex3f(-0.8, -8.5, 1);
	glVertex3f(0.8, -8.5, 1);
	//CUADRADO FRENTE
	glVertex3f(-0.8, -7.5, 1);
	glVertex3f(-0.8, -8.5, 1);

	glVertex3f(0.8, -7.5, 1);
	glVertex3f(0.8, -8.5, 1);

	glVertex3f(-0.8, -7.5, -1);
	glVertex3f(0.8, -7.5, -1);

	glVertex3f(-0.8, -8.5, -1);
	glVertex3f(0.8, -8.5, -1);
	//CUADRADO ATRAS
	glVertex3f(-0.8, -7.5, -1);
	glVertex3f(-0.8, -8.5, -1);

	glVertex3f(0.8, -7.5, -1);
	glVertex3f(0.8, -8.5, -1);

	glVertex3f(-0.8, -7.5, 1);
	glVertex3f(-0.8, -7.5, -1);

	glVertex3f(-0.8, -8.5, 1);
	glVertex3f(-0.8, -8.5, -1);
	//cuadrado lado1
	glVertex3f(-0.8, -7.5, 1);
	glVertex3f(-0.8, -8.5, 1);

	glVertex3f(-0.8, -7.5, -1);
	glVertex3f(-0.8, -8.5, -1);

	glVertex3f(0.8, -7.5, 1);
	glVertex3f(0.8, -7.5, -1);

	glVertex3f(0.8, -8.5, 1);
	glVertex3f(0.8, -8.5, -1);
	//cuadrado lado2
	glVertex3f(0.8, -7.5, 1);
	glVertex3f(0.8, -8.5, 1);

	glVertex3f(0.8, -7.5, -1);
	glVertex3f(0.8, -8.5, -1);

	glEnd();

	//HOMBROS

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(-2.7, -3.2, 0);
	glutSolidSphere(0.7, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(2.7, -3.2, 0);
	glutSolidSphere(0.7, 100, 100);
	glPopMatrix();

	//articulaciones union torax pierna
	glColor3f(0, 0, 0);

	glPushMatrix();

	glTranslatef(1.5, -8.3, 0);
	glutSolidSphere(0.8, 100, 100);
	glPopMatrix();

	glPushMatrix();

	glTranslatef(-1.5, -8.3, 0);
	glutSolidSphere(0.8, 100, 100);
	glPopMatrix();

	//biceps

	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(2.7, 4.4, 0);
	cilindro(0, 0, -8, 30, 0.5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(-2.7, 4.4, 0);
	cilindro(0, 0, -8, 30, 0.5);
	glPopMatrix();

	//codos

	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(-2.7, -5.8, 0);
	glutSolidSphere(0.6, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(2.7, -5.8, 0);
	glutSolidSphere(0.6, 100, 100);
	glPopMatrix();

	//antebrazos

	glPushMatrix();
	glTranslatef(2.7, 2, 0);
	cilindro(0, 0, -8, 30, 0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.7, 2, 0);
	cilindro(0, 0, -8, 30, 0.5);
	glPopMatrix();

	//manos

	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(2.7, -8.4, 0);
	glutSolidSphere(0.6, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(-2.7, -8.4, 0);
	glutSolidSphere(0.6, 100, 100);
	glPopMatrix();

	//cuadricep
	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(1.5, -0.8, 0);
	cilindro(0, 0, -8, 30, 0.6);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, -0.8, 0);
	cilindro(0, 0, -8, 30, 0.6);
	glPopMatrix();

	//rodillas
	glColor3f(0, 0, 0);

	glPushMatrix();

	glTranslatef(1.5, -11, 0);
	glutSolidSphere(0.7, 100, 100);
	glPopMatrix();

	glPushMatrix();

	glTranslatef(-1.5, -11, 0);
	glutSolidSphere(0.7, 100, 100);
	glPopMatrix();

	//espinilla
	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(-1.5, -4, 0);
	cilindro(0, 0, -7, 30, 0.6);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, -4, 0);
	cilindro(0, 0, -7, 30, 0.6);
	glPopMatrix();

	glPushMatrix();
	//zapatos

	glBegin(GL_QUADS);

	glColor3f(0, 0, 0);

	glVertex3f(-0.8, -14.5, -0.8);
	glVertex3f(-2.4, -14.5, -0.8);

	glVertex3f(-0.8, -14.5, 2);
	glVertex3f(-2.4, -14.5, 2);
	//zapato1
	glVertex3f(-0.8, -14.5, -0.8);
	glVertex3f(-0.8, -14.5, 2);

	glVertex3f(-2.4, -14.5, -0.8);
	glVertex3f(-2.4, -14.5, 2);

	glVertex3f(0.8, -14.5, -0.8);
	glVertex3f(2.4, -14.5, -0.8);

	glVertex3f(0.8, -14.5, 2);
	glVertex3f(2.4, -14.5, 2);
	//zapato
	glVertex3f(0.8, -14.5, -0.8);
	glVertex3f(0.8, -14.5, 2);

	glVertex3f(2.4, -14.5, -0.8);
	glVertex3f(2.4, -14.5, 2);

	glEnd();
	glPopMatrix();
}

void Policia()
{
	//cabeza
	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(0, 0, 0);
	glutSolidSphere(2, 100, 100);
	glPopMatrix();

	//ojos

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(-0.8, 0.2, 1.7);
	glutSolidSphere(0.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(-0.8, 0.2, 1.9);
	glutSolidSphere(0.2, 100, 100);
	glPopMatrix();

	//////ojo2

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(0.8, 0.2, 1.7);
	glutSolidSphere(0.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0.8, 0.2, 1.9);
	glutSolidSphere(0.2, 100, 100);
	glPopMatrix();

	//cabello

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0, 0.7, 0);
	glutSolidSphere(1.8, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0, 0, -0.5);
	glutSolidSphere(1.9, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0.8, 0.4, 0);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	//
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(-0.8, 0.4, 0);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(-0.8, 0, 0);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(-0.8, -0.1, 0);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	//

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0.8, 0.4, 0);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0.8, 0, 0);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0.8, -0.1, 0);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	//////cuello
	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(0, 7.5, 0);
	cilindro(0, 0, -8, 30, 1);
	glPopMatrix();

	//insignia

	glPushMatrix();
	glColor3f(0.06, 0.10, 0.15);
	glTranslatef(1, -4, 1);
	glutSolidSphere(0.4, 100, 100);
	glPopMatrix();

	//torax

	glBegin(GL_QUADS);
	glColor3f(0.57, 0.65, 0.75);

	glVertex3f(-2, -2.6, 1);
	glVertex3f(2, -2.6, 1);

	glVertex3f(-2, -2.6, -1);
	glVertex3f(2, -2.6, -1);
	//cuadrado arriba
	glVertex3f(-2, -2.6, 1);
	glVertex3f(-2, -2.6, -1);

	glVertex3f(2, -2.6, 1);
	glVertex3f(2, -2.6, -1);

	glVertex3f(-2, -7.5, 1);
	glVertex3f(2, -7.5, 1);

	glVertex3f(-2, -7.5, -1);
	glVertex3f(2, -7.5, -1);
	//cuadrado abajo
	glVertex3f(-2, -7.5, 1);
	glVertex3f(-2, -7.5, -1);

	glVertex3f(2, -7.5, 1);
	glVertex3f(2, -7.5, -1);

	glVertex3f(-2, -2.6, 1);
	glVertex3f(-2, -7.5, 1);

	glVertex3f(2, -2.6, 1);
	glVertex3f(2, -7.5, 1);
	//cuadrado frente
	glVertex3f(-2, -2.6, 1);
	glVertex3f(2, -2.6, 1);

	glVertex3f(-2, -7.5, 1);
	glVertex3f(2, -7.5, 1);

	glVertex3f(-2, -2.6, -1);
	glVertex3f(-2, -7.5, -1);

	glVertex3f(2, -2.6, -1);
	glVertex3f(2, -7.5, -1);
	//cuadrado atras
	glVertex3f(-2, -2.6, -1);
	glVertex3f(2, -2.6, -1);

	glVertex3f(-2, -7.5, -1);
	glVertex3f(2, -7.5, -1);

	glVertex3f(-2, -2.6, 1);
	glVertex3f(-2, -2.6, -1);

	glVertex3f(-2, -7.5, 1);
	glVertex3f(-2, -7.5, -1);
	//cuadrado lado1
	glVertex3f(-2, -2.6, -1);
	glVertex3f(-2, -7.5, -1);

	glVertex3f(-2, -2.6, 1);
	glVertex3f(-2, -7.5, 1);

	glVertex3f(2, -2.6, 1);
	glVertex3f(2, -2.6, -1);

	glVertex3f(2, -7.5, 1);
	glVertex3f(2, -7.5, -1);
	//cuadrado lado2
	glVertex3f(2, -2.6, -1);
	glVertex3f(2, -7.5, -1);

	glVertex3f(2, -2.6, 1);
	glVertex3f(2, -7.5, 1);

	glEnd();

	//////////////////////////truza
	glColor3f(0.06, 0.10, 0.15);

	glBegin(GL_QUADS);

	glVertex3f(-0.8, -7.5, 1);
	glVertex3f(0.8, -7.5, 1);

	glVertex3f(-0.8, -8.5, 1);
	glVertex3f(0.8, -8.5, 1);
	//CUADRADO FRENTE
	glVertex3f(-0.8, -7.5, 1);
	glVertex3f(-0.8, -8.5, 1);

	glVertex3f(0.8, -7.5, 1);
	glVertex3f(0.8, -8.5, 1);

	glVertex3f(-0.8, -7.5, -1);
	glVertex3f(0.8, -7.5, -1);

	glVertex3f(-0.8, -8.5, -1);
	glVertex3f(0.8, -8.5, -1);
	//CUADRADO ATRAS
	glVertex3f(-0.8, -7.5, -1);
	glVertex3f(-0.8, -8.5, -1);

	glVertex3f(0.8, -7.5, -1);
	glVertex3f(0.8, -8.5, -1);

	glVertex3f(-0.8, -7.5, 1);
	glVertex3f(-0.8, -7.5, -1);

	glVertex3f(-0.8, -8.5, 1);
	glVertex3f(-0.8, -8.5, -1);
	//cuadrado lado1
	glVertex3f(-0.8, -7.5, 1);
	glVertex3f(-0.8, -8.5, 1);

	glVertex3f(-0.8, -7.5, -1);
	glVertex3f(-0.8, -8.5, -1);

	glVertex3f(0.8, -7.5, 1);
	glVertex3f(0.8, -7.5, -1);

	glVertex3f(0.8, -8.5, 1);
	glVertex3f(0.8, -8.5, -1);
	//cuadrado lado2
	glVertex3f(0.8, -7.5, 1);
	glVertex3f(0.8, -8.5, 1);

	glVertex3f(0.8, -7.5, -1);
	glVertex3f(0.8, -8.5, -1);

	glEnd();

	//HOMBROS

	glPushMatrix();
	glColor3f(0.57, 0.65, 0.75);
	glTranslatef(-2.7, -3.2, 0);
	glutSolidSphere(0.7, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.57, 0.65, 0.75);
	glTranslatef(2.7, -3.2, 0);
	glutSolidSphere(0.7, 100, 100);
	glPopMatrix();

	//articulaciones union torax pierna
	glColor3f(0.06, 0.10, 0.15);

	glPushMatrix();

	glTranslatef(1.5, -8.3, 0);
	glutSolidSphere(0.8, 100, 100);
	glPopMatrix();

	glPushMatrix();

	glTranslatef(-1.5, -8.3, 0);
	glutSolidSphere(0.8, 100, 100);
	glPopMatrix();

	//biceps

	glPushMatrix();
	glColor3f(0.57, 0.65, 0.75);
	glTranslatef(2.7, 4.4, 0);
	cilindro(0, 0, -8, 30, 0.5);
	glPopMatrix();

	glPushMatrix();

	glTranslatef(-2.7, 4.4, 0);
	cilindro(0, 0, -8, 30, 0.5);
	glPopMatrix();

	//codos

	glPushMatrix();
	glColor3f(0.57, 0.65, 0.75);
	glTranslatef(-2.7, -5.8, 0);
	glutSolidSphere(0.6, 100, 100);
	glPopMatrix();

	glPushMatrix();

	glTranslatef(2.7, -5.8, 0);
	glutSolidSphere(0.6, 100, 100);
	glPopMatrix();

	//antebrazos

	glPushMatrix();
	glTranslatef(2.7, 2, 0);
	cilindro(0, 0, -8, 30, 0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.7, 2, 0);
	cilindro(0, 0, -8, 30, 0.5);
	glPopMatrix();

	//manos

	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(2.7, -8.4, 0);
	glutSolidSphere(0.6, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(-2.7, -8.4, 0);
	glutSolidSphere(0.6, 100, 100);
	glPopMatrix();

	//cuadricep
	glColor3f(0.06, 0.10, 0.15);
	glPushMatrix();
	glTranslatef(1.5, -0.8, 0);
	cilindro(0, 0, -8, 30, 0.6);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, -0.8, 0);
	cilindro(0, 0, -8, 30, 0.6);
	glPopMatrix();

	//rodillas
	glColor3f(0.06, 0.10, 0.15);

	glPushMatrix();

	glTranslatef(1.5, -11, 0);
	glutSolidSphere(0.7, 100, 100);
	glPopMatrix();

	glPushMatrix();

	glTranslatef(-1.5, -11, 0);
	glutSolidSphere(0.7, 100, 100);
	glPopMatrix();

	//espinilla
	glColor3f(0.06, 0.10, 0.15);
	glPushMatrix();
	glTranslatef(-1.5, -4, 0);
	cilindro(0, 0, -7, 30, 0.6);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, -4, 0);
	cilindro(0, 0, -7, 30, 0.6);
	glPopMatrix();

	glPushMatrix();
	//zapatos

	glBegin(GL_QUADS);

	glColor3f(0, 0, 0);

	glVertex3f(-0.8, -14.5, -0.8);
	glVertex3f(-2.4, -14.5, -0.8);

	glVertex3f(-0.8, -14.5, 2);
	glVertex3f(-2.4, -14.5, 2);
	//zapato1
	glVertex3f(-0.8, -14.5, -0.8);
	glVertex3f(-0.8, -14.5, 2);

	glVertex3f(-2.4, -14.5, -0.8);
	glVertex3f(-2.4, -14.5, 2);

	glVertex3f(0.8, -14.5, -0.8);
	glVertex3f(2.4, -14.5, -0.8);

	glVertex3f(0.8, -14.5, 2);
	glVertex3f(2.4, -14.5, 2);
	//zapato
	glVertex3f(0.8, -14.5, -0.8);
	glVertex3f(0.8, -14.5, 2);

	glVertex3f(2.4, -14.5, -0.8);
	glVertex3f(2.4, -14.5, 2);

	glEnd();
	glPopMatrix();
}

void persona()
{
	//cabeza
	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(0, 0, 0);
	glutSolidSphere(2, 100, 100);
	glPopMatrix();

	//ojos

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(-0.8, 0.2, 1.7);
	glutSolidSphere(0.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(-0.8, 0.2, 1.9);
	glutSolidSphere(0.2, 100, 100);
	glPopMatrix();

	//////ojo2

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(0.8, 0.2, 1.7);
	glutSolidSphere(0.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0.8, 0.2, 1.9);
	glutSolidSphere(0.2, 100, 100);
	glPopMatrix();

	//cabello

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0, 0.7, 0);
	glutSolidSphere(1.8, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0, 0, -0.5);
	glutSolidSphere(1.9, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0.8, 0.4, 0);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	//
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(-0.8, 0.4, 0);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(-0.8, 0, 0);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(-0.8, -0.1, 0);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	//

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0.8, 0.4, 0);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0.8, 0, 0);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0.8, -0.1, 0);
	glutSolidSphere(1.3, 100, 100);
	glPopMatrix();

	//////cuello
	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(0, 7.5, 0);
	cilindro(0, 0, -8, 30, 1);
	glPopMatrix();

	//torax

	glBegin(GL_QUADS);
	glColor3f(1, 0, 0);

	glVertex3f(-2, -2.6, 1);
	glVertex3f(2, -2.6, 1);

	glVertex3f(-2, -2.6, -1);
	glVertex3f(2, -2.6, -1);
	//cuadrado arriba
	glVertex3f(-2, -2.6, 1);
	glVertex3f(-2, -2.6, -1);

	glVertex3f(2, -2.6, 1);
	glVertex3f(2, -2.6, -1);

	glVertex3f(-2, -7.5, 1);
	glVertex3f(2, -7.5, 1);

	glVertex3f(-2, -7.5, -1);
	glVertex3f(2, -7.5, -1);
	//cuadrado abajo
	glVertex3f(-2, -7.5, 1);
	glVertex3f(-2, -7.5, -1);

	glVertex3f(2, -7.5, 1);
	glVertex3f(2, -7.5, -1);

	glVertex3f(-2, -2.6, 1);
	glVertex3f(-2, -7.5, 1);

	glVertex3f(2, -2.6, 1);
	glVertex3f(2, -7.5, 1);
	//cuadrado frente
	glVertex3f(-2, -2.6, 1);
	glVertex3f(2, -2.6, 1);

	glVertex3f(-2, -7.5, 1);
	glVertex3f(2, -7.5, 1);

	glVertex3f(-2, -2.6, -1);
	glVertex3f(-2, -7.5, -1);

	glVertex3f(2, -2.6, -1);
	glVertex3f(2, -7.5, -1);
	//cuadrado atras
	glVertex3f(-2, -2.6, -1);
	glVertex3f(2, -2.6, -1);

	glVertex3f(-2, -7.5, -1);
	glVertex3f(2, -7.5, -1);

	glVertex3f(-2, -2.6, 1);
	glVertex3f(-2, -2.6, -1);

	glVertex3f(-2, -7.5, 1);
	glVertex3f(-2, -7.5, -1);
	//cuadrado lado1
	glVertex3f(-2, -2.6, -1);
	glVertex3f(-2, -7.5, -1);

	glVertex3f(-2, -2.6, 1);
	glVertex3f(-2, -7.5, 1);

	glVertex3f(2, -2.6, 1);
	glVertex3f(2, -2.6, -1);

	glVertex3f(2, -7.5, 1);
	glVertex3f(2, -7.5, -1);
	//cuadrado lado2
	glVertex3f(2, -2.6, -1);
	glVertex3f(2, -7.5, -1);

	glVertex3f(2, -2.6, 1);
	glVertex3f(2, -7.5, 1);

	glEnd();

	//////////////////////////truza
	glColor3f(0.25, 0.42, 0.59);

	glBegin(GL_QUADS);

	glVertex3f(-0.8, -7.5, 1);
	glVertex3f(0.8, -7.5, 1);

	glVertex3f(-0.8, -8.5, 1);
	glVertex3f(0.8, -8.5, 1);
	//CUADRADO FRENTE
	glVertex3f(-0.8, -7.5, 1);
	glVertex3f(-0.8, -8.5, 1);

	glVertex3f(0.8, -7.5, 1);
	glVertex3f(0.8, -8.5, 1);

	glVertex3f(-0.8, -7.5, -1);
	glVertex3f(0.8, -7.5, -1);

	glVertex3f(-0.8, -8.5, -1);
	glVertex3f(0.8, -8.5, -1);
	//CUADRADO ATRAS
	glVertex3f(-0.8, -7.5, -1);
	glVertex3f(-0.8, -8.5, -1);

	glVertex3f(0.8, -7.5, -1);
	glVertex3f(0.8, -8.5, -1);

	glVertex3f(-0.8, -7.5, 1);
	glVertex3f(-0.8, -7.5, -1);

	glVertex3f(-0.8, -8.5, 1);
	glVertex3f(-0.8, -8.5, -1);
	//cuadrado lado1
	glVertex3f(-0.8, -7.5, 1);
	glVertex3f(-0.8, -8.5, 1);

	glVertex3f(-0.8, -7.5, -1);
	glVertex3f(-0.8, -8.5, -1);

	glVertex3f(0.8, -7.5, 1);
	glVertex3f(0.8, -7.5, -1);

	glVertex3f(0.8, -8.5, 1);
	glVertex3f(0.8, -8.5, -1);
	//cuadrado lado2
	glVertex3f(0.8, -7.5, 1);
	glVertex3f(0.8, -8.5, 1);

	glVertex3f(0.8, -7.5, -1);
	glVertex3f(0.8, -8.5, -1);

	glEnd();

	//HOMBROS

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(-2.7, -3.2, 0);
	glutSolidSphere(0.7, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(2.7, -3.2, 0);
	glutSolidSphere(0.7, 100, 100);
	glPopMatrix();

	//articulaciones union torax pierna
	glColor3f(0.25, 0.42, 0.59);

	glPushMatrix();

	glTranslatef(1.5, -8.3, 0);
	glutSolidSphere(0.8, 100, 100);
	glPopMatrix();

	glPushMatrix();

	glTranslatef(-1.5, -8.3, 0);
	glutSolidSphere(0.8, 100, 100);
	glPopMatrix();

	//biceps

	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(2.7, 4.4, 0);
	cilindro(0, 0, -8, 30, 0.5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(-2.7, 4.4, 0);
	cilindro(0, 0, -8, 30, 0.5);
	glPopMatrix();

	//codos

	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(-2.7, -5.8, 0);
	glutSolidSphere(0.6, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(2.7, -5.8, 0);
	glutSolidSphere(0.6, 100, 100);
	glPopMatrix();

	//antebrazos

	glPushMatrix();
	glTranslatef(2.7, 2, 0);
	cilindro(0, 0, -8, 30, 0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.7, 2, 0);
	cilindro(0, 0, -8, 30, 0.5);
	glPopMatrix();

	//manos

	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(2.7, -8.4, 0);
	glutSolidSphere(0.6, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.91, 0.76, 0.47); //color carne
	glTranslatef(-2.7, -8.4, 0);
	glutSolidSphere(0.6, 100, 100);
	glPopMatrix();

	//cuadricep
	glColor3f(0.25, 0.42, 0.59);

	glPushMatrix();
	glTranslatef(1.5, -0.8, 0);
	cilindro(0, 0, -8, 30, 0.6);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, -0.8, 0);
	cilindro(0, 0, -8, 30, 0.6);
	glPopMatrix();

	//rodillas
	glColor3f(0.25, 0.42, 0.59);

	glPushMatrix();

	glTranslatef(1.5, -11, 0);
	glutSolidSphere(0.7, 100, 100);
	glPopMatrix();

	glPushMatrix();

	glTranslatef(-1.5, -11, 0);
	glutSolidSphere(0.7, 100, 100);
	glPopMatrix();

	//espinilla
	glColor3f(0.25, 0.42, 0.59);
	glPushMatrix();
	glTranslatef(-1.5, -4, 0);
	cilindro(0, 0, -7, 30, 0.6);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, -4, 0);
	cilindro(0, 0, -7, 30, 0.6);
	glPopMatrix();

	glPushMatrix();
	//zapatos

	glBegin(GL_QUADS);

	glColor3f(0, 0, 0);

	glVertex3f(-0.8, -14.5, -0.8);
	glVertex3f(-2.4, -14.5, -0.8);

	glVertex3f(-0.8, -14.5, 2);
	glVertex3f(-2.4, -14.5, 2);
	//zapato1
	glVertex3f(-0.8, -14.5, -0.8);
	glVertex3f(-0.8, -14.5, 2);

	glVertex3f(-2.4, -14.5, -0.8);
	glVertex3f(-2.4, -14.5, 2);

	glVertex3f(0.8, -14.5, -0.8);
	glVertex3f(2.4, -14.5, -0.8);

	glVertex3f(0.8, -14.5, 2);
	glVertex3f(2.4, -14.5, 2);
	//zapato
	glVertex3f(0.8, -14.5, -0.8);
	glVertex3f(0.8, -14.5, 2);

	glVertex3f(2.4, -14.5, -0.8);
	glVertex3f(2.4, -14.5, 2);

	glEnd();
	glPopMatrix();
}

void E_redes()
{

	edificio_redes();

	glPushMatrix();
	glTranslatef(110, 0, 0);
	edificio_redes();
	glPopMatrix();

	glBegin(GL_QUADS);
	glColor3f(0.6, 0, 0);

	glVertex3f(-4, 15, -15);
	glVertex3f(50, 15, -15);

	glVertex3f(-4, 15, -79);
	glVertex3f(50, 15, -79);
	//cuadrado arriba
	glVertex3f(-4, 15, -15);
	glVertex3f(-4, 15, -79);

	glVertex3f(50, 15, -15);
	glVertex3f(50, 15, -79);

	glVertex3f(-4, 0, -15);
	glVertex3f(-4, 15, -15);

	glVertex3f(50, 0, -15);
	glVertex3f(50, 15, -15);
	//cuadrado frente
	glVertex3f(-4, 0, -15);
	glVertex3f(50, 0, -15);

	glVertex3f(-4, 15, -15);
	glVertex3f(50, 15, -15);

	glVertex3f(-4, 0, -79);
	glVertex3f(-4, 15, -79);

	glVertex3f(50, 0, -79);
	glVertex3f(50, 15, -79);
	//cuadrado atras
	glVertex3f(-4, 0, -79);
	glVertex3f(50, 0, -79);

	glVertex3f(-4, 15, -79);
	glVertex3f(50, 15, -79);

	glEnd();
}

void autobus()
{

	glColor4f(1, 1, 1, 1);
	glBegin(GL_QUADS);

	glVertex3f(-36, 5, -2);
	glVertex3f(4, 5, -2);

	glVertex3f(-36, -8, -2);
	glVertex3f(4, -8, -2);
	//cara enfrente
	glVertex3f(-36, 5, -2);
	glVertex3f(-36, -8, -2);

	glVertex3f(4, 5, -2);
	glVertex3f(4, -8, -2);

	glVertex3f(-36, 5, -13);
	glVertex3f(4, 5, -13);

	glVertex3f(-36, -8, -13);
	glVertex3f(4, -8, -13);
	//cara atras
	glVertex3f(-36, 5, -13);
	glVertex3f(-36, -8, -13);

	glVertex3f(4, 5, -13);
	glVertex3f(4, -8, -13);

	glVertex3f(-36, 5, -2);
	glVertex3f(-36, 5, -13);

	glVertex3f(4, 5, -2);
	glVertex3f(4, 5, -13);
	//cara arriba
	glVertex3f(-36, 5, -2);
	glVertex3f(4, 5, -2);

	glVertex3f(-36, 5, -13);
	glVertex3f(4, 5, -13);

	glVertex3f(-36, -8, -2);
	glVertex3f(-36, -8, -13);

	glVertex3f(4, -8, -2);
	glVertex3f(4, -8, -13);
	//cara abajo
	glVertex3f(-36, -8, -2);
	glVertex3f(4, -8, -2);

	glVertex3f(-36, -8, -13);
	glVertex3f(4, -8, -13);

	glVertex3f(-36, 5, -2);
	glVertex3f(-36, 5, -13);

	glVertex3f(-36, -8, -2);
	glVertex3f(-36, -8, -13);
	//cara lado derecho
	glVertex3f(-36, 5, -2);
	glVertex3f(-36, -8, -2);

	glVertex3f(-36, 5, -13);
	glVertex3f(-36, -8, -13);

	glVertex3f(4, 5, -2);
	glVertex3f(4, 5, -13);

	glVertex3f(4, -8, -2);
	glVertex3f(4, -8, -13);
	//cara lado izquierdo
	glVertex3f(4, 5, -2);
	glVertex3f(4, -8, -2);

	glVertex3f(4, 5, -13);
	glVertex3f(4, -8, -13);

	glEnd();

	//////////////////////////////////////////////////

	glBegin(GL_POLYGON);

	glVertex3f(4, 5, -2);
	glVertex3f(4, -8, -2);

	glVertex3f(4, 5, -2);
	glVertex3f(13, -3, -2);
	//triangulo lado1
	glVertex3f(13, -3, -2);
	glVertex3f(13, -8, -2);

	glVertex3f(4, -8, -2);
	glVertex3f(13, -8, -2);

	glEnd();

	glBegin(GL_POLYGON);

	glVertex3f(4, 5, -13);
	glVertex3f(4, -8, -13);

	glVertex3f(4, 5, -13);
	glVertex3f(13, -3, -13);
	//triangulo lado2
	glVertex3f(13, -3, -13);
	glVertex3f(13, -8, -13);

	glVertex3f(4, -8, -13);
	glVertex3f(13, -8, -13);

	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(4, 5, -2);
	glVertex3f(4, 5, -13);

	glVertex3f(4, -8, -2);
	glVertex3f(4, -8, -13);
	//cuadrado lado1
	glVertex3f(4, 5, -2);
	glVertex3f(4, -8, -2);

	glVertex3f(4, 5, -13);
	glVertex3f(4, -8, -13);

	glVertex3f(4, -8, -2);
	glVertex3f(4, -8, -13);

	glVertex3f(13, -8, -2);
	glVertex3f(13, -8, -13);
	//cuadrado abajo
	glVertex3f(4, -8, -2);
	glVertex3f(13, -8, -2);

	glVertex3f(4, -8, -13);
	glVertex3f(13, -8, -13);

	glVertex3f(13, -3, -2);
	glVertex3f(13, -3, -13);

	glVertex3f(13, -8, -2);
	glVertex3f(13, -8, -13);
	//cuadrado lado2
	glVertex3f(13, -3, -2);
	glVertex3f(13, -8, -2);

	glVertex3f(13, -3, -13);
	glVertex3f(13, -8, -13);

	glVertex3f(4, 5, -2);
	glVertex3f(4, 5, -13);

	glVertex3f(13, -3, -2);
	glVertex3f(13, -3, -13);
	//cuadrado enfrente
	glVertex3f(4, 5, -2);
	glVertex3f(13, -3, -2);

	glVertex3f(4, 5, -13);
	glVertex3f(13, -3, -13);

	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0, 0, 0);

	glVertex3f(-27, 3, -1.9);
	glVertex3f(-2.5, 3, -1.9);

	glVertex3f(-27, -2.7, -1.9);
	glVertex3f(-2.5, -2.7, -1.9);

	glVertex3f(-27, 3, -1.9);
	glVertex3f(-27, -2.7, -1.9);
	//ventanas
	glVertex3f(-2.5, 3, -1.9);
	glVertex3f(-2.5, -2.7, -1.9);

	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0, 0, 0);

	glVertex3f(-29.5, 3, -13.1);
	glVertex3f(2.1, 3, -13.1);

	glVertex3f(-29.5, -2.7, -13.1);
	glVertex3f(2.1, -2.7, -13.1);

	glVertex3f(-29.5, 3, -13.1);
	glVertex3f(-29.5, -2.7, -13.1);
	//ventanas atras
	glVertex3f(2.1, 3, -13.1);
	glVertex3f(2.1, -2.7, -13.1);

	glEnd();

	/*
	int i,j;
	
	for(i=3;i<30;i++)
	{
		glColor3f(0,0,0);
		if(j==6)
		{
			j=0;
			
			glBegin(GL_QUADS);
		
			glVertex3f(-i+5.7,3,-1.9);
			glVertex3f(-i,3,-1.9);
			
			glVertex3f(-i+5.7,-2.7,-1.9);
			glVertex3f(-i,-2.7,-1.9);
			
			glVertex3f(-i+5.7,3,-1.9);
			glVertex3f(-i+5.7,-2.7,-1.9);
											//ventanas 
			glVertex3f(-i,3,-1.9);
			glVertex3f(-i,-2.7,-1.9);
			
			glEnd();
			
			glBegin(GL_QUADS);
	
			glVertex3f(-i+5.7,3,-13.1);
			glVertex3f(-i,3,-13.1);
										//ventanas atras
			glVertex3f(-i+5.7,-2.7,-13.1);
			glVertex3f(-i,-2.7,-13.1);
			
			glVertex3f(-i+5.7,3,-13.1);
			glVertex3f(-i+5.7,-2.7,-13.1);
			
			glVertex3f(-i,3,-13.1);
			glVertex3f(-i,-2.7,-13.1);
			
			glEnd();
		}
		j++;
	
	}
	
	*/

	glBegin(GL_QUADS);
	glColor3f(0, 0, 0);

	glVertex3f(-35, 3, -1.9);
	glVertex3f(-32, 3, -1.9);

	glVertex3f(-32, 3, -1.9);
	glVertex3f(-32, -2.7, -1.9);

	glVertex3f(-35, 3, -1.9);
	glVertex3f(-35, -2.7, -1.9);
	//ventana atras1
	glVertex3f(-35, -2.7, -1.9);
	glVertex3f(-32, -2.7, -1.9);

	glEnd();

	glBegin(GL_QUADS);

	glVertex3f(-35, 3, -13.1);
	glVertex3f(-30, 3, -13.1);

	glVertex3f(-30, 3, -13.1);
	glVertex3f(-30, -2.7, -13.1);

	glVertex3f(-35, 3, -13.1);
	glVertex3f(-35, -2.7, -13.1);
	//ventana atras2
	glVertex3f(-35, -2.7, -13.1);
	glVertex3f(-30, -2.7, -13.1);

	glEnd();

	glBegin(GL_QUADS);

	glVertex3f(-31.5, 3, -1.9);
	glVertex3f(-27.5, 3, -1.9);

	glVertex3f(-31.5, -7, -1.9);
	glVertex3f(-27.5, -7, -1.9);

	glVertex3f(-31.5, 3, -1.9);
	glVertex3f(-31.5, -7, -1.9);
	//puerta atraS
	glVertex3f(-27.5, 3, -1.9);
	glVertex3f(-27.5, -7, -1.9);

	glEnd();

	glBegin(GL_QUADS);

	glVertex3f(-2, 3, -1.9);
	glVertex3f(2, 3, -1.9);

	glVertex3f(-2, -7, -1.9);
	glVertex3f(2, -7, -1.9);

	glVertex3f(-2, 3, -1.9);
	glVertex3f(-2, -7, -1.9);
	//puerta adelante
	glVertex3f(2, 3, -1.9);
	glVertex3f(2, -7, -1.9);

	glEnd();

	glBegin(GL_POLYGON);

	glVertex3f(2.6, 3, -1.9);
	glVertex3f(2.6, -2.7, -1.9);

	glVertex3f(2.6, 3, -1.9);
	glVertex3f(4.3, 3, -1.9);
	//triangulo enfrente
	glVertex3f(2.6, -2.7, -1.9);
	glVertex3f(10, -2.7, -1.9);

	glVertex3f(2.6, 3, -1.9);
	glVertex3f(2.6, -2.7, -1.9);

	glVertex3f(4.5, 3, -1.9);
	glVertex3f(10, -2.7, -1.9);

	glEnd();

	glBegin(GL_POLYGON);

	glVertex3f(2.6, 3, -13.1);
	glVertex3f(2.6, -2.7, -13.1);

	glVertex3f(2.6, 3, -13.1);
	glVertex3f(4.3, 3, -13.1);
	//triangulo enfrente
	glVertex3f(2.6, -2.7, -13.1);
	glVertex3f(10, -2.7, -13.1);

	glVertex3f(2.6, 3, -13.1);
	glVertex3f(2.6, -2.7, -13.1);

	glVertex3f(4.5, 3, -13.1);
	glVertex3f(10, -2.7, -13.1);

	glEnd();

	glBegin(GL_QUADS);

	glColor3f(0, 0, 0);

	glVertex3f(5.8, 3.4, -2.3);
	glVertex3f(5.8, 3.4, -12.7);

	glVertex3f(12, -2, -2.3);
	glVertex3f(12, -2, -12.7);
	//cuadrado enfrente
	glVertex3f(5.8, 3.4, -2.3);
	glVertex3f(12, -2, -2.3);

	glVertex3f(5.8, 3.4, -12.7);
	glVertex3f(12, -2, -12.7);

	glEnd();

	//lineas

	glBegin(GL_LINES);

	glColor3f(0, 0, 0);
	glLineWidth(3);
	glVertex3f(-19.6, -6, -1);
	glVertex3f(-2.5, -6, -1);

	glVertex3f(-20, -6, -13.1);
	glVertex3f(2, -6, -13.1);

	glEnd();

	/////parilla y focos

	glBegin(GL_QUADS);
	glColor3f(0, 0, 0);

	glVertex3f(13, -4, -3);
	glVertex3f(13, -4, -12);

	glVertex3f(13, -6.5, -5);
	glVertex3f(13, -6.5, -10);

	glVertex3f(13, -4, -3);
	glVertex3f(13, -6.5, -5);

	glVertex3f(13, -4, -12);
	glVertex3f(13, -6.5, -10);

	glEnd();

	glPushMatrix();

	glTranslatef(12.8, -5.4, -7.5d);
	glColor3f(1, 1, 1);
	glutSolidSphere(0.7, 100, 100);

	glPopMatrix();

	glPushMatrix();

	glTranslatef(12, -6.5, -12);
	glColor3f(0.96, 0.96, 0.10);
	glutSolidSphere(1, 100, 100);

	glPopMatrix();

	glPushMatrix();

	glTranslatef(12, -6.5, -3);
	glColor3f(0.96, 0.96, 0.10);
	glutSolidSphere(1, 100, 100);

	glPopMatrix();

	/////llantas

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(5.6, -8, -3);
	glutSolidTorus(1.2, 2.9, 30.0, 50.0);
	glPopMatrix();

	glPushMatrix();
	llanta(5.6, -8, -1.9);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(5.6, -8, -12);
	glutSolidTorus(1.2, 2.9, 30.0, 50.0);
	glPopMatrix();

	glPushMatrix();
	llanta(5.6, -8, -13.1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(-24, -8, -3);
	glutSolidTorus(1.2, 2.9, 30.0, 50.0);
	glPopMatrix();

	glPushMatrix();
	llanta(-24, -8, -1.9);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(-24, -8, -12);
	glutSolidTorus(1.2, 2.9, 30.0, 50.0);
	glPopMatrix();

	glPushMatrix();
	llanta(-24, -8, -13.1);
	glPopMatrix();
}

void banqueta()
{
	glColor3f(1, 1, 0.2);
	glLineWidth(3.0);

	glBegin(GL_LINES);

	glVertex3f(-40, -6, 0);
	glVertex3f(-200, -6, 0);

	glVertex3f(-40, -6, 0);
	glVertex3f(-40, -6, -8);
	//cara abajo
	glVertex3f(-200, -6, 0);
	glVertex3f(-200, -6, -8);

	glVertex3f(-40, -6, -8);
	glVertex3f(-200, -6, -8);

	glEnd();

	glColor3f(0.2, 0.2, 0.2);

	glBegin(GL_QUADS);

	glVertex3f(-40, -7.5, 0);
	glVertex3f(-200, -7.5, 0);

	glVertex3f(-40, -7.5, 0);
	glVertex3f(-40, -7.5, -8);
	//cara abajo
	glVertex3f(-200, -7.5, 0);
	glVertex3f(-200, -7.5, -8);

	glVertex3f(-40, -7.5, -8);
	glVertex3f(-200, -7.5, -8);

	glVertex3f(-40, -6, 0);
	glVertex3f(-40, -6, -8);

	glVertex3f(-200, -6, 0);
	glVertex3f(-200, -6, -8);
	//cara arriba
	glVertex3f(-40, -6, 0);
	glVertex3f(-200, -6, 0);

	glVertex3f(-40, -6, -8);
	glVertex3f(-200, -6, -8);

	glVertex3f(-40, -6, 0);
	glVertex3f(-40, -7.5, 0);

	glVertex3f(-200, -6, 0);
	glVertex3f(-200, -7.5, 0);
	//cara enfrente
	glVertex3f(-40, -6, 0);
	glVertex3f(-200, -6, 0);

	glVertex3f(-40, -7.5, 0);
	glVertex3f(-200, -7.5, 0);

	glVertex3f(-40, -6, -8);
	glVertex3f(-40, -7.5, 0 - 8);

	glVertex3f(-200, -6, 0 - 8);
	glVertex3f(-200, -7.5, -8);
	//cara atras
	glVertex3f(-40, -6, -8);
	glVertex3f(-200, -6, -8);

	glVertex3f(-40, -7.5, -8);
	glVertex3f(-200, -7.5, -8);

	glVertex3f(-40, -6, 0);
	glVertex3f(-40, -7.5, 0);

	glVertex3f(-40, -6, -8);
	glVertex3f(-40, -7.5, -8);
	//cara lado derecho
	glVertex3f(-40, -6, 0);
	glVertex3f(-40, -6, -8);

	glVertex3f(-40, -7.5, 0);
	glVertex3f(-40, -7.5, -8);

	glVertex3f(-200, -6, 0);
	glVertex3f(-200, -7.5, 0);

	glVertex3f(-200, -6, -8);
	glVertex3f(-200, -7.5, -8);
	//cara lado izquierdo
	glVertex3f(-200, -6, 0);
	glVertex3f(-200, -6, -8);

	glVertex3f(-200, -7.5, 0);
	glVertex3f(-200, -7.5, -8);

	glEnd();
}

void suelo()
{

	glColor3f(0.37, 0.37, 0.37);

	glBegin(GL_QUADS);

	glVertex3f(-1000, -11, -1000);
	glVertex3f(1000, -11, -1000);

	glVertex3f(-1000, -11, -1000);
	glVertex3f(-1000, -11, 2000);

	glVertex3f(-1000, -11, 2000);
	glVertex3f(1000, -11, 2000);

	glVertex3f(1000, -11, 2000);
	glVertex3f(1000, -11, -1000);

	glEnd();
}

void cerca()
{
	int i, j;

	glColor3f(0, 0, 0);

	glLineWidth(3.0);

	glBegin(GL_LINES);

	glVertex3f(-45, 2, -2);
	glVertex3f(-200, 2, -2);

	glVertex3f(-45, -8, -2);
	glVertex3f(-200, -8, -2);

	glVertex3f(-45, 2, -2);
	glVertex3f(-45, -8, -2);

	glVertex3f(-200, 2, -2);
	glVertex3f(-200, -8, -2);

	glEnd();

	for (i = 46; i < 201; i++)
	{

		glColor3f(0.62, 0.62, 0.62);
		glLineWidth(1.0);
		glBegin(GL_LINES);

		if (i < 55)
		{
			int x = 2, y = 0, k;

			for (k = 47; k <= 54; k++)
			{
				glVertex3f(-k, 2, -2);
				glVertex3f(-k + x, -y, -2);

				x++;
				y++;
			}
		}
		else
		{
			glVertex3f(-i, 2, -2);
			glVertex3f(-i + 10, -8, -2);
		}

		glEnd();

		if (j == 16)
		{
			j = 0;
			glColor3f(0, 0, 0);
			glLineWidth(5.0);
			glBegin(GL_LINES);
			glVertex3f(-i, 2, -2);
			glVertex3f(-i, -8, -2);
			glEnd();
		}
		j++;
	}

	for (i = 36; i < 191; i++)
	{
		glColor3f(0.62, 0.62, 0.62);
		glLineWidth(1.0);
		glBegin(GL_LINES);

		if (i < 45)
		{

			int k, y = -7;

			for (k = 36; k <= 44; k++)
			{
				glVertex3f(-45, y, -2);
				glVertex3f(-k - 10, -8, -2);

				y++;
			}
		}
		else
		{
			glVertex3f(-i, 2, -2);
			glVertex3f(-i - 10, -8, -2);
		}
	}

	int x = 9, y = 7, k;

	for (k = 191; k <= 200; k++)
	{
		glVertex3f(-k, 2, -2);
		glVertex3f(-k - x, -y, -2);

		x--;
		y--;
	}

	int k1, y1 = 1;

	for (k1 = 201; k1 <= 208; k1++)
	{
		glVertex3f(-200, y1, -2);
		glVertex3f(-k1 + 10, -8, -2);

		y1--;
	}

	glEnd();
}

void rejas()
{
	int i;

	glColor3f(0, 0, 0.4);

	glLineWidth(3.0);

	glBegin(GL_LINES);

	glVertex3f(-40, 1, -1);
	glVertex3f(-5, 1, -1);

	glVertex3f(-40, -7.5, -1);
	glVertex3f(-5, -7.5, -1);

	glVertex3f(-40, 1, -1);
	glVertex3f(-40, -7.5, -1);

	glVertex3f(-5, 1, -1);
	glVertex3f(-5, -7.5, -1);

	glEnd();

	//cruz
	glLineWidth(5.0);
	glBegin(GL_LINES);

	glVertex3f(-40, -3, -1);
	glVertex3f(-5, -3, -1);

	glVertex3f(-22, 1, -1);
	glVertex3f(-22, -7.5, -1);

	glEnd();

	for (i = 40; i > 4; i--)
	{

		glLineWidth(1.0);
		glBegin(GL_LINES);

		glVertex3f(-i, 1, -1);
		glVertex3f(-i, -7.5, -1);

		glEnd();
	}
}

void rectangulo()
{

	glColor3f(0, 0, 0.4);

	glBegin(GL_QUADS);

	glVertex3f(-42, 9.5, 0);
	glVertex3f(-5, 9.5, 0);

	glVertex3f(-42, 3, 0);
	glVertex3f(-5, 3, 0);
	//CARA ENFRENTE
	glVertex3f(-42, 9.5, 0);
	glVertex3f(-42, 3, 0);

	glVertex3f(-5, 9.5, 0);
	glVertex3f(-5, 3, 0);

	glVertex3f(-42, 9.5, -5);
	glVertex3f(-5, 9.5, -5);

	glVertex3f(-42, 3, -5);
	glVertex3f(-5, 3, -5);
	//CARA atras
	glVertex3f(-42, 9.5, -5);
	glVertex3f(-42, 3, -5);

	glVertex3f(-5, 9.5, -5);
	glVertex3f(-5, 3, -5);

	glVertex3f(-5, 9.5, 0);
	glVertex3f(-5, 9.5, -5);

	glVertex3f(-5, 3, 0);
	glVertex3f(-5, 3, -5);
	//LADO DERECHO
	glVertex3f(-5, 9.5, 0);
	glVertex3f(-5, 3, 0);

	glVertex3f(-5, 9.5, -5);
	glVertex3f(-5, 3, -5);

	glVertex3f(-42, 9.5, 0);
	glVertex3f(-42, 9.5, -5);

	glVertex3f(-42, 3, 0);
	glVertex3f(-42, 3, -5);
	//LADO IZQUIERDO
	glVertex3f(-42, 9.5, 0);
	glVertex3f(-42, 3, 0);

	glVertex3f(-42, 9.5, -5);
	glVertex3f(-42, 3, -5);

	glVertex3f(-42, 9.5, -5);
	glVertex3f(-42, 3, -5);

	glVertex3f(-5, 9.5, -5);
	glVertex3f(-5, 3, -5);
	//ATRAS
	glVertex3f(-5, 9.5, -5);
	glVertex3f(-5, 9.5, 0);

	glVertex3f(-5, 3, -5);
	glVertex3f(-5, 3, 0);

	glVertex3f(-42, 3, 0);
	glVertex3f(-42, 3, -5);

	glVertex3f(-5, 3, 0);
	glVertex3f(-5, 3, -5);
	//ABAJO
	glVertex3f(-42, 3, 0);
	glVertex3f(-5, 3, 0);

	glVertex3f(-42, 3, -5);
	glVertex3f(-5, 3, -5);

	glEnd();

	int x1, x2, i;
	x1 = -39;
	x2 = -35;

	for (i = 1; i < 5; i++)
	{

		glColor3f(0.6, 0, 0);

		glBegin(GL_QUADS);

		glVertex3f(x1, 13, 1);
		glVertex3f(x2, 13, 1);

		glVertex3f(x1, 7.5, 1);
		glVertex3f(x2, 7.5, 1);
		//CARA ENFRENTE
		glVertex3f(x1, 13, 1);
		glVertex3f(x1, 7.5, 1);

		glVertex3f(x2, 13, 1);
		glVertex3f(x2, 7.5, 1);

		glVertex3f(x2, 13, 1);
		glVertex3f(x2, 13, -6);

		glVertex3f(x2, 7.5, 1);
		glVertex3f(x2, 7.5, -6);
		//CARA lado derecho
		glVertex3f(x2, 13, 1);
		glVertex3f(x2, 7.5, 1);

		glVertex3f(x2, 13, -6);
		glVertex3f(x2, 7.5, -6);

		glVertex3f(x1, 13, 1);
		glVertex3f(x1, 13, -6);

		glVertex3f(x1, 7.5, 1);
		glVertex3f(x1, 7.5, -6);
		//CARA lado izquierdo
		glVertex3f(x1, 13, 1);
		glVertex3f(x1, 7.5, 1);

		glVertex3f(x1, 13, -6);
		glVertex3f(x1, 7.5, -6);

		glVertex3f(x1, 13, -6);
		glVertex3f(x2, 13, -6);

		glVertex3f(x1, 7.5, -6);
		glVertex3f(x2, 7.5, -6);
		//CARA atras
		glVertex3f(x1, 13, -6);
		glVertex3f(x1, 7.5, -6);

		glVertex3f(x2, 13, -6);
		glVertex3f(x2, 7.5, -6);

		glVertex3f(x1, 7.5, 1);
		glVertex3f(x2, 7.5, 1);

		glVertex3f(x1, 7.5, -6);
		glVertex3f(x2, 7.5, -6);
		//CARA abajo
		glVertex3f(x1, 7.5, 1);
		glVertex3f(x1, 7.5, -6);

		glVertex3f(x2, 7.5, 1);
		glVertex3f(x2, 7.5, -6);

		glEnd();

		x1 = x1 + 9;
		x2 = x1 + 4;
	}
}

void paradero()
{
	glPushMatrix();

	glColor3f(0.5, 0.5, 0.5);
	glTranslatef(0, 0, 0);
	cilindro(0, 0, 10, 30, 1.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(30, 0, 0);
	cilindro(0, 0, 10, 30, 1.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(30, 0, -40);
	cilindro(0, 0, 10, 30, 1.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, -40);
	cilindro(0, 0, 10, 30, 1.5);
	glPopMatrix();

	glBegin(GL_QUADS);

	glColor3f(0.83, 0.80, 0.80);

	glVertex3f(-10, 10, 5);
	glVertex3f(40, 10, 5);

	glVertex3f(-10, 10, -45);
	glVertex3f(40, 10, -45);
	//cuadrado abajo
	glVertex3f(-10, 10, 5);
	glVertex3f(-10, 10, -45);

	glVertex3f(40, 10, 5);
	glVertex3f(40, 10, -45);

	glVertex3f(-10, 11.5, 5);
	glVertex3f(40, 11.5, 5);

	glVertex3f(-10, 11.5, -45);
	glVertex3f(40, 11.5, -45);
	//cuadrado arriba
	glVertex3f(-10, 11.5, 5);
	glVertex3f(-10, 11.5, -45);

	glVertex3f(40, 11.5, 5);
	glVertex3f(40, 11.5, -45);

	glVertex3f(-10, 10, 5);
	glVertex3f(-10, 11.5, 5);

	glVertex3f(-10, 10, -45);
	glVertex3f(-10, 11.5, -45);
	//cuadrado frente
	glVertex3f(-10, 11.5, 5);
	glVertex3f(-10, 11.5, -45);

	glVertex3f(-10, 10, 5);
	glVertex3f(-10, 10, -45);

	glVertex3f(40, 10, 5);
	glVertex3f(40, 11.5, 5);

	glVertex3f(40, 10, -45);
	glVertex3f(40, 11.5, -45);
	//cuadrado atras
	glVertex3f(40, 11.5, 5);
	glVertex3f(40, 11.5, -45);

	glVertex3f(40, 10, 5);
	glVertex3f(40, 10, -45);

	glVertex3f(-10, 10, 5);
	glVertex3f(-10, 11.5, 5);

	glVertex3f(40, 10, 5);
	glVertex3f(40, 11.5, 5);
	//cuadrado lado1
	glVertex3f(-10, 10, 5);
	glVertex3f(40, 10, 5);

	glVertex3f(-10, 11.5, 5);
	glVertex3f(40, 11.5, 5);

	glVertex3f(-10, 10, -45);
	glVertex3f(-10, 11.5, -45);

	glVertex3f(40, 10, -45);
	glVertex3f(40, 11.5, -45);
	//cuadrado lado1
	glVertex3f(-10, 10, -45);
	glVertex3f(40, 10, -45);

	glVertex3f(-10, 11.5, -45);
	glVertex3f(40, 11.5, -45);

	glEnd();

	glBegin(GL_LINES);

	glColor3f(0, 0, 0);

	glVertex3f(-10, 10, 5);
	glVertex3f(40, 10, 5);

	glVertex3f(-10, 10, -45);
	glVertex3f(40, 10, -45);
	//cuadrado abajo
	glVertex3f(-10, 10, 5);
	glVertex3f(-10, 10, -45);

	glVertex3f(40, 10, 5);
	glVertex3f(40, 10, -45);

	glVertex3f(-10, 11.5, 5);
	glVertex3f(40, 11.5, 5);

	glVertex3f(-10, 11.5, -45);
	glVertex3f(40, 11.5, -45);
	//cuadrado arriba
	glVertex3f(-10, 11.5, 5);
	glVertex3f(-10, 11.5, -45);

	glVertex3f(40, 11.5, 5);
	glVertex3f(40, 11.5, -45);

	glVertex3f(-10, 10, 5);
	glVertex3f(-10, 11.5, 5);

	glVertex3f(-10, 10, -45);
	glVertex3f(-10, 11.5, -45);
	//cuadrado frente
	glVertex3f(-10, 11.5, 5);
	glVertex3f(-10, 11.5, -45);

	glVertex3f(-10, 10, 5);
	glVertex3f(-10, 10, -45);

	glVertex3f(40, 10, 5);
	glVertex3f(40, 11.5, 5);

	glVertex3f(40, 10, -45);
	glVertex3f(40, 11.5, -45);
	//cuadrado atras
	glVertex3f(40, 11.5, 5);
	glVertex3f(40, 11.5, -45);

	glVertex3f(40, 10, 5);
	glVertex3f(40, 10, -45);

	glVertex3f(-10, 10, 5);
	glVertex3f(-10, 11.5, 5);

	glVertex3f(40, 10, 5);
	glVertex3f(40, 11.5, 5);
	//cuadrado lado1
	glVertex3f(-10, 10, 5);
	glVertex3f(40, 10, 5);

	glVertex3f(-10, 11.5, 5);
	glVertex3f(40, 11.5, 5);

	glVertex3f(-10, 10, -45);
	glVertex3f(-10, 11.5, -45);

	glVertex3f(40, 10, -45);
	glVertex3f(40, 11.5, -45);
	//cuadrado lado1
	glVertex3f(-10, 10, -45);
	glVertex3f(40, 10, -45);

	glVertex3f(-10, 11.5, -45);
	glVertex3f(40, 11.5, -45);

	glEnd();
}

void Entrada()
{

	glColor3f(0.5, 0.5, 0.5);
	cilindro(0, 0, 10, 30, 5);

	//////////ventana

	glColor3f(0.2, .06, 1);

	glBegin(GL_QUADS);

	glVertex3f(-4, 2, 5);
	glVertex3f(-1, 2, 5);

	glVertex3f(-4, -5, 5);
	glVertex3f(-1, -5, 5);
	;

	glVertex3f(-4, 2, 5);
	glVertex3f(-4, -5, 5);

	glVertex3f(-1, 2, 5);
	glVertex3f(-1, -5, 5);
	;

	glEnd();

	///rejas derechas
	rejas();

	//cerca derecha

	cerca();

	//////////rectangulo izquierdo

	rectangulo();

	//////////rectangulo derecho

	glRotatef(-90, 0, 1, 0);
	glTranslatef(45, 0, 0);

	rectangulo();
	//rejas izquierda

	rejas();

	//cilindro derecho
	glColor3f(0.5, 0.5, 0.5);
	glTranslatef(-7, 0, -3);

	cilindro(3, 0, 10, 30, 2);

	// cilindro izquierdo

	glColor3f(0.5, 0.5, 0.5);
	glTranslatef(-45, 0, 46);

	cilindro(3, 0, 10, 30, 2);

	//cerca izquierda
	glRotatef(90, 0, 0.1, 0);

	glTranslatef(247, -2, 52);

	cerca();

	//banqueta

	glTranslatef(-210, 0, -38);

	banqueta();

	//banqueta 2

	glTranslatef(-0.1, 0, 90);

	glPushMatrix();

	///////////////////////////////////////////////////////////////	//Autobuses

	if (avanza1 > 30)
	{

		glTranslatef(-avanza1, 3, -65);

		avanza1 = avanza1 - 0.5;
	}

	if (avanza1 == 30)
	{

		if (w < 90)
		{
			w = w + 0.2;
		}
		else
		{
			w = w + 0;

			avanza2 = avanza2 - 0.4;
			printf("\n= %f", avanza2);
		}

		glTranslatef(-avanza1, 3, -avanza2);
		glRotatef(-w, 0, 1, 0);
	}

	autobus();
	glPopMatrix();

	glPushMatrix();

	glTranslatef(0, 3, 0);
	glRotatef(90, 0, 1, 0);

	autobus();

	glPopMatrix();

	glPushMatrix();

	glTranslatef(-200, 0, -100);

	pasto(160, -20);

	glPopMatrix();

	glPushMatrix();

	glTranslatef(3, -0, -102);

	pasto(100, -100);

	glPopMatrix();

	glPushMatrix();

	glTranslatef(20, -8, -108);
	glRotatef(-33, 0, 1, 0);

	mural();

	glPopMatrix();

	glPushMatrix();

	glTranslatef(212, 0, -94);
	banqueta();

	glPopMatrix();

	glPushMatrix();

	glTranslatef(10, -0, 146);

	pasto(200, -200);

	glPopMatrix();

	glPushMatrix();

	glTranslatef(-10, -0, -14);

	cerca();

	glPopMatrix();

	glPushMatrix();

	glRotatef(90, 0, 1, 0);

	glTranslatef(38, -0, -35);
	cerca();

	glPopMatrix();

	glPushMatrix();

	glTranslatef(-50, 120, 170);

	sol();

	glPopMatrix();

	glPushMatrix();

	glTranslatef(-15, -1, -17);

	banqueta();

	glPopMatrix();

	glPushMatrix();

	glTranslatef(150, -7, -103);
	E_redes();

	glPopMatrix();

	glPushMatrix();
	glTranslatef(6, 0, 20);
	arbol();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(40, 0, -160);
	arbol();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5, 2, -204);
	paradero();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(4, -0.8, -345);
	pasto(200, 100);
	glPopMatrix();

	glPushMatrix();
	glScalef(0.5, 0.5, 0.5);
	glTranslatef(5, 0, 0);
	persona();
	glPopMatrix();

	glPushMatrix();
	glScalef(0.5, 0.5, 0.5);
	glTranslatef(5, 0, -170);
	Policia();
	glPopMatrix();

	camina -= 0.2;

	glPushMatrix();
	glScalef(0.5, 0.5, 0.5);
	glTranslatef(-35, -11, -camina);
	persona2();
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glTranslatef(180, -11, -170);
	Edificio_nuevo();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-200, 0, -120);
	arbol();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-125, 0, -120);
	arbol();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-60, 0, -120);
	arbol();
	glPopMatrix();
}

void trucos()
{

	switch (truco)
	{

	case 'x':
		glPushMatrix();
		glScalef(0.5, 0.5, 0.5);
		glTranslatef(10, -5, 100);
		Isis();
		glPopMatrix();
		break;

	case 'w':
		glPushMatrix();
		//glScalef(0.5,0.5,0.5);
		glTranslatef(10, -5, 100);

		mural();

		glPopMatrix();

		break;
	}
}

void display(void)
{

	glEnable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	glRotatef(-camPitch, 1.0, 0.0, 0.0);
	glRotatef(-camYaw, 0.0, 1.0, 0.0);
	glTranslatef(camX, camY, camZ);

	trucos();

	suelo();
	//calles();
	Entrada();

	glEnd();
	glPopMatrix();
	glFlush();
	glutSwapBuffers();
}

void init(void)
{

	glClearColor(0.8, 0.89, 0.89, 0.1);
	//glClearColor(0, 0, 0, 0);

	enterMouseDrag(0, 0);
}

void idle(void)
{
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(1350, 700);
	//glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);

	glutCreateWindow("Tec");
	init();
	glutDisplayFunc(display);
	glutTimerFunc(0, Tiempo, 0);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialFunc);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(allMotionFunc);
	glutPassiveMotionFunc(allMotionFunc);
	glutIdleFunc(idle);
	glutMainLoop();

	return 0;
}

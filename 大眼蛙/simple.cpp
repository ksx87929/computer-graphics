#include <math.h>
#include <iostream>
#include <GL/glut.h>
const int n = 100;
GLfloat R = 0.75;
const GLfloat pi = 3.1415926;
void mydisplay()
{
	GLUquadricObj* quadricPtr;
	quadricPtr = gluNewQuadric();
	int radius = 360, slices = 100, stacks = 100;
	int i = 0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	//glutWireTeapot(5);

	
	glPushMatrix();//¥k¤â
		glTranslatef(5.5, -3.5, 0);
		glRotated(45, 1, 0, 1);
		glScalef(1.5, 0.4, 1);
		glColor3f(0.8, 1, 0);
		gluSphere(quadricPtr, 2, 100, 100);
	glPopMatrix();
	glPushMatrix();//¥k¤â«ü
		glTranslatef(7, -1.7, 0);
		glColor3f(0.8, 1, 0);
		gluSphere(quadricPtr, 0.5, 100, 100);
	glPopMatrix();
	glPushMatrix();//¥k¤â«ü
	glTranslatef(7.8, -2.7, 0);
	glColor3f(0.8, 1, 0);
	gluSphere(quadricPtr, 0.5, 100, 100);
	glPopMatrix();
	glPushMatrix();//¥ª¤â
	glTranslatef(-5, -4.5, 0);
	glRotated(75, 1, 0, 1);
	glScalef(1, 0.3, 1);
	glColor3f(0.8, 1, 0);
	gluSphere(quadricPtr, 2, 100, 100);
	glPopMatrix();
	glPushMatrix();//¥ª¤â«ü
	glTranslatef(-5, -6.3, 0);
	glColor3f(0.8, 1, 0);
	gluSphere(quadricPtr, 0.5, 100, 100);
	glPopMatrix();
	glPushMatrix();//¥ª¤â«ü
	glTranslatef(-6.4, -5.3, 0);
	glColor3f(0.8, 1, 0);
	gluSphere(quadricPtr, 0.5, 100, 100);
	glPopMatrix();
	glPushMatrix();//¨­Åé
		glTranslatef(0, -8, 0);
		glRotatef(-90, 1, 0,0);
		glColor3f(1, 1, 1);
		gluCylinder(quadricPtr, 6, 4, 5, 100, 100);
	glPopMatrix();
	glPushMatrix();//ªá¯¾
		glPointSize(5);
		glLineWidth(20);
		glColor3f(2, 0.1, 0.5);
		glBegin(GL_LINES);
			glVertex3d(-3, -3,3);
			glVertex3d(-3, -8,3);
			glVertex3d(0, -3,3);
			glVertex3d(0, -8,3);
			glVertex3d(3, -3,3);
			glVertex3d(3, -8,3);
		glEnd();
	glPopMatrix();

	glPushMatrix();//½¹½ºµ²
		glPointSize(5);
		glLineWidth(2);
		glColor3f(0, 0, 0);
		glBegin(GL_TRIANGLES);
			glVertex3d(0, -5,3);
			glVertex3d(-1, -4,3);
			glVertex3d(-1, -6,3);
			glVertex3d(0, -5,3);
			glVertex3d(1, -4,3);
			glVertex3d(1, -6,3);
		glEnd();
	glPopMatrix();

	glPushMatrix();//Áy
		glTranslatef(0, 1, 0);
		glScalef(1, 0.75, 1);
		glColor3f(0.8, 1, 0);
		gluSphere(quadricPtr, 7, 100, 100);
	glPopMatrix();

	glPushMatrix();//¥ª¸|¬õ
		R = 1.0F;
		glTranslatef(-5, 0, 0);
		glColor3f(1, 0.7, 0.9);
		glBegin(GL_POLYGON);
		for (i = 0; i < n; i++)
			glVertex3f(R*cos(2 * pi / n * i), R*sin(2 * pi / n * i), 3);
		glEnd();
	glPopMatrix();

	glPushMatrix();//¥k¸|¬õ
		R = 1.0F;
		glTranslatef(5, 0, 0);
		glColor3f(1, 0.7, 0.9);
		glBegin(GL_POLYGON);
		for (i = 0; i < n; i++)
			glVertex3f(R*cos(2 * pi / n * i), R*sin(2 * pi / n * i), 3);
		glEnd();
	glPopMatrix();

	glPushMatrix();//¼L¤Ú
		glPointSize(5);
		glLineWidth(7);
		glBegin(GL_LINES);
			glColor3f(0, 0, 0);
			glVertex3f(-3, 0, 3);
			glVertex3f(0, -2.5, 3);
			glVertex3f(0, -2.5, 3);
			glVertex3f(3, 0, 3);
		glEnd();
	glPopMatrix();

	glPushMatrix(); //¥ª²´
		glTranslatef(-3, 5, 0);
		glColor3f(1.0, 1.0, 1.0);
		gluSphere(quadricPtr, 3.5, 100, 100);
	glPopMatrix();

	glPushMatrix();//¥ªÀû¤Õ
		glPointSize(5);
		glLineWidth(5);
		glBegin(GL_LINES);
			glColor3f(0, 0, 0);
			glVertex3f(-1.75, 5.75, 1);
			glVertex3f(-0.75, 5, 1);
			glVertex3f(-0.75, 5, 1);
			glVertex3f(-1.75, 4.25, 1);
		glEnd();
	glPopMatrix();

	glPushMatrix(); //¥k²´
		glTranslatef(3, 5, 0);
		glColor3f(1.0, 1.0, 1.0);
		gluSphere(quadricPtr, 3.5, 100, 100);
	glPopMatrix();

	glPushMatrix();//¥kÀû¤Õ
		R = 0.75F;
		glTranslatef(1, 5, 0);
		glColor3f(0, 0, 0);
		glBegin(GL_POLYGON);
		for (i = 0; i < n; i++)
			glVertex3f(R*cos(2 * pi / n * i), R*sin(2 * pi / n * i), 1);
		glEnd();
	glPopMatrix();

	glPushMatrix();//¥ª¸}
		glColor3f(0.8,1, 0);
		glScalef(1, 0.3, 1);
		glTranslatef(-3, -28,5);
		gluSphere(quadricPtr, 3, 100, 100);
	glPopMatrix();
	
	glPushMatrix();//¥ª¸}³k
	glColor3f(0.8, 1, 0);
	glScalef(0.8, 0.5, 1);
	glTranslatef(-6.5, -17.6, 5);
	gluSphere(quadricPtr, 1, 100, 100);
	glPopMatrix();
	glPushMatrix();//¥ª¸}³k
	glColor3f(0.8, 1, 0);
	glScalef(1, 0.5, 1);
	glTranslatef(-3.3, -18, 5);
	gluSphere(quadricPtr, 1, 100, 100);
	glPopMatrix();
	glPushMatrix();//¥ª¸}³k
	glColor3f(0.8, 1, 0);
	glScalef(1, 0.5, 1);
	glTranslatef(-1.3, -18, 5);
	gluSphere(quadricPtr, 1, 100, 100);
	glPopMatrix();

	glPushMatrix();//¥k¸}
	glColor3f(0.8, 1, 0);
	glScalef(1, 0.3, 1);
	glTranslatef(3, -28, 5);
	gluSphere(quadricPtr, 3, 100, 100);
	glPopMatrix();
	glPushMatrix();//¥ª¸}³k
	glColor3f(0.8, 1, 0);
	glScalef(0.8, 0.5, 1);
	glTranslatef(6.5, -17.6, 5);
	gluSphere(quadricPtr, 1, 100, 100);
	glPopMatrix();
	glPushMatrix();//¥ª¸}³k
	glColor3f(0.8, 1, 0);
	glScalef(1, 0.5, 1);
	glTranslatef(3.3, -18, 5);
	gluSphere(quadricPtr, 1, 100, 100);
	glPopMatrix();
	glPushMatrix();//¥ª¸}³k
	glColor3f(0.8, 1, 0);
	glScalef(1, 0.5, 1);
	glTranslatef(1.3, -18, 5);
	gluSphere(quadricPtr, 1, 100, 100);
	glPopMatrix();

	glFlush();
	gluDeleteQuadric(quadricPtr);
}

void init()
{
	//gluLookAt(5, 5, 5, 0, 0, 0, 0, 1, 0);
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     glOrtho(-20.0, 20.0, -20.0, 20.0, -20.0, 20.0);
	 glMatrixMode(GL_MODELVIEW);
}

void main(int argc, char** argv)
{
     glutInitWindowSize(500,500);
     glutCreateWindow("simple");
     glutDisplayFunc(mydisplay);
     init();

     glutMainLoop();
}
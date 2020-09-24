#include <stdlib.h>
#include <GL/glut.h>
#include <iostream>
#include <math.h>
using namespace std;
typedef float point[4];
typedef float point3[4];
/* initial tetrahedron */
point v[]={{0.0, 0.0, 1.0}, {0.0, 0.942809, -0.33333},
          {-0.816497, -0.471405, -0.333333}, {0.816497, -0.471405, -0.333333}};
int n;
int smlen=0;
int menu;
//point smoo[100000];
struct smooth
{
	point n;
	point tri[3];

};
void givepoint(point a, point b)
{
	a[0] = b[0];
	a[1] = b[1];
	a[2] = b[2];
}
smooth	smooths[100000];
int smoothlen=0;
bool equal(point a, point b)
{
	return (a[0] == b[0] && a[1] == b[1] && a[2] == b[2]);
}
void normal(point p)
{
    double sqrt();
    float d =0.0;
    int i;
    for(i=0; i<3; i++) d+=p[i]*p[i];
	d = sqrtf(d);
    if(d>0.0) for(i=0; i<3; i++) p[i]/=d;
}
void CrossProduct(point a, point b, point c, point r)
{
	r[0] = (b[1] - a[1])*(c[2] - a[2]) - (b[2] - a[2])*(c[1] - a[1]);
	r[1] = (b[2] - a[2])*(c[0] - a[0]) - (b[0] - a[0])*(c[2] - a[2]);
	r[2] = (b[0] - a[0])*(c[1] - a[1]) - (b[1] - a[1])*(c[0] - a[0]);
	normal(r);
}	
void findn(point a, point tmp)
{
	int count = 0;
		for (int i = 0; i < smoothlen; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (equal(smooths[i].tri[j],a) )
				{
					tmp[0] += smooths[i].n[0];
					tmp[1] += smooths[i].n[1];
					tmp[2] += smooths[i].n[2];
					count++;
				}
			}		
		}
		tmp[0] /= count;
		tmp[1] /= count;
		tmp[2] /= count;
		normal(tmp);
}
int k = 1;
void triangle( point a, point b, point c)
{
	if (k == 1)
	{
		glBegin(GL_LINE_LOOP);
		glVertex3fv(a);
		glVertex3fv(b);
		glVertex3fv(c);
		glEnd();
	}
	else if (k == 2)
	{

		point n;
		CrossProduct(a, b, c, n);
		glBegin(GL_TRIANGLES);
		glNormal3fv(n);
		glVertex3fv(a);
		glVertex3fv(b);
		glVertex3fv(c);
		glEnd();
	}
	else
	{
		point n;
		glBegin(GL_TRIANGLES);
			givepoint(n, a);
			findn(a, n);
			glNormal3fv(n);
			glVertex3fv(a);

			givepoint(n, b);
			findn(b, n);
			glNormal3fv(n);
			glVertex3fv(b);

			givepoint(n, c);
			findn(c, n);
			glNormal3fv(n);
			glVertex3fv(c);
		glEnd();
	}
}

void divide_triangle(point a, point b, point c, int m)
{

/* triangle subdivision using vertex numbers
righthand rule applied to create outward pointing faces */

    point v1, v2, v3;
    int j;
    if(m>0)
    {
        for(j=0; j<3; j++) v1[j]=a[j]+b[j];
         normal(v1);
        for(j=0; j<3; j++) v2[j]=a[j]+c[j];
         normal(v2);
        for(j=0; j<3; j++) v3[j]=b[j]+c[j];
         normal(v3);
        divide_triangle(a, v1, v2, m-1);
        divide_triangle(c, v2, v3, m-1);
        divide_triangle(b, v3, v1, m-1);
        divide_triangle(v1, v3, v2, m-1);
    }
	else
	{
		givepoint(smooths[smoothlen].tri[0], a);
		givepoint(smooths[smoothlen].tri[1], b);
		givepoint(smooths[smoothlen].tri[2], c);
		CrossProduct(a, b, c, smooths[smoothlen].n);
		smoothlen++;
		(triangle(a,b,c)); /* draw triangle at end of recursion */
	}
		
}

void tetrahedron( int m)
{
    divide_triangle(v[0], v[1], v[2], m);
    divide_triangle(v[3], v[2], v[1], m);
    divide_triangle(v[0], v[3], v[1], m);
    divide_triangle(v[0], v[2], v[3], m);
}
void display(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glEnable(GL_LIGHTING);
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	//glShadeModel(GL_FLAT);
	glShadeModel(GL_SMOOTH);
    tetrahedron(n);

    glFlush();
}
void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-4.0, 4.0, -4.0 * (GLfloat) h / (GLfloat) w,
            4.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
    else
        glOrtho(-4.0 * (GLfloat) w / (GLfloat) h,
            4.0 * (GLfloat) w / (GLfloat) h, -4.0, 4.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    display();
}
void myinit()
{
    glEnable(GL_DEPTH_TEST); /* enable z buffer */

    glClearColor (1.0, 1.0, 1.0, 1.0);
    glColor3f (0.5, 0.5, 0.5);
}
void demo_menu(int id)
{
	if (id == 1)
	{
		k = 1;
	}
	else if (id == 2)
	{
		k = 2;
	}
	else
	{
		k = 3;
	}
	glutPostRedisplay();

}

void main(int argc, char **argv)
{
    n=3;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("sphere approximation");
    myinit();
	glutCreateMenu(demo_menu);
	glutAddMenuEntry("Wireframe", 1);
	glutAddMenuEntry("Flat", 2);
	glutAddMenuEntry("Smooth", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutMainLoop();
}

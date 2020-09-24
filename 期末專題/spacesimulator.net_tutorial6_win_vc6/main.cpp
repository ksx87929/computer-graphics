/*
 * ---------------- www.spacesimulator.net --------------
 *   ---- Space simulators and 3d engine tutorials ----
 *
 * Author: Damiano Vitulli
 *
 * This program is released under the BSD licence
 * By using this program you agree to licence terms on spacesimulator.net copyright page
 *
 *
 * Tutorial 6: Matrices and multi-objects loading
 * 
 * 
 *
 * To compile this project you must include the following libraries:
 * opengl32.lib,glu32.lib,glut32.lib
 * You need also the header file glut.h in your compiler directory.
 *  
 */

/*
 * Spaceships credits:
 * fighter1.3ds - created by: Dario Vitulli 
 * fighter2.3ds - taken from http://www.3dcafe.com/asp/meshes.asp
 * fighter3.3ds - created by: Rene Reiter <renereiter@hotmail.com>
 */

#ifdef __WIN32__
#include <windows.h>
#endif

#include <GL/glut.h>
#include "load_bmp.h"
#include "load_3ds.h"
#include "object.h"



/**********************************************************
 *
 * VARIABLES DECLARATION
 *
 *********************************************************/

// The width and height of your window, change them as you like
int screen_width=640;
int screen_height=480;

// Absolute rotation values (0-359 degrees) and rotation increments for each frame
double rotation_x=0, rotation_x_increment=0.1;
double rotation_y=0, rotation_y_increment=0.05;
double rotation_z=0, rotation_z_increment=0.03;
 
// Flag for rendering as lines or filled polygons
int filling=1; //0=OFF 1=ON

//Lights settings
GLfloat light_ambient[]= { 0.1f, 0.1f, 0.1f, 0.1f };
GLfloat light_diffuse[]= { 1.0f, 1.0f, 1.0f, 0.0f };
GLfloat light_specular[]= { 1.0f, 1.0f, 1.0f, 0.0f };
GLfloat light_position[]= { 100.0f, 0.0f, -10.0f, 1.0f };

//Materials settings
GLfloat mat_ambient[]= { 0.1f, 0.1f, 0.1f, 0.0f };
GLfloat mat_diffuse[]= { 1.0f, 1.0f, 1.0f, 0.0f };
GLfloat mat_specular[]= { 0.2f, 0.2f, 0.2f, 0.0f };
GLfloat mat_shininess[]= { 1.0f };



/**********************************************************
 *
 * SUBROUTINE init(void)
 *
 * Used to initialize OpenGL and to setup our world
 *
 *********************************************************/

void init(void)
{
	MatrGenerateLookupTab();

    glClearColor(0.0, 0.0, 0.0, 0.0); // Clear background color to black

    // Viewport transformation
    glViewport(0,0,screen_width,screen_height);  

    // Projection transformation
    glMatrixMode(GL_PROJECTION); // Specifies which matrix stack is the target for matrix operations 
    glLoadIdentity(); // We initialize the projection matrix as identity
    gluPerspective(45.0f,(GLfloat)screen_width/(GLfloat)screen_height,5.0f,10000.0f); // We define the "viewing volume"
   
	//Lights initialization and activation
    glLightfv (GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv (GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv (GL_LIGHT1, GL_POSITION, light_position);    
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHTING);

    //Materials initialization and activation
	glMaterialfv (GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv (GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv (GL_FRONT, GL_SHININESS, mat_shininess);    

	//Other initializations
    glShadeModel(GL_SMOOTH); // Type of shading for the polygons
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Texture mapping perspective correction (OpenGL... thank you so much!)
    glEnable(GL_TEXTURE_2D); // Texture mapping ON
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)
	glEnable(GL_CULL_FACE); // Enable the back face culling
    glEnable(GL_DEPTH_TEST); // Enable the depth test (also called z buffer)

	//Objects loading
	ObjLoad ("fighter1.3ds","skull.bmp",             -10.0, 0.0, -30.0,    900,0,0);
	ObjLoad ("fighter2.3ds",'\0',                     10.0, 0.0, -30.0,    900,0,0);
	ObjLoad ("fighter3.3ds","spaceshiptexture.bmp",    0.0, 0.0, -30.0,    900,0,0);
}



/**********************************************************
 *
 * SUBROUTINE resize(int p_width, int p_height)
 *
 * This routine must be called everytime we resize our window.
 *
 * Input parameters: p_width = width in pixels of our viewport
 *					 p_height = height in pixels of our viewport
 * 
 *********************************************************/

void resize (int p_width, int p_height)
{
	if (screen_width==0 && screen_height==0) exit(0);
    screen_width=p_width; // We obtain the new screen width values and store it
    screen_height=p_height; // Height value

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We clear both the color and the depth buffer so to draw the next frame
    glViewport(0,0,screen_width,screen_height); // Viewport transformation

    glMatrixMode(GL_PROJECTION); // Projection transformation
    glLoadIdentity(); // We initialize the projection matrix as identity
    gluPerspective(45.0f,(GLfloat)screen_width/(GLfloat)screen_height,5.0f,10000.0f);

    glutPostRedisplay (); // This command redraw the scene (it calls the same routine of glutDisplayFunc)
}



/**********************************************************
 *
 * SUBROUTINE keyboard(unsigned char p_key, int p_x, int p_y)
 *
 * Used to handle the keyboard input (ASCII Characters)
 *
 * Input parameters: p_key = ascii code of the key pressed
 *					 p_x = not used, keeped to maintain compatibility with glut function
 *                   p_y = not used, keeped to maintain compatibility with glut function
 * 
 *********************************************************/

void keyboard(unsigned char p_key, int p_x, int p_y)
{  
    switch (p_key)
    {
		/*
        case ' ':
            rotation_x_increment=0;
            rotation_y_increment=0;
            rotation_z_increment=0;
        break;
		*/

		case 'j': case 'J':
			ObjTranslate(&object[obj_control],0,0,-1);
        break;
		case 'm': case 'M':
			ObjTranslate(&object[obj_control],0,0,1);
        break;
        case 'k': case 'K':
			ObjRotate(&object[obj_control],0,20,0);
        break;
        case 'l': case 'L':
			ObjRotate(&object[obj_control],0,-20,0);
        break;
		case 'r': case 'R':
            if (filling==0)
            {
                glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)
                filling=1;
            }   
            else 
            {
                glPolygonMode (GL_FRONT_AND_BACK, GL_LINE); // Polygon rasterization mode (polygon outlined)
                filling=0;
            }
        break;
		case 27:
			exit(0);
		break;
    }
}



/**********************************************************
 *
 * SUBROUTINE keyboard(int p_key, int p_x, int py)
 *
 * Used to handle the keyboard input (not ASCII Characters)
 *
 * Input parameters: p_key = code of the key pressed
 *					 p_x = not used, keeped to maintain compatibility with glut function
 *                   p_y = not used, keeped to maintain compatibility with glut function
 * 
 *********************************************************/

void keyboard_s (int p_key, int p_x, int py)
{
    switch (p_key)
    {
        case GLUT_KEY_UP:
			ObjRotate(&object[obj_control],-20,0,0);
            //rotation_x_increment = rotation_x_increment +0.005;
        break;
        case GLUT_KEY_DOWN:
			ObjRotate(&object[obj_control],20,0,0);
            //rotation_x_increment = rotation_x_increment -0.005;
        break;
        case GLUT_KEY_LEFT:
			ObjRotate(&object[obj_control],0,0,20);
            //rotation_y_increment = rotation_y_increment +0.005;
        break;
        case GLUT_KEY_RIGHT:
			ObjRotate(&object[obj_control],0,0,-20);
            //rotation_y_increment = rotation_y_increment -0.005;
        break;
        case GLUT_KEY_PAGE_UP:
			obj_control++;
			if (obj_control>=obj_qty) obj_control=0;
		break;
        case GLUT_KEY_PAGE_DOWN:
			obj_control--;
			if (obj_control<0) obj_control=obj_qty-1;
		break;
    }
}



/**********************************************************
 *
 * SUBROUTINE display(void)
 *
 * This is our main rendering subroutine, called each frame
 * 
 *********************************************************/

void display(void)
{
    int i,j;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // This clear the background color to dark blue
    glMatrixMode(GL_MODELVIEW); // Modeling transformation
    glLoadIdentity(); // Initialize the model matrix as identity
	
    for (i=0;i<obj_qty;i++)
	{
		glPushMatrix(); // We save the current matrix
	
		glMultMatrixf(&object[i].matrix[0][0]); // Now let's multiply the object matrix by the identity-first matrix
		
		if (object[i].id_texture!=-1) 
		{
			glBindTexture(GL_TEXTURE_2D, object[i].id_texture); // We set the active texture 
		    glEnable(GL_TEXTURE_2D); // Texture mapping ON
		}
		else
		    glDisable(GL_TEXTURE_2D); // Texture mapping OFF

		glBegin(GL_TRIANGLES); // glBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
		for (j=0;j<object[i].polygons_qty;j++)
		{
			//----------------- FIRST VERTEX -----------------
			//Normal coordinates of the first vertex
			glNormal3f( object[i].normal[ object[i].polygon[j].a ].x,
						object[i].normal[ object[i].polygon[j].a ].y,
						object[i].normal[ object[i].polygon[j].a ].z);
			// Texture coordinates of the first vertex
			glTexCoord2f( object[i].mapcoord[ object[i].polygon[j].a ].u,
						  object[i].mapcoord[ object[i].polygon[j].a ].v);
			// Coordinates of the first vertex
			glVertex3f( object[i].vertex[ object[i].polygon[j].a ].x,
						object[i].vertex[ object[i].polygon[j].a ].y,
						object[i].vertex[ object[i].polygon[j].a ].z);

			//----------------- SECOND VERTEX -----------------
			//Normal coordinates of the second vertex
			glNormal3f( object[i].normal[ object[i].polygon[j].b ].x,
						object[i].normal[ object[i].polygon[j].b ].y,
						object[i].normal[ object[i].polygon[j].b ].z);
			// Texture coordinates of the second vertex
			glTexCoord2f( object[i].mapcoord[ object[i].polygon[j].b ].u,
						  object[i].mapcoord[ object[i].polygon[j].b ].v);
			// Coordinates of the second vertex
			glVertex3f( object[i].vertex[ object[i].polygon[j].b ].x,
						object[i].vertex[ object[i].polygon[j].b ].y,
						object[i].vertex[ object[i].polygon[j].b ].z);
        
			//----------------- THIRD VERTEX -----------------
			//Normal coordinates of the third vertex
			glNormal3f( object[i].normal[ object[i].polygon[j].c ].x,
						object[i].normal[ object[i].polygon[j].c ].y,
						object[i].normal[ object[i].polygon[j].c ].z);
			// Texture coordinates of the third vertex
			glTexCoord2f( object[i].mapcoord[ object[i].polygon[j].c ].u,
						  object[i].mapcoord[ object[i].polygon[j].c ].v);
			// Coordinates of the Third vertex
			glVertex3f( object[i].vertex[ object[i].polygon[j].c ].x,
						object[i].vertex[ object[i].polygon[j].c ].y,
						object[i].vertex[ object[i].polygon[j].c ].z);

		}
		glEnd();
		glPopMatrix(); // Restore the previous matrix 
	}
    glFlush(); // This force the execution of OpenGL commands
    glutSwapBuffers(); // In double buffered mode we invert the positions of the visible buffer and the writing buffer
}



/**********************************************************
 *
 * The main routine
 * 
 *********************************************************/

int main(int argc, char **argv)
{
    // We use the GLUT utility to initialize the window, to handle the input and to interact with the windows system
    glutInit(&argc, argv);    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(screen_width,screen_height);
    glutInitWindowPosition(0,0);
    glutCreateWindow("www.spacesimulator.net - 3d engine tutorials - To exit press ESC");    
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc (resize);
    glutKeyboardFunc (keyboard);
    glutSpecialFunc (keyboard_s);
    init();
    glutMainLoop();

    return(0);    
}

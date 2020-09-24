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
 * Framework functions (GLUT)
 *  
 */

#ifdef FRAMEWORK_GLUT

#include "glut/glut.h"
#include "framework.h"
#include "object.h"
#include "camera.h"
#include "render_ogl.h"
#include "malloc.h"
#include "load_ini.h"
#include "main.h"
#include <stdio.h>



/*
 *
 * VARIABLES DECLARATION
 *
 */

// Main window configuration
int framework_screen_width=640; // Width
int framework_screen_height=480; // Height
int framework_screen_depth; // Color depth
int framework_fullscreen=0; // Fullscreen

void keyboard(unsigned char p_key, int p_x, int p_y);
void keyboard_s (int p_key, int p_x, int py);



/*
 * int FrameworkInit()
 *
 * This function initializes the framework (rendering device, input and sound system)
 *
 * Return value: (char) 1 if the framework was initialized correctly otherwise it exit with ERRORLEVEL 0
 *
 */

int FrameworkInit(int *argcp,char *argv[])
{
    // We use the GLUT utility to initialize the window, to handle the input and to interact with the windows system
    fprintf( stdout, "GLUT Framework initialization\n");
    glutInit(argcp,argv);    

    // Now we load the video settings from config.ini
	LoadINI("config.ini");
	framework_screen_width=LoadINI_GetInt("config.ini","screen settings","width",640);
	framework_screen_height=LoadINI_GetInt("config.ini","screen settings","height",480);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(framework_screen_width,framework_screen_height);
    glutInitWindowPosition(0,0);
    glutCreateWindow("www.spacesimulator.net - 3d engine tutorials - To exit press ESC");    

    // Set the GLUT function
	fprintf( stdout, "Setting GLUT functions\n");
    glutDisplayFunc(MainLoop);
    glutIdleFunc(MainLoop);
    glutReshapeFunc (FrameworkResize);
    glutKeyboardFunc (keyboard);
    glutSpecialFunc (keyboard_s);

    fprintf( stdout, "Framework initialized correctly\n");
	return (1);
}



/*
 * void FrameworkQuit()
 *
 * Used to exit from the program
 *
 */

void FrameworkQuit()
{
    fprintf( stdout, "Exiting program\n");
	exit(0);
}



/*
 * void FrameworkMainLoop()
 *
 * Main loop, where our simulation runs!
 *
 */

void FrameworkMainLoop()
{
	fprintf( stdout, "Entering main loop\n");
	glutMainLoop();
}



/*
 * void FrameworkResize(int p_width, int p_height)
 *
 * This routine must be called everytime we resize our window.
 *
 * Input parameters: p_width = width in pixels of our viewport
 *					 p_height = height in pixels of our viewport
 * 
 */

void FrameworkResize (int p_width, int p_height)
{
	if (framework_screen_width==0 && framework_screen_height==0) exit(0);
    framework_screen_width=p_width; // We obtain the new screen width values and store it
    framework_screen_height=p_height; // Height value

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We clear both the color and the depth buffer so to draw the next frame
    glViewport(0,0,framework_screen_width,framework_screen_height); // Viewport transformation

    glMatrixMode(GL_PROJECTION); // Projection transformation
    glLoadIdentity(); // We initialize the projection matrix as identity
    gluPerspective(45.0f,(GLfloat)framework_screen_width/(GLfloat)framework_screen_height,5.0f,10000.0f);

    glutPostRedisplay (); // This command redraw the scene (it calls the same routine of glutDisplayFunc)
}



/*
 * void FrameworkEvents()
 *
 * Subroutine used to handle some events (keyboard, joystick inputs, framework quit)
 *
 */

void FrameworkEvents()
{
	// Glut handles events using functions callbacks
}



/*
 * long Framework_GetTicks()
 *
 * Used to get the time in msec for time based animation and FPS calculation
 *
 */

long Framework_GetTicks()
{
	//In GLUT we have not functions to get system ticks so we return always +10
	//WARNING Physics is NOT time corrected!
	static long s_ticks;  
	return (s_ticks=s_ticks+10);
}



/*
 * void FrameworkSwapBuffers()
 *
 * Subroutine used to swap the color-buffers
 *
 * In double buffered mode the visible buffer is replaced with the writing buffer
 *
 */

void FrameworkSwapBuffers() 
{
	glutSwapBuffers( );
}



/*
 * void keyboard(unsigned char p_key, int p_x, int p_y)
 *
 * Used to handle the keyboard input (ASCII Characters)
 *
 * Input parameters: p_key = ascii code of the key pressed
 *					 p_x = not used, to maintain compatibility with glut function
 *                   p_y = not used, to maintain compatibility with glut function
 * 
 */

void keyboard(unsigned char p_key, int p_x, int p_y)
{  
	KeyboardHandle((int)p_key);
}



/*
 * void keyboard(int p_key, int p_x, int py)
 *
 * Used to handle the keyboard input (not ASCII Characters)
 *
 * Input parameters: p_key = code of the key pressed
 *					 p_x = not used, to maintain compatibility with glut function
 *                   p_y = not used, to maintain compatibility with glut function
 * 
 */

void keyboard_s (int p_key, int p_x, int py)
{
    switch (p_key)
    {
        case GLUT_KEY_UP:
            KeyboardHandle(273);
        break;
        case GLUT_KEY_DOWN:
            KeyboardHandle(274);
        break;
        case GLUT_KEY_LEFT:
            KeyboardHandle(276);
        break;
        case GLUT_KEY_RIGHT:
            KeyboardHandle(275);
        break;
		case GLUT_KEY_PAGE_UP:
            KeyboardHandle(280);
        break;
        case GLUT_KEY_PAGE_DOWN:
            KeyboardHandle(281);
        break;

    }
}



/*
 * void FrameworkAudioPlayWave(char *p_filename)
 *
 * To mix and play sounds
 *
 * Input parameters: p_filename = Filename of the sound to play
 *
 */

void FrameworkAudioPlayWave(char *file)
{
	// Glut has not audio
}

#endif

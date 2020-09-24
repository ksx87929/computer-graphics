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
 * Tutorial 11: Physics: dynamics, acceleration and force
 * 
 * 
 *
 * To compile this project you must include the following libraries:
 *
 * For OpenGL: opengl32.lib, glu32.lib 
 * For the GLUT FRAMEWORK: glut32.lib and glut.h
 * For the SDL FRAMEWORK: SDL.lib, SDLmain.lib, sdl.h (sdl.h links to other sdl headers)
 * To use a specific framework please enable the preprocessor directive: FRAMEWORK_x (where x is the name of the framework, for example GLUT = FRAMEWORK_GLUT)
 */

/*
 * Spaceships credits:
 * fighter1.3ds - created by: Dario Vitulli 
 * fighter2.3ds - taken from http://www.3dcafe.com/asp/meshes.asp
 *
 */

#include <stdio.h>
#include "framework.h"
#include "load_bmp.h"
#include "load_3ds.h"
#include "load_ini.h"
#include "object.h"
#include "camera.h"
#include "fonts_ogl.h"
#include "render_ogl.h"


 
/*
 *
 * VARIABLES DECLARATION
 *
 */

// FPS calculation and time based physics
unsigned int fps_physics=0;
unsigned int fps_rendering=0;
unsigned int set_fps_physics=100;
unsigned int remaining_time=0; // Remaining time for time based physics
unsigned long fps_count_physics=0; // FPS for physics engine
unsigned long fps_count_rendering=0; // FPS for rendering engine
unsigned long last_ticks=0; // Number of ticks since last frame
int msecxdframe=1000/set_fps_physics; // Milliseconds we want for each physics frame

 

/*
 * void WorldInit(void)
 *
 * Used to setup our world
 *  
 */

void WorldInit(void)
{
	LoadINI("world.ini");

	//Lookup tables
	MatrGenerateLookupTab();

	//Fonts loading
	FontCreate("fonts/font1.bmp",16,16,0,256,0,14); // First font
	FontCreate("fonts/font2.bmp",16,16,0,256,0,14); // Second font

	//Camera initialization
	MatrIdentity_4x4(camera.matrix);
	
	//Object loading
	ObjLoadFromIni("world.ini","object1");
	ObjLoadFromIni("world.ini","object2");
	ObjLoadFromIni("world.ini","object3");


	//Objects loading (old mode)
	/*
	ObjLoad ("fighter1.3ds","skull.bmp",             -10.0, 0.0, -30.0,    900,0,0);
	ObjLoad ("fighter2.3ds",'\0',                      0.0, 0.0, -30.0,    900,0,0);
	ObjLoad ("fighter3.3ds","spaceshiptexture.bmp",   10.0, 0.0, -30.0,    900,0,0);
	*/
}



/*
 * void FrameworkMainLoop()
 *
 * Main loop, where our simulation runs!
 *
 */

void MainLoop(void)
{
	int i; // Counters
	unsigned long l_start_time; // Start time for time based physics
	unsigned long l_elapsed_time; // Elapsed time for time based physics
	unsigned int l_frame_time; // Frame time for time based physics

	// Events
    FrameworkEvents(); // Process incoming events

	// Rendering
	fps_count_rendering++; // Increase rendering FPS counter
	l_start_time = Framework_GetTicks();
	RenderDisplay(); // Draw the screen

	// Elapsed time calculation
	l_elapsed_time=Framework_GetTicks()-l_start_time+remaining_time; // Elapsed time (we add also the previous remaining time)
	l_frame_time=l_elapsed_time / msecxdframe; // Frames quantity we must cycle for physics
	remaining_time=l_elapsed_time % msecxdframe; // Get the remaining time because we are working with integer values

	// Physics
	while (l_frame_time-->0) // Now do physics as many times as we need to match the elapsed time of the rendering phase
	{
		fps_count_physics++; // Increase physics FPS counter

		for(i=0;i<obj_qty;i++) //
		{
			ObjDrag(&object[i]); // Add Drag
			ObjDynamics(&object[i],(float)1.0/(float)set_fps_physics); // Do dynamics
		}
	}//while (l_frame_time-->0)
	

	// Rendering and Physics FPS calculation
	if ((Framework_GetTicks()-last_ticks)>=1000) // Every second
	{
		last_ticks = Framework_GetTicks(); // Save the current ticks to catch the next second
		// Assings the current FPS count to the global variables
		fps_physics=fps_count_physics;
		fps_rendering=fps_count_rendering;
		// Clear the local counters
		fps_count_physics = 0;
		fps_count_rendering = 0;
	}

}



/*
 * void KeyboardHandle(int p_unicode)
 *
 * This function handles keyboard events
 *
 */

void KeyboardHandle(int p_unicode)
{
	switch (p_unicode)
	{
	case 106: // j
		ObjForce(&object[obj_control],1,0,0,-50000);
		break;
	case 109: // m
		ObjForce(&object[obj_control],1,0,0,50000);
		break;
	case 107: // k
		ObjTorque(&object[obj_control],0,50000,0);
		break;
	case 108: // l
		ObjTorque(&object[obj_control],0,-50000,0);
		break;
	case 101: // e
		CamRotate(200,0,0);
		break;
	case 99: // c
		CamRotate(-200,0,0);
		break;
	case 97: // a
		CamRotate(0,-200,0);
		break;
	case 100: // d
		CamRotate(0,200,0);
		break;
	case 122: // z
		CamRotate(0,0,-200);
		break;
	case 120: // x
		CamRotate(0,0,200);
		break;
	case 119: // w
		CamTranslate(0,0,1);
		break;
	case 115: // s
		CamTranslate(0,0,-1);
		break;
	case 114: // r
		render_filling++;
		if (render_filling>1) render_filling=0;
		break;
	case 273: // UP ARROW
		ObjTorque(&object[obj_control],-50000,0,0);
		break;
	case 274: // DOWN_ARROW
		ObjTorque(&object[obj_control],50000,0,0);
		break;
	case 276: // LEFT ARROW
		ObjTorque(&object[obj_control],0,0,50000);
		break;
	case 275: // RIGHT ARROW
		ObjTorque(&object[obj_control],0,0,-50000);
		break;
	case 280: // PAGEUP
		obj_control++;
		camera.n_object++;
		if (obj_control>=obj_qty) obj_control=0;
		if (camera.n_object>=obj_qty) camera.n_object=0;
		break;
	case 281: // PAGEDOWN
		obj_control--;
		camera.n_object--;
		if (obj_control<0) obj_control=obj_qty-1;
		if (camera.n_object<0) camera.n_object=obj_qty-1;
		break;
	case 27:// ESCAPE
		FrameworkQuit();
		break;
	default:                                    
		break;                                  
	}
}



/*
 * The main routine
 *  
 */

int main(int argcp, char *argv[])
{
	// Standard output as log file
	freopen( "log.txt", "w", stdout );
	freopen( "log.txt", "w", stderr );

	// Initializations
	FrameworkInit(&argcp,argv); // Framework
	RenderInit(); // Graphic library
	WorldInit(); // World

	// Main loop
    FrameworkMainLoop();

    return 0;
}

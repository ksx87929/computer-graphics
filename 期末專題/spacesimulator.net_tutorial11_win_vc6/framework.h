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
 * Framework functions
 *  
 * File header
 *  
 */

#ifndef _FRAMEWORK_H
#define _FRAMEWORK_H

// SDL
#ifdef FRAMEWORK_SDL
#include "sdl/include/sdl.h"
#define FRAMEWORK_MAXSOUNDS 4
typedef struct {
    Uint8 *data;
    Uint32 dpos;
    Uint32 dlen;
} sound_type;
extern SDL_AudioSpec framework_audiospec;
extern sound_type sounds[FRAMEWORK_MAXSOUNDS];
extern void FrameworkEvents();
extern void FrameworkMixAudio(void *unused, Uint8 *stream, int len);
#endif

// GLUT
#ifdef FRAMEWORK_GLUT
#include "glut/glut.h"
#endif



/*
 * VARIABLES DECLARATION
 *  
 */

extern int framework_screen_width;
extern int framework_screen_height;



/*
 * FUNCTIONS DECLARATION
 *  
 */

extern int FrameworkInit(int *argcp,char *argv[]);
extern void FrameworkQuit();
extern void FrameworkMainLoop();
extern void FrameworkResize (int p_width, int p_height);
extern void FrameworkEvents();
extern long Framework_GetTicks();
extern void FrameworkSwapBuffers();
extern void FrameworkAudioPlayWave(char *file);

#endif
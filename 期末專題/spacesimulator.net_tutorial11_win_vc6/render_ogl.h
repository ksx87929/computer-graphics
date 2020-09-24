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
 * Rendering functions
 *  
 * File header
 *  
 */

#ifndef _RENDER_OGL_H
#define _RENDER_OGL_H


#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>



/*
 * VARIABLES DECLARATION
 *  
 */

extern int render_filling; //0=OFF 1=ON

//Lights settings
extern GLfloat light_ambient[];
extern GLfloat light_diffuse[];
extern GLfloat light_specular[];
extern GLfloat light_position[];

//Materials settings
extern GLfloat mat_ambient[];
extern GLfloat mat_diffuse[];
extern GLfloat mat_specular[];
extern GLfloat mat_shininess[];



/*
 * FUNCTIONS DECLARATION
 *  
 */

extern void RenderInit(void);
extern void RenderDisplay(void);

#endif
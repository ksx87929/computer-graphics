/*
 * ---------------- www.spacesimulator.net --------------
 *   ---- Space simulators and 3d engine tutorials ----
 *
 * Author: Damiano Vitulli
 *
 * This program is released under the BSD licence
 * By using this program you agree to licence terms on spacesimulator.net copyright page
 *
 * Main File header
 *  
 */

#ifndef _MAIN_H
#define _MAIN_H



/*
 * VARIABLES DECLARATION
 *  
 */

// FPS calculation and time based physics
extern unsigned int fps_physics;
extern unsigned int fps_rendering;
extern unsigned int set_fps_physics;



/*
 * FUNCTIONS DECLARATION
 *  
 */

extern void MainLoop(void);
extern void KeyboardHandle(int p_unicode);

#endif
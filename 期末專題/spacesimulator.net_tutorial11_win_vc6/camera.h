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
 * Camera management
 *  
 * File header
 *  
 */

#ifndef _CAMERA_H
#define _CAMERA_H

#include "mat_matr.h"



/*
 * TYPES DECLARATION
 *  
 */

typedef struct {

	int n_object; // number of the object to link (only if type>0)
	
	matrix_4x4_type matrix; //camera matrix

} camera_type, *camera_type_ptr;



/*
 * VARIABLES DECLARATION
 *  
 */

extern camera_type camera;



/*
 * FUNCTIONS DECLARATION
 *  
 */

extern void CamTranslate (float p_x,float p_y,float p_z);
extern void CamTranslateW (float p_x,float p_y,float p_z);
extern void CamRotate (int p_angle_x,int p_angle_y,int p_angle_z);

#endif
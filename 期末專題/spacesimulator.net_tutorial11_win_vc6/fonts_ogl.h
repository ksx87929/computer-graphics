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
 * Fonts functions
 *  
 * File header
 *  
 */

#ifndef _FONTS_H
#define _FONTS_H

#include "mat_vect.h"
#include "mat_matr.h"



/*
 * CONSTANTS DECLARATION
 *  
 */

#define MAX_FONTS 10 // Max number of fonts



/*
 * TYPES DECLARATION
 *  
 */

typedef struct{
    
    int list_base; //Base number of the OpenGL list that contain the font
	int texture_id; //Texture ID of the font

}font_type;



/*
 * VARIABLES DECLARATION
 *  
 */

extern font_type font[MAX_FONTS];



/*
 * FUNCTIONS DECLARATION
 *  
 */

extern char FontCreate(char *p_filename,int p_grid_x_qty,int p_grid_y_qty,int p_fonts_start_pos,int p_fonts_end_pos,int p_ascii_offset,int p_fonts_spacing);
extern void FontPrint(int p_font, int p_x, int p_y, char *p_string);

#endif
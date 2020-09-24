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
 */

#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "framework.h"
#include "load_bmp.h"
#include "fonts_ogl.h"



/*
 * VARIABLES DECLARATION
 *
 */

int fonts_qty=0; // Total number of fonts
font_type font[MAX_FONTS]; // Font declaration



/*
 * char FontCreate(char *p_filename,int p_grid_x_qty,int p_grid_y_qty,int p_fonts_start_pos,int p_fonts_end_pos,int p_ascii_offset,int p_fonts_spacing)
 *
 * This function loads a font from a bmp image  
 *
 * Parameters: p_filename = filename of a bmp image mapped with a font
 *			   p_grid_x_qty = number of characters per row
 *             p_grid_y_qty = number of characters per column
 *             p_fonts_start_pos = absolute position to start reading characters
 *             p_fonts_end_pos = absolute position to end reading characters
 *             p_ascii_offset = ascii offset to use for internal storage
 *             p_fonts_spacing = (pixels) distance between characters
 *
 * Return value: (char) 1 if the font was loaded correctly, 0 otherwise
 *
 */

char FontCreate(char *p_filename,int p_grid_x_qty,int p_grid_y_qty,int p_fonts_start_pos,int p_fonts_end_pos,int p_ascii_offset,int p_fonts_spacing)
{
	int i; // Index variable
	int l_x,l_y; // x and y counters	
	float l_x_inc,l_y_inc; // x and y increments
	int l_image_width,l_image_height; // width and height of the image
	int l_font_width, l_font_height; // width and height of the font

    fprintf(stdout, "Creating font: %s\n",p_filename);

    font[fonts_qty].list_base=glGenLists(p_fonts_end_pos-p_fonts_start_pos); // Initialize the display list that will contain the font
    font[fonts_qty].texture_id=LoadBMP(p_filename); // The Function LoadBitmap() returns the current texture ID		
    if (font[fonts_qty].texture_id == -1) return (0); // If the texture was not found, bye bye!
	glBindTexture(GL_TEXTURE_2D, font[fonts_qty].texture_id); // Set the active texture

	// We need to know the texture size (width and height) to calculate the size of the font
	// With these OpenGL functions we can extrapolate the width and the height of the current active texture
	glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH,&l_image_width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT,&l_image_height); 
	l_font_width=l_image_width/p_grid_x_qty; // Width of the font
	l_font_height=l_image_height/p_grid_y_qty; // Height!

	l_x=p_fonts_start_pos%p_grid_x_qty; // x counter (column)
	l_y=p_fonts_start_pos/p_grid_y_qty; // y counter (row)
	l_x_inc=1.0f/p_grid_x_qty; // x increment normalized to 0->1
	l_y_inc=1.0f/p_grid_y_qty; // y increment normalized to 0->1

	for (i=p_fonts_start_pos;i<=p_fonts_end_pos;i++)
	{			
		glNewList(font[fonts_qty].list_base+i+p_ascii_offset,GL_COMPILE); // Display list creation

		// Now we create a little square for each font
		glBegin(GL_QUADS);

		// First vertex
		glTexCoord2f(l_x*l_x_inc,1-l_y*l_y_inc-l_y_inc);
		glVertex2i(0,0);

		// Second vertex
		glTexCoord2f(l_x*l_x_inc+l_x_inc,1-l_y*l_y_inc-l_y_inc);
		glVertex2i(l_font_width,0);

		// Third vertex
		glTexCoord2f(l_x*l_x_inc+l_x_inc,1-l_y*l_y_inc);
		glVertex2i(l_font_width,l_font_height);

		// Fourth vertex
		glTexCoord2f(l_x*l_x_inc,1-l_y*l_y_inc);
		glVertex2i(0,l_font_height);

		glEnd();

		glTranslated(p_fonts_spacing,0,0);// Distance between characters
		glEndList();

		// Indexes updating
		l_x++;
		if (l_x>=p_grid_x_qty) {l_x=0;l_y++;}
	}                                                                                                       // Loop Until All 256 Are Built
	fonts_qty++; // Increase the font number
	return (1); // If all is ok then return 1
}



/*
 * void FontPrint(int p_font, int p_x, int p_y, char *p_string)
 *
 * Prints a string to a specified position using p_font font
 *
 * Parameters: p_font = ID number of the font to use
 *			   p_x = x start position (pixels)
 *             p_y = y start position (pixels)
 *             p_string = string 
 *
 */

void FontPrint(int p_font, int p_x, int p_y, char *p_string)
{
	glBindTexture(GL_TEXTURE_2D, font[p_font].texture_id);  // Set the active texture
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Transparency mode (text background transparent)
	glEnable(GL_BLEND); // Activate blending

	// We need to deactivate lighting and depth testing
    glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
 
	// The fonts must be printed in 2d so we must create an Orthogonal projection matrix
    glMatrixMode(GL_PROJECTION); // Specifies the PROJECTION matrix as the target for matrix operations 
	glPushMatrix(); // Save the current matrix
    glLoadIdentity(); // We initialize the projection matrix as identity
    glOrtho(0,framework_screen_width,0,framework_screen_height,0.0f,1.0f); // Orthogonal projection matrix
        
	glMatrixMode(GL_MODELVIEW); // Specifies the MODELVIEW matrix as the target for matrix operations 
	glPushMatrix(); // Save the current matrix
    glLoadIdentity(); // Initialize the model matrix as identity
    glTranslated(p_x,p_y,0); // Where to print!
    glListBase(font[p_font].list_base); // Where is stored the font!
	glCallLists(strlen(p_string),GL_BYTE,p_string); // Print the text!!
    glPopMatrix(); // Restore the previous matrix

	// Now it is necessary to restore the previous status...
    glMatrixMode(GL_PROJECTION); // Specifies the PROJECTION matrix as the target for matrix operations 
    glPopMatrix(); // Restore the previous matrix
	glMatrixMode(GL_MODELVIEW); // Specifies the MODELVIEW matrix as the target for matrix operations 
	glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

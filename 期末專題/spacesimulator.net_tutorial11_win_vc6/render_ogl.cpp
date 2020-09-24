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
 */

#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "camera.h"
#include "fonts_ogl.h"
#include "framework.h"
#include "object.h"
#include "main.h"



/*
 * VARIABLES DECLARATION
 *
 */

// Flag for rendering as lines or filled polygons
int render_filling=1; //0=OFF 1=ON

//Lights settings
GLfloat light_ambient[]= { 0.1f, 0.1f, 0.1f, 0.1f };
GLfloat light_diffuse[]= { 1.0f, 1.0f, 1.0f, 0.0f };
GLfloat light_specular[]= { 1.0f, 1.0f, 1.0f, 0.0f };
GLfloat light_position[]= { 100.0f, 0.0f, -10.0f, 1.0f };

//Materials settings
GLfloat mat_ambient[]= { 0.2f, 0.2f, 0.2f, 0.0f };
GLfloat mat_diffuse[]= { 1.0f, 1.0f, 1.0f, 0.0f };
GLfloat mat_specular[]= { 0.2f, 0.2f, 0.2f, 0.0f };
GLfloat mat_shininess[]= { 1.0f };



/*
 * void RenderInit(void)
 *
 * Used to initialize the Graphic library
 *  
 */

void RenderInit(void)
{
	fprintf(stdout, "Graphic library initialization\n");

    glClearColor(0.0, 0.0, 0.0, 0.0); // Clear background color to black

    // Viewport transformation
    glViewport(0,0,framework_screen_width,framework_screen_height);  

    // Projection transformation
    glMatrixMode(GL_PROJECTION); // Specifies which matrix stack is the target for matrix operations 
    glLoadIdentity(); // Initialize the projection matrix as identity
    gluPerspective(45.0f,(GLfloat)framework_screen_width/(GLfloat)framework_screen_height,5.0f,50000000.0f); // Define the "viewing volume"
   
	//Lights initialization and activation
    glLightfv (GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv (GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv (GL_LIGHT1, GL_POSITION, light_position);    
	//glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
	//glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHTING);

    //Materials initialization and activation
	glMaterialfv (GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv (GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv (GL_FRONT, GL_SHININESS, mat_shininess);    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // We modulate the color with the original surface color.

	//Other initializations
    glShadeModel(GL_SMOOTH); // Type of shading for the polygons
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Texture mapping perspective correction (OpenGL... thank you so much!)
    glEnable(GL_TEXTURE_2D); // Enables the Texture mapping
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)
	glEnable(GL_CULL_FACE); // Enable back face culling
    glEnable(GL_DEPTH_TEST); // Enable depth test (also called z buffer)
	glEnableClientState(GL_VERTEX_ARRAY); // Enable the vertex array list (glDrawElements)
	glEnableClientState(GL_NORMAL_ARRAY); // Enable the normal array list (glDrawElements)
	glEnableClientState(GL_TEXTURE_COORD_ARRAY); // Enable the texture coordinates array list (glDrawElements)
}



/*
 * void RenderDisplay(void)
 *
 * This is our main rendering subroutine, called each frame
 *  
 */

void RenderDisplay(void)
{
    int i;
	int l_n_font=0;
	char l_string[100];
	GLfloat l_feed_buffer[10]; // Feedback buffer

	if (render_filling==0)
	{
		glPolygonMode (GL_FRONT_AND_BACK, GL_LINE); // Polygon rasterization mode (polygon filled)
	}   
	else 
	{
		glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon outlined)
	}

	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // This clear the background

	glMatrixMode(GL_MODELVIEW); // Specifies the MODELVIEW matrix as the target for matrix operations
    glLoadIdentity(); // Initialize the model matrix as identity
	
	// Camera transformation
	glLoadMatrixf(&camera.matrix[0][0]); // At first we load the camera matrix
	
	// We don't want to move the light with the camera so we set again 
	// the light position after the camera transformation
	glLightfv (GL_LIGHT1, GL_POSITION, light_position); 

	// Display the objects!
    for (i=0;i<obj_qty;i++)
	{
		glPushMatrix(); // Save the current matrix
	
		glMultMatrixf(&object[i].matrix[0][0]); // Now let's multiply the object matrix by the camera matrix
		
		if (object[i].id_texture!=-1) 
		{
			glBindTexture(GL_TEXTURE_2D, object[i].id_texture); // Set the active texture 
		    glEnable(GL_TEXTURE_2D); // Texture mapping ON
		}
		else
		    glDisable(GL_TEXTURE_2D); // Texture mapping OFF

		// OpenGL Vertex,normal and texcoord arrays! 
        glVertexPointer(3,GL_FLOAT,3*sizeof(float),&object[i].vertex[0].x);
	    glNormalPointer(GL_FLOAT,3*sizeof(float),&object[i].normal[0].x);
		glTexCoordPointer(2,GL_FLOAT,2*sizeof(float),&object[i].mapcoord[0].u);
	    
		//Draw the object
		glDrawElements(GL_TRIANGLES,object[i].polygons_qty*3, GL_UNSIGNED_SHORT, &object[i].polygon[0].a);

		glPopMatrix(); // Restore the previous matrix 

	}
	
	// Print objects' AABB
    for (i=0;i<obj_qty;i++)
	{
		glPushMatrix(); // Save the current matrix
	
		glMultMatrixf(&object[i].matrix[0][0]); // Now let's multiply the object matrix by the camera matrix

		glDisable(GL_LIGHTING);
		glBegin(GL_POINTS);
			glVertex3f(object[i].aabb[0].x,object[i].aabb[0].y,object[i].aabb[0].z);
			glVertex3f(object[i].aabb[1].x,object[i].aabb[1].y,object[i].aabb[1].z);
			glVertex3f(object[i].aabb[2].x,object[i].aabb[2].y,object[i].aabb[2].z);
			glVertex3f(object[i].aabb[3].x,object[i].aabb[3].y,object[i].aabb[3].z);
			glVertex3f(object[i].aabb[4].x,object[i].aabb[4].y,object[i].aabb[4].z);
			glVertex3f(object[i].aabb[5].x,object[i].aabb[5].y,object[i].aabb[5].z);
			glVertex3f(object[i].aabb[6].x,object[i].aabb[6].y,object[i].aabb[6].z);
			glVertex3f(object[i].aabb[7].x,object[i].aabb[7].y,object[i].aabb[7].z);
		glEnd();
		glEnable(GL_LIGHTING);

		glPopMatrix(); // Restore the previous matrix 
	}

	// Print objects' names
    for (i=0;i<obj_qty;i++)
	{
		glPushMatrix(); // Save the current matrix
	
		glMultMatrixf(&object[i].matrix[0][0]); // Multiply the object matrix by the camera matrix
		glEnable(GL_TEXTURE_2D); // Texture mapping ON

		// Now we use the feedback buffer to know the final-window coordinates of the rendered object
		l_feed_buffer[0]=-1; l_feed_buffer[1]=-1; l_feed_buffer[2]=-1;
		glFeedbackBuffer(3,GL_3D,l_feed_buffer);
		glRenderMode(GL_FEEDBACK); // In GL_FEEDBACK rendering mode nothing is drawn, instead rendering information are sent to the feedback buffer
		glBegin(GL_POINTS); // glBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
			glVertex3f(0.0f,0.0f,0.0f); // We draw only a point using the same matrix of the object
		glEnd();
		glRenderMode(GL_RENDER); // Restore the normal rendering mode
		if (l_feed_buffer[0]==GL_POINT_TOKEN && l_feed_buffer[1]>=0 && l_feed_buffer[2]>=0)
		{
			if (i==obj_control) 
			    glColor4f(1.0f,0.0f,0.0f,1.0f); // Assign the red color to the selected object
			else 
			    glColor4f(1.0f,1.0f,1.0f,1.0f);
			// Finally we print the name of the object using the object's window coordinates
			sprintf(l_string,"%s",object[i].name);
			FontPrint(1,(int)l_feed_buffer[1],(int)l_feed_buffer[2],l_string);
		}
		glPopMatrix(); // Restore the previous matrix 		
	}

	// Print object information
    glColor4f(0.8f,0.8f,1.0f,1.0f);
	sprintf(l_string,"Object selected: %s",object[obj_control].name);
	FontPrint(0,0,framework_screen_height-20,l_string);
	sprintf(l_string,"Position: x=%.1f y=%.1f z=%.1f",object[obj_control].matrix[3][0], object[obj_control].matrix[3][1], object[obj_control].matrix[3][2]);
	FontPrint(0,0,framework_screen_height-40,l_string);

	// Print FPS
	sprintf(l_string,"FPS physics: %u",fps_physics);
	FontPrint(0,0,framework_screen_height-60,l_string);
	sprintf(l_string,"FPS rendering: %u",fps_rendering);
	FontPrint(0,0,framework_screen_height-80,l_string);

	glFlush(); // Forces the execution of OpenGL commands
	FrameworkSwapBuffers(); // In double buffered mode the visible buffer is replaced by the writing buffer
}
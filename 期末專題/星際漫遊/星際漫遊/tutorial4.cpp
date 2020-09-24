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
 * Tutorial 4: 3d engine - 3ds models loader
 * 
 * 
 *
 * To compile this project you must include the following libraries:
 * opengl32.lib,glu32.lib,glut.lib
 * You need also the header file glut.h in your compiler directory.
 *  
 */

/*
 * 3DS Spaceship created by: Rene Reiter <renereiter@hotmail.com>
 */



#include <windows.h>
#include <GL/glut.h>
#include "tutorial4.h"
#include "texture.h"
#include "3dsloader.h"
#include <cstdlib>
#include <ctime>
#include <time.h>
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

//Now the object is generic, the cube has annoyed us a little bit, or not?
obj_type object,object2;
GLfloat X = 0, Y = -10, Z = 10;
time_t t1, t2;

/**********************************************************
 *
 * SUBROUTINE init()
 *
 * Used to initialize OpenGL and to setup our world
 *
 *********************************************************/
typedef struct {
	float xx, yy, zz;
}ver_type;

// The polygon (triangle), 3 numbers that aim 3 vertices
typedef struct {
	int aa, bb, cc;
}poly_type;

// The mapcoord type, 2 texture coordinates for each vertex
typedef struct {
	float uu, vv;
}map_type;

// The object type
typedef struct {
	ver_type ver[2000];
	poly_type poly[2000];
	map_type map[2000];
	int id_tex;
} obj, *obj_ptr;

obj base =
{
	{
		-100, -100, 100,   // vertex v0
		100,  -100, 100,   // vertex v1
		100,  -100, -100,  // vertex v2
		-100, -100, -100,  // vertex v3
		-100, 100,  100,   // vertex v4
		100,  100,  100,   // vertex v5
		100,  100,  -100,  // vertex v6 
		-100, 100,  -100   // vertex v7
	},
	{
		0, 1, 4,  // polygon v0,v1,v4
		1, 5, 4,  // polygon v1,v5,v4
		1, 2, 5,  // polygon v1,v2,v5
		2, 6, 5,  // polygon v2,v6,v5
		2, 3, 6,  // polygon v2,v3,v6
		3, 7, 6,  // polygon v3,v7,v6
		3, 0, 7,  // polygon v3,v0,v7
		0, 4, 7,  // polygon v0,v4,v7
		4, 5, 7,  // polygon v4,v5,v7
		5, 6, 7,  // polygon v5,v6,v7
		3, 2, 0,  // polygon v3,v2,v0
		2, 1, 0   // polygon v2,v1,v0
	},
	{
		0.0, 0.0,  // mapping coordinates for vertex v0
		1.0, 0.0,  // mapping coordinates for vertex v1
		1.0, 0.0,  // mapping coordinates for vertex v2
		0.0, 0.0,  // mapping coordinates for vertex v3
		0.0, 1.0,  // mapping coordinates for vertex v4
		1.0, 1.0,  // mapping coordinates for vertex v5
		1.0, 1.0,  // mapping coordinates for vertex v6 
		0.0, 1.0   // mapping coordinates for vertex v7
	},
	0,
};
obj cube1 =
{
	{
		-6, -6, 6,   // vertex v0
		6,  -6, 6,   // vertex v1
		6,  -6, -6,  // vertex v2
		-6, -6, -6,  // vertex v3
		-6, 6,  6,   // vertex v4
		6,  6,  6,   // vertex v5
		6,  6,  -6,  // vertex v6 
		-6, 6,  -6   // vertex v7
	},
	{
		0, 1, 4,  // polygon v0,v1,v4
		1, 5, 4,  // polygon v1,v5,v4
		1, 2, 5,  // polygon v1,v2,v5
		2, 6, 5,  // polygon v2,v6,v5
		2, 3, 6,  // polygon v2,v3,v6
		3, 7, 6,  // polygon v3,v7,v6
		3, 0, 7,  // polygon v3,v0,v7
		0, 4, 7,  // polygon v0,v4,v7
		4, 5, 7,  // polygon v4,v5,v7
		5, 6, 7,  // polygon v5,v6,v7
		3, 2, 0,  // polygon v3,v2,v0
		2, 1, 0   // polygon v2,v1,v0
	},
	{
		0.0, 0.0,  // mapping coordinates for vertex v0
		1.0, 0.0,  // mapping coordinates for vertex v1
		1.0, 0.0,  // mapping coordinates for vertex v2
		0.0, 0.0,  // mapping coordinates for vertex v3
		0.0, 1.0,  // mapping coordinates for vertex v4
		1.0, 1.0,  // mapping coordinates for vertex v5
		1.0, 1.0,  // mapping coordinates for vertex v6 
		0.0, 1.0   // mapping coordinates for vertex v7
	},
	0,
};
obj cube2 =
{
	{
		-6, -6, 6,   // vertex v0
		6,  -6, 6,   // vertex v1
		6,  -6, -6,  // vertex v2
		-6, -6, -6,  // vertex v3
		-6, 6,  6,   // vertex v4
		6,  6,  6,   // vertex v5
		6,  6,  -6,  // vertex v6 
		-6, 6,  -6   // vertex v7
	},
	{
		0, 1, 4,  // polygon v0,v1,v4
		1, 5, 4,  // polygon v1,v5,v4
		1, 2, 5,  // polygon v1,v2,v5
		2, 6, 5,  // polygon v2,v6,v5
		2, 3, 6,  // polygon v2,v3,v6
		3, 7, 6,  // polygon v3,v7,v6
		3, 0, 7,  // polygon v3,v0,v7
		0, 4, 7,  // polygon v0,v4,v7
		4, 5, 7,  // polygon v4,v5,v7
		5, 6, 7,  // polygon v5,v6,v7
		3, 2, 0,  // polygon v3,v2,v0
		2, 1, 0   // polygon v2,v1,v0
	},
	{
		0.0, 0.0,  // mapping coordinates for vertex v0
		1.0, 0.0,  // mapping coordinates for vertex v1
		1.0, 0.0,  // mapping coordinates for vertex v2
		0.0, 0.0,  // mapping coordinates for vertex v3
		0.0, 1.0,  // mapping coordinates for vertex v4
		1.0, 1.0,  // mapping coordinates for vertex v5
		1.0, 1.0,  // mapping coordinates for vertex v6 
		0.0, 1.0   // mapping coordinates for vertex v7
	},
	0,
};
obj cube3 =
{
	{
		-6, -6, 6,   // vertex v0
		6,  -6, 6,   // vertex v1
		6,  -6, -6,  // vertex v2
		-6, -6, -6,  // vertex v3
		-6, 6,  6,   // vertex v4
		6,  6,  6,   // vertex v5
		6,  6,  -6,  // vertex v6 
		-6, 6,  -6   // vertex v7
	},
	{
		0, 1, 4,  // polygon v0,v1,v4
		1, 5, 4,  // polygon v1,v5,v4
		1, 2, 5,  // polygon v1,v2,v5
		2, 6, 5,  // polygon v2,v6,v5
		2, 3, 6,  // polygon v2,v3,v6
		3, 7, 6,  // polygon v3,v7,v6
		3, 0, 7,  // polygon v3,v0,v7
		0, 4, 7,  // polygon v0,v4,v7
		4, 5, 7,  // polygon v4,v5,v7
		5, 6, 7,  // polygon v5,v6,v7
		3, 2, 0,  // polygon v3,v2,v0
		2, 1, 0   // polygon v2,v1,v0
	},
	{
		0.0, 0.0,  // mapping coordinates for vertex v0
		1.0, 0.0,  // mapping coordinates for vertex v1
		1.0, 0.0,  // mapping coordinates for vertex v2
		0.0, 0.0,  // mapping coordinates for vertex v3
		0.0, 1.0,  // mapping coordinates for vertex v4
		1.0, 1.0,  // mapping coordinates for vertex v5
		1.0, 1.0,  // mapping coordinates for vertex v6 
		0.0, 1.0   // mapping coordinates for vertex v7
	},
	0,
};
obj cube4 =
{
	{
		-10, -10, 10,   // vertex v0
		10,  -10, 10,   // vertex v1
		10,  -10, -10,  // vertex v2
		-10, -10, -10,  // vertex v3
		-10, 10,  10,   // vertex v4
		10,  10,  10,   // vertex v5
		10,  10,  -10,  // vertex v6 
		-10, 10,  -10   // vertex v7
	},
	{
		0, 1, 4,  // polygon v0,v1,v4
		1, 5, 4,  // polygon v1,v5,v4
		1, 2, 5,  // polygon v1,v2,v5
		2, 6, 5,  // polygon v2,v6,v5
		2, 3, 6,  // polygon v2,v3,v6
		3, 7, 6,  // polygon v3,v7,v6
		3, 0, 7,  // polygon v3,v0,v7
		0, 4, 7,  // polygon v0,v4,v7
		4, 5, 7,  // polygon v4,v5,v7
		5, 6, 7,  // polygon v5,v6,v7
		3, 2, 0,  // polygon v3,v2,v0
		2, 1, 0   // polygon v2,v1,v0
	},
	{
		0.0, 0.0,  // mapping coordinates for vertex v0
		1.0, 0.0,  // mapping coordinates for vertex v1
		1.0, 0.0,  // mapping coordinates for vertex v2
		0.0, 0.0,  // mapping coordinates for vertex v3
		0.0, 1.0,  // mapping coordinates for vertex v4
		1.0, 1.0,  // mapping coordinates for vertex v5
		1.0, 1.0,  // mapping coordinates for vertex v6 
		0.0, 1.0   // mapping coordinates for vertex v7
	},
	0,
};
obj cube5 =
{
	{
		-10, -10, 10,   // vertex v0
		10,  -10, 10,   // vertex v1
		10,  -10, -10,  // vertex v2
		-10, -10, -10,  // vertex v3
		-10, 10,  10,   // vertex v4
		10,  10,  10,   // vertex v5
		10,  10,  -10,  // vertex v6 
		-10, 10,  -10   // vertex v7
	},
	{
		0, 1, 4,  // polygon v0,v1,v4
		1, 5, 4,  // polygon v1,v5,v4
		1, 2, 5,  // polygon v1,v2,v5
		2, 6, 5,  // polygon v2,v6,v5
		2, 3, 6,  // polygon v2,v3,v6
		3, 7, 6,  // polygon v3,v7,v6
		3, 0, 7,  // polygon v3,v0,v7
		0, 4, 7,  // polygon v0,v4,v7
		4, 5, 7,  // polygon v4,v5,v7
		5, 6, 7,  // polygon v5,v6,v7
		3, 2, 0,  // polygon v3,v2,v0
		2, 1, 0   // polygon v2,v1,v0
	},
	{
		0.0, 0.0,  // mapping coordinates for vertex v0
		1.0, 0.0,  // mapping coordinates for vertex v1
		1.0, 0.0,  // mapping coordinates for vertex v2
		0.0, 0.0,  // mapping coordinates for vertex v3
		0.0, 1.0,  // mapping coordinates for vertex v4
		1.0, 1.0,  // mapping coordinates for vertex v5
		1.0, 1.0,  // mapping coordinates for vertex v6 
		0.0, 1.0   // mapping coordinates for vertex v7
	},
	0,
};
void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0); // This clear the background color to black
    glShadeModel(GL_SMOOTH); // Type of shading for the polygons
   	
    // Viewport transformation
    glViewport(0,0,screen_width,screen_height);  
	t1 = clock();
    // Projection transformation
    glMatrixMode(GL_PROJECTION); // Specifies which matrix stack is the target for matrix operations 
    glLoadIdentity(); // We initialize the projection matrix as identity
    gluPerspective(45.0f,(GLfloat)screen_width/(GLfloat)screen_height,10.0f,10000.0f); // We define the "viewing volume"
   
    glEnable(GL_DEPTH_TEST); // We enable the depth test (also called z buffer)
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)
    
    glEnable(GL_TEXTURE_2D); // This Enable the Texture mapping
	char a[] = "fighter2.3ds";
    Load3DS (&object, a);
	char b[] = "falcon.bmp";
    object.id_texture=LoadBitmap(b); // The Function LoadBitmap() return the current texture ID

	glEnable(GL_TEXTURE_2D); // This Enable the Texture mapping
	char c[] = "3-4.bmp";
	cube1.id_tex = LoadBitmap(c); // The Function LoadBitmap() return the current texture ID

	glEnable(GL_TEXTURE_2D); // This Enable the Texture mapping
	char d[] = "24584a936be11a9.bmp";
	base.id_tex = LoadBitmap(d); // The Function LoadBitmap() return the current texture ID

	glEnable(GL_TEXTURE_2D); // This Enable the Texture mapping
	char f[] = "maxresdefault2.bmp";
	cube2.id_tex = LoadBitmap(f); // The Function LoadBitmap() return the current texture ID

	glEnable(GL_TEXTURE_2D); // This Enable the Texture mapping
	char e[] = "maxresdefault.bmp";
	cube3.id_tex = LoadBitmap(e); // The Function LoadBitmap() return the current texture ID

	
    // If the last function returns -1 it means the file was not found so we exit from the program
    if (object.id_texture==-1)
    {
       // MessageBox(NULL,"Image file: spaceshiptexture.bmp not found", "Zetadeck",MB_OK | MB_ICONERROR);
        exit (0);
    }
	srand(time(NULL));
}



/**********************************************************
 *
 * SUBROUTINE resize(int,int)
 *
 * This routine must be called everytime we resize our window.
 * 
 *********************************************************/

void resize (int width, int height)
{
    screen_width=width; // We obtain the new screen width values and store it
    screen_height=height; // Height value

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We clear both the color and the depth buffer so to draw the next frame
    glViewport(0,0,screen_width,screen_height); // Viewport transformation

    glMatrixMode(GL_PROJECTION); // Projection transformation
    glLoadIdentity(); // We initialize the projection matrix as identity
    gluPerspective(45.0f,(GLfloat)screen_width/(GLfloat)screen_height,10.0f,10000.0f);

    glutPostRedisplay (); // This command redraw the scene (it calls the same routine of glutDisplayFunc)
}



/**********************************************************
 *
 * SUBROUTINE keyboard(unsigned char,int,int)
 *
 * Used to handle the keyboard input (ASCII Characters)
 * 
 *********************************************************/

void keyboard (unsigned char key, int x, int y)
{
        
    switch (key)
    {
  
        case ' ':
            rotation_x_increment=0;
            rotation_y_increment=0;
            rotation_z_increment=0;
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
		case 'z': case 'Z':
			Y += 1;
		break;
		case 'x': case 'X':
			Y -= 1;
		break;
    }
}



/**********************************************************
 *
 * SUBROUTINE keyboard(int,int,int)
 *
 * Used to handle the keyboard input (not ASCII Characters)
 * 
 *********************************************************/

void keyboard_s (int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_UP:
			Z -= 1;
            //rotation_x_increment = rotation_x_increment +0.005;
        break;
        case GLUT_KEY_DOWN:
			Z += 1;
            //rotation_x_increment = rotation_x_increment -0.005;
        break;
        case GLUT_KEY_LEFT:
			X -= 1;
            //rotation_y_increment = rotation_y_increment +0.005;
        break;
        case GLUT_KEY_RIGHT:
			X += 1;
            //rotation_y_increment = rotation_y_increment -0.005;
        break;
    }
}



/**********************************************************
 *
 * SUBROUTINE display()
 *
 * This is our main rendering subroutine, called each frame
 * 
 *********************************************************/
int tempx2 = (rand() % 15)+21, tempx3 = (rand() % 15) + 21,tempz1=-(rand()%50)-150, tempz2 = -(rand() % 50) - 150, tempz3 = -(rand() % 50) - 150;
GLfloat COUNT = 1,c=0,
cube1x = 0, cube1y = -10, cube1z = tempz1,
cube2x= -tempx2, cube2y=-10, cube2z=tempz2,
cube3x= tempx3, cube3y=-10, cube3z=tempz3;
int K = -250;
void display(void)
{
	int l_index;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // This clear the background color to dark blue
    glMatrixMode(GL_MODELVIEW); // Modeling transformation
    glLoadIdentity(); // Initialize the model matrix as identity

	glPushMatrix();//base
		glTranslatef(0.0, 0.0, K);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)
		glTranslatef(0.0, 0, 0); // We move the object forward (the model matrix is multiplied by the translation matrix)
		glRotatef(0, 1.0, 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, base.id_tex); // We set the active texture 
		glBegin(GL_TRIANGLES); // GlBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
		for (l_index = 0;l_index < 2;l_index++)
		{
			//----------------- FIRST VERTEX -----------------
			// Texture coordinates of the first vertex
			glTexCoord2f(base.map[base.poly[l_index].aa].uu,
				base.map[base.poly[l_index].aa].vv);
			// Coordinates of the first vertex
			glVertex3f(base.ver[base.poly[l_index].aa].xx,
				base.ver[base.poly[l_index].aa].yy,
				base.ver[base.poly[l_index].aa].zz); //Vertex definition

	//----------------- SECOND VERTEX -----------------
	// Texture coordinates of the second vertex
			glTexCoord2f(base.map[base.poly[l_index].bb].uu,
				base.map[base.poly[l_index].bb].vv);
			// Coordinates of the second vertex
			glVertex3f(base.ver[base.poly[l_index].bb].xx,
				base.ver[base.poly[l_index].bb].yy,
				base.ver[base.poly[l_index].bb].zz);

			//----------------- THIRD VERTEX -----------------
			// Texture coordinates of the third vertex
			glTexCoord2f(base.map[base.poly[l_index].cc].uu,
				base.map[base.poly[l_index].cc].vv);
			// Coordinates of the Third vertex
			glVertex3f(base.ver[base.poly[l_index].cc].xx,
				base.ver[base.poly[l_index].cc].yy,
				base.ver[base.poly[l_index].cc].zz);
		}
		glEnd();
	glPopMatrix();
	
	//¤ÓªÅ²î
	glPushMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Polygon rasterization mode (polygon outlined)
		glTranslatef(0.0, 0.0, -45); // We move the object forward (the model matrix is multiplied by the translation matrix)
		glRotatef(25, 1.0, 0.0, 0.0);
		glTranslatef(X, Y, Z);
		glBindTexture(GL_TEXTURE_2D, object.id_texture); // We set the active texture 
		glBegin(GL_TRIANGLES); // glBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
		for (l_index=0;l_index<object.polygons_qty;l_index++)
		{
			//----------------- FIRST VERTEX -----------------
			// Texture coordinates of the first vertex
			glTexCoord2f( object.mapcoord[ object.polygon[l_index].a ].u,
						  object.mapcoord[ object.polygon[l_index].a ].v);
			// Coordinates of the first vertex
			glVertex3f( object.vertex[ object.polygon[l_index].a ].x,
						object.vertex[ object.polygon[l_index].a ].y,
						object.vertex[ object.polygon[l_index].a ].z); //Vertex definition

			//----------------- SECOND VERTEX -----------------
			// Texture coordinates of the second vertex
			glTexCoord2f( object.mapcoord[ object.polygon[l_index].b ].u,
						  object.mapcoord[ object.polygon[l_index].b ].v);
			// Coordinates of the second vertex
			glVertex3f( object.vertex[ object.polygon[l_index].b ].x,
						object.vertex[ object.polygon[l_index].b ].y,
						object.vertex[ object.polygon[l_index].b ].z);
        
			//----------------- THIRD VERTEX -----------------
			// Texture coordinates of the third vertex
			glTexCoord2f( object.mapcoord[ object.polygon[l_index].c ].u,
						  object.mapcoord[ object.polygon[l_index].c ].v);
			// Coordinates of the Third vertex
			glVertex3f( object.vertex[ object.polygon[l_index].c ].x,
						object.vertex[ object.polygon[l_index].c ].y,
						object.vertex[ object.polygon[l_index].c ].z);
		}
		glEnd();
	glPopMatrix();

	glPushMatrix();//cube1
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)
	glTranslatef(cube1x, cube1y, cube1z); // We move the object forward (the model matrix is multiplied by the translation matrix)
	glRotatef(-45, 1.0, 0.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, cube1.id_tex); // We set the active texture 
	glBegin(GL_TRIANGLES); // GlBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
	for (l_index = 0;l_index < 2;l_index++)
	{
		//----------------- FIRST VERTEX -----------------
		// Texture coordinates of the first vertex
		glTexCoord2f(cube1.map[cube1.poly[l_index].aa].uu,
			cube1.map[cube1.poly[l_index].aa].vv);
		// Coordinates of the first vertex
		glVertex3f(cube1.ver[cube1.poly[l_index].aa].xx,
			cube1.ver[cube1.poly[l_index].aa].yy,
			cube1.ver[cube1.poly[l_index].aa].zz); //Vertex definition

//----------------- SECOND VERTEX -----------------
// Texture coordinates of the second vertex
		glTexCoord2f(cube1.map[cube1.poly[l_index].bb].uu,
			cube1.map[cube1.poly[l_index].bb].vv);
		// Coordinates of the second vertex
		glVertex3f(cube1.ver[cube1.poly[l_index].bb].xx,
			cube1.ver[cube1.poly[l_index].bb].yy,
			cube1.ver[cube1.poly[l_index].bb].zz);

		//----------------- THIRD VERTEX -----------------
		// Texture coordinates of the third vertex
		glTexCoord2f(cube1.map[cube1.poly[l_index].cc].uu,
			cube1.map[cube1.poly[l_index].cc].vv);
		// Coordinates of the Third vertex
		glVertex3f(cube1.ver[cube1.poly[l_index].cc].xx,
			cube1.ver[cube1.poly[l_index].cc].yy,
			cube1.ver[cube1.poly[l_index].cc].zz);
	}
	for (l_index = 2;l_index < 12;l_index++)
	{
		//----------------- FIRST VERTEX -----------------
		// Texture coordinates of the first vertex
		glTexCoord2f(cube1.map[cube1.poly[l_index].aa].uu,
			cube1.map[cube1.poly[l_index].aa].vv);
		// Coordinates of the first vertex
		glVertex3f(cube1.ver[cube1.poly[l_index].aa].xx,
			cube1.ver[cube1.poly[l_index].aa].yy,
			cube1.ver[cube1.poly[l_index].aa].zz); //Vertex definition

//----------------- SECOND VERTEX -----------------
// Texture coordinates of the second vertex
		glTexCoord2f(cube1.map[cube1.poly[l_index].bb].uu,
			cube1.map[cube1.poly[l_index].bb].vv);
		// Coordinates of the second vertex
		glVertex3f(cube1.ver[cube1.poly[l_index].bb].xx,
			cube1.ver[cube1.poly[l_index].bb].yy,
			cube1.ver[cube1.poly[l_index].bb].zz);

		//----------------- THIRD VERTEX -----------------
		// Texture coordinates of the third vertex
		glTexCoord2f(cube1.map[cube1.poly[l_index].cc].uu,
			cube1.map[cube1.poly[l_index].cc].vv);
		// Coordinates of the Third vertex
		glVertex3f(cube1.ver[cube1.poly[l_index].cc].xx,
			cube1.ver[cube1.poly[l_index].cc].yy,
			cube1.ver[cube1.poly[l_index].cc].zz);
	}
	glEnd();
	glPopMatrix();

	glPushMatrix();//cube2
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)
	glTranslatef(cube2x, cube2y, cube2z); // We move the object forward (the model matrix is multiplied by the translation matrix)
	glRotatef(-45, 1.0, 0.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, cube2.id_tex); // We set the active texture 
	glBegin(GL_TRIANGLES); // GlBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
	for (l_index = 0;l_index < 2;l_index++)
	{
		//----------------- FIRST VERTEX -----------------
		// Texture coordinates of the first vertex
		glTexCoord2f(cube2.map[cube2.poly[l_index].aa].uu,
			cube2.map[cube2.poly[l_index].aa].vv);
		// Coordinates of the first vertex
		glVertex3f(cube2.ver[cube2.poly[l_index].aa].xx,
			cube2.ver[cube2.poly[l_index].aa].yy,
			cube2.ver[cube2.poly[l_index].aa].zz); //Vertex definition

//----------------- SECOND VERTEX -----------------
// Texture coordinates of the second vertex
		glTexCoord2f(cube2.map[cube2.poly[l_index].bb].uu,
			cube2.map[cube2.poly[l_index].bb].vv);
		// Coordinates of the second vertex
		glVertex3f(cube2.ver[cube2.poly[l_index].bb].xx,
			cube2.ver[cube2.poly[l_index].bb].yy,
			cube2.ver[cube2.poly[l_index].bb].zz);

		//----------------- THIRD VERTEX -----------------
		// Texture coordinates of the third vertex
		glTexCoord2f(cube2.map[cube2.poly[l_index].cc].uu,
			cube2.map[cube2.poly[l_index].cc].vv);
		// Coordinates of the Third vertex
		glVertex3f(cube2.ver[cube2.poly[l_index].cc].xx,
			cube2.ver[cube2.poly[l_index].cc].yy,
			cube2.ver[cube2.poly[l_index].cc].zz);
	}
	for (l_index = 2;l_index < 12;l_index++)
	{
		//----------------- FIRST VERTEX -----------------
		// Texture coordinates of the first vertex
		glTexCoord2f(cube2.map[cube2.poly[l_index].aa].uu,
			cube2.map[cube2.poly[l_index].aa].vv);
		// Coordinates of the first vertex
		glVertex3f(cube2.ver[cube2.poly[l_index].aa].xx,
			cube2.ver[cube2.poly[l_index].aa].yy,
			cube2.ver[cube2.poly[l_index].aa].zz); //Vertex definition

//----------------- SECOND VERTEX -----------------
// Texture coordinates of the second vertex
		glTexCoord2f(cube2.map[cube2.poly[l_index].bb].uu,
			cube2.map[cube2.poly[l_index].bb].vv);
		// Coordinates of the second vertex
		glVertex3f(cube2.ver[cube2.poly[l_index].bb].xx,
			cube2.ver[cube2.poly[l_index].bb].yy,
			cube2.ver[cube2.poly[l_index].bb].zz);

		//----------------- THIRD VERTEX -----------------
		// Texture coordinates of the third vertex
		glTexCoord2f(cube2.map[cube2.poly[l_index].cc].uu,
			cube2.map[cube2.poly[l_index].cc].vv);
		// Coordinates of the Third vertex
		glVertex3f(cube2.ver[cube2.poly[l_index].cc].xx,
			cube2.ver[cube2.poly[l_index].cc].yy,
			cube2.ver[cube2.poly[l_index].cc].zz);
	}
	glEnd();
	glPopMatrix();

	glPushMatrix();//cube3
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)
	glTranslatef(cube3x, cube3y, cube3z); // We move the object forward (the model matrix is multiplied by the translation matrix)
	glRotatef(-45, 1.0, 0.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, cube3.id_tex); // We set the active texture 
	glBegin(GL_TRIANGLES); // GlBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
	for (l_index = 0;l_index < 2;l_index++)
	{
		//----------------- FIRST VERTEX -----------------
		// Texture coordinates of the first vertex
		glTexCoord2f(cube3.map[cube3.poly[l_index].aa].uu,
			cube3.map[cube3.poly[l_index].aa].vv);
		// Coordinates of the first vertex
		glVertex3f(cube3.ver[cube3.poly[l_index].aa].xx,
			cube3.ver[cube3.poly[l_index].aa].yy,
			cube3.ver[cube3.poly[l_index].aa].zz); //Vertex definition

//----------------- SECOND VERTEX -----------------
// Texture coordinates of the second vertex
		glTexCoord2f(cube3.map[cube3.poly[l_index].bb].uu,
			cube3.map[cube3.poly[l_index].bb].vv);
		// Coordinates of the second vertex
		glVertex3f(cube3.ver[cube3.poly[l_index].bb].xx,
			cube3.ver[cube3.poly[l_index].bb].yy,
			cube3.ver[cube3.poly[l_index].bb].zz);

		//----------------- THIRD VERTEX -----------------
		// Texture coordinates of the third vertex
		glTexCoord2f(cube3.map[cube3.poly[l_index].cc].uu,
			cube3.map[cube3.poly[l_index].cc].vv);
		// Coordinates of the Third vertex
		glVertex3f(cube3.ver[cube3.poly[l_index].cc].xx,
			cube3.ver[cube3.poly[l_index].cc].yy,
			cube3.ver[cube3.poly[l_index].cc].zz);
	}
	for (l_index = 2;l_index < 12;l_index++)
	{
		//----------------- FIRST VERTEX -----------------
		// Texture coordinates of the first vertex
		glTexCoord2f(cube3.map[cube3.poly[l_index].aa].uu,
			cube3.map[cube3.poly[l_index].aa].vv);
		// Coordinates of the first vertex
		glVertex3f(cube3.ver[cube3.poly[l_index].aa].xx,
			cube3.ver[cube3.poly[l_index].aa].yy,
			cube3.ver[cube3.poly[l_index].aa].zz); //Vertex definition

//----------------- SECOND VERTEX -----------------
// Texture coordinates of the second vertex
		glTexCoord2f(cube3.map[cube3.poly[l_index].bb].uu,
			cube3.map[cube3.poly[l_index].bb].vv);
		// Coordinates of the second vertex
		glVertex3f(cube3.ver[cube3.poly[l_index].bb].xx,
			cube3.ver[cube3.poly[l_index].bb].yy,
			cube3.ver[cube3.poly[l_index].bb].zz);

		//----------------- THIRD VERTEX -----------------
		// Texture coordinates of the third vertex
		glTexCoord2f(cube3.map[cube3.poly[l_index].cc].uu,
			cube3.map[cube3.poly[l_index].cc].vv);
		// Coordinates of the Third vertex
		glVertex3f(cube3.ver[cube3.poly[l_index].cc].xx,
			cube3.ver[cube3.poly[l_index].cc].yy,
			cube3.ver[cube3.poly[l_index].cc].zz);
	}
	glEnd();
	glPopMatrix();

	//cube1y-= COUNT;
	//cube2y-= COUNT;
	//cube3y-= COUNT;
	cube1z++;
	cube2z++;
	cube3z++;
	if (abs(X - cube1x) < 10 && abs(Y - cube1y) < 10 && abs(Z - cube1z) < 10)
	{
		cube1x = -2147483647, cube1y = -2147483647, cube1z = 100;
		cube2x = -2147483647, cube2y = -2147483647, cube2z = 100;
		cube3x = -2147483647, cube3y = -2147483647, cube3z = 100;
		glEnable(GL_TEXTURE_2D); // This Enable the Texture mapping
		char d[] = "images1.bmp";
		base.id_tex = LoadBitmap(d); // The Function LoadBitmap() return the current texture ID
		K = -249;
	}
	else if (abs(X - cube2x) < 10 && abs(Y - cube2y) < 10 && abs(Z - cube2z) < 10)
	{
		cube1x = -2147483647, cube1y = -2147483647, cube1z = 100;
		cube2x = -2147483647, cube2y = -2147483647, cube2z = 100;
		cube3x = -2147483647, cube3y = -2147483647, cube3z = 100;
		glEnable(GL_TEXTURE_2D); // This Enable the Texture mapping
		char d[] = "images1.bmp";
		base.id_tex = LoadBitmap(d); // The Function LoadBitmap() return the current texture ID
		K = -249;
	}
	else if (abs(X - cube3x) < 10 && abs(Y - cube3y) < 10 && abs(Z - cube3z) < 10)
	{
		cube1x = -2147483647, cube1y = 0, cube1z = 100;
		cube2x = -2147483647, cube2y = 0, cube2z = 100;
		cube3x = -2147483647, cube3y = 0, cube3z = 100;
		glEnable(GL_TEXTURE_2D); // This Enable the Texture mapping
		char d[] = "images1.bmp";
		base.id_tex = LoadBitmap(d); // The Function LoadBitmap() return the current texture ID
		K = -249;
	}
	if (cube1z == 50)
	{
		cube1x = rand() % 10;
		cube1y = rand()%20-10;
		cube1z = -(rand() % 50) - 150;
		c++;
	}
	if (cube2z == 50)
	{
		cube2x = -(rand() % 15) - 21;
		cube2y = rand()%20-10;
		cube2z = -(rand() % 50) - 150;
	}
	if (cube3z == 50)
	{
		cube3x = (rand() % 15) + 21;
		cube3y = rand()%20-10;
		cube3z = -(rand() % 50) - 150;
	}
	if (c == 12)
	{
		c = 0;
		COUNT +=2;
	}
	t2=clock();
	if (t2 - t1 > 30000)
	{
		cube1x = -2147483647, cube1y = 0, cube1z = 100;
		cube2x = -2147483647, cube2y = 0, cube2z = 100;
		cube3x = -2147483647, cube3y = 0, cube3z = 100;
		glEnable(GL_TEXTURE_2D); // This Enable the Texture mapping
		char d[] = "zVi0P6Gl.bmp";
		base.id_tex = LoadBitmap(d); // The Function LoadBitmap() return the current texture ID
		K = -249;
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
    glutCreateWindow("www.spacesimulator.net - 3d engine tutorials: Tutorial 4");    
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc (resize);
    glutKeyboardFunc (keyboard);
    glutSpecialFunc (keyboard_s);
    init();
    glutMainLoop();

    return(0);    
}

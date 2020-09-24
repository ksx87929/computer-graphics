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
 * Object functions
 *  
 */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "load_3ds.h"
#include "load_bmp.h"
#include "object.h"
#include "load_ini.h"



/*
 * VARIABLES DECLARATION
 * 
 */

obj_type object[MAX_OBJECTS]; //Now the object is generic, the cube has annoyed us a little bit...
int obj_qty=0; //Number of objects in the world
int obj_control=0; //Number of the object that we can control



/*
 * char ObjLoad(char *p_object_name, char *p_texture_name, float p_pos_x, float p_pos_y, float p_pos_z, float p_rot_x, float p_rot_y, float p_rot_z)
 *
 * This function loads an object and set some parameters 
 *
 * Parameters: p_object_name = object name
 *			   p_texture_name = texture name
 *             p_pos_x = starting position x coordinate
 *             p_pos_y = starting position y coordinate
 *             p_pos_z = starting position z coordinate
 *             p_rot_x = starting orientation x axis
 *             p_rot_y = starting orientation y axis
 *             p_rot_z = starting orientation z axis
 *
 * Return value: (char) 1 if the object was loaded correctly, 0 otherwise
 *
 */

char ObjLoad(char *p_object_name, char *p_texture_name, float p_pos_x, float p_pos_y, float p_pos_z, float p_rot_x, float p_rot_y, float p_rot_z)
{
    if (Load3DS (&object[obj_qty],p_object_name)==0) return(0); //Object loading
    object[obj_qty].id_texture=LoadBMP(p_texture_name); // The Function LoadBitmap() returns the current texture ID
	ObjCalcNormals(&object[obj_qty]); //As soon as we have all the object data we need to calculate all the normals of the object's vertices
	MatrIdentity_4x4(object[obj_qty].matrix); //Object matrix initialization
	ObjPosition(&object[obj_qty], p_pos_x, p_pos_y, p_pos_z); // Object initial position
	ObjRotate(&object[obj_qty], 1, p_rot_x, p_rot_y, p_rot_z); // Object initial rotation
	obj_qty++; // Let's increase the object number and get ready to load another object!
	return (1); // If all is ok then return 1
}



/*
 * char ObjLoadFromIni(char *p_ini_filename, char *p_object_name)
 *
 * This function loads an object using the configuration read from an INI file
 *
 * Parameters: p_ini_filename = ini filename to load
 *			   p_object_name = object name to assign
 *
 * Return value: (char) 1 if the object was loaded correctly, 0 otherwise
 *
 */

char ObjLoadFromIni(char *p_ini_filename, char *p_object_name)
{	
	char *l_string;
	char l_string2[LOADINI_MAX_CHARS];
	float l_x,l_y,l_z;
	int i,j;

    l_string=LoadINI_GetString(p_ini_filename,p_object_name,"filename",""); // Get the object filename
    if (Load3DS (&object[obj_qty],l_string)==0) return(0); // Object loading

	strcpy(object[obj_qty].name,p_object_name);// Object name assignment

    l_string=LoadINI_GetString(p_ini_filename,p_object_name,"texture",""); // Get the texture filename
	object[obj_qty].id_texture=LoadBMP(l_string); // The Function LoadBitmap() returns the current texture ID

	ObjCalcNormals(&object[obj_qty]); // As soon as we have all the object data we need to calculate all the normals of the object's vertices
	ObjCreateBSphere(&object[obj_qty]); // Calculating AABB and BSphere

	MatrIdentity_4x4(object[obj_qty].matrix); // Object matrix initialization

	if (LoadINI_GetInt(p_ini_filename,p_object_name,"use_euler_or_matrix",0)==0)
	{
		// Use euler position
		l_x=(float)LoadINI_GetFloat(p_ini_filename,p_object_name,"position_x",0);
		l_y=(float)LoadINI_GetFloat(p_ini_filename,p_object_name,"position_y",0);
		l_z=(float)LoadINI_GetFloat(p_ini_filename,p_object_name,"position_z",0);
 		ObjPosition(&object[obj_qty], l_x, l_y, l_z); // Object initial position

		// Use euler angles
		l_x=(float)LoadINI_GetFloat(p_ini_filename,p_object_name,"rotation_x",0);
		l_y=(float)LoadINI_GetFloat(p_ini_filename,p_object_name,"rotation_y",0);
		l_z=(float)LoadINI_GetFloat(p_ini_filename,p_object_name,"rotation_z",0);
		ObjRotate(&object[obj_qty], 1, l_x, l_y, l_z); // Object initial rotation
	}
	else
	{
		// Use matrix
		for (i=0;i<4;i++)
		{
			for (j=0;j<4;j++)
			{
				sprintf(l_string2,"matrix_%d%d",i,j);
				object[obj_qty].matrix[i][j]=(float)LoadINI_GetFloat(p_ini_filename,p_object_name,l_string2,0);
			}
		}
	}
	object[obj_qty].mass = (float) LoadINI_GetFloat(p_ini_filename,p_object_name,"mass",1); // Get the mass
	obj_qty++; // Let's increase the object number and get ready to load another object!
	return (1); // If all is ok then return 1
}



/*
 * void ObjSaveToIni(char *p_ini_filename, char *p_object_name)
 *
 * This function saves the current position of the object into the INI array
 * To save it permanently you must call also the function: LoadINI_Save
 *
 * Parameters: p_ini_filename = ini filename to load
 *			   p_object_name = object name to assign
 *
 */

void ObjSaveToIni(char *p_ini_filename, char *p_object_name)
{	
	char l_string[LOADINI_MAX_CHARS];
	int i,j,k;

	for(i=0;i<obj_qty;i++) // For each object
	{
		if (!strcmp(object[i].name,p_object_name)) // Check the name
		{
			LoadINI_SetInt(p_ini_filename,p_object_name,"use_euler_or_matrix",1); // Use matrix notation
			for (j=0;j<4;j++)
			{
				for (k=0;k<4;k++)
				{
					// Store the matrix into the INI Dictionary
					sprintf(l_string,"matrix_%d%d",j,k); 
					LoadINI_SetFloat(p_ini_filename,p_object_name,l_string,object[i].matrix[j][k]);
				}
			}
			break; // Object found, exit from the cycle
		}
	}
}



/*
 * void ObjCreateBSphere(obj_type_ptr p_object)
 *
 * This function calculates the object Bounding sphere (it uses the object's AABB)
 *
 * Input parameters: p_object = object
 *
 */

void ObjCreateBSphere(obj_type_ptr p_object)
{
	p3d_type l_vector;
	
	//AABB creation
	ObjCreateAABB(p_object);

	//Object center (It uses the extreme points of the AABB)
	p_object->bsphere_center.x = (p_object->aabb[6].x + p_object->aabb[0].x)/2;
	p_object->bsphere_center.y = (p_object->aabb[6].y + p_object->aabb[0].y)/2;
	p_object->bsphere_center.z = (p_object->aabb[6].z + p_object->aabb[0].z)/2;
	
    //Object radius (It uses the extreme points of the AABB)
	VectCreate(&p_object->aabb[0],&p_object->aabb[6],&l_vector);
	p_object->bsphere_radius = VectLength(&l_vector)/2;
}



/*
 * void ObjCreateAABB(obj_type_ptr p_object)
 *
 * This function calculates the object starting AABB
 *
 * Input parameters: p_object = object
 *
 */

void ObjCreateAABB(obj_type_ptr p_object)
{
	int i;

	// At first store the first vertex into the first AABB vertex
	for (i=0; i<8; i++)
	{
		p_object->aabb[i].x = p_object->vertex[0].x;
		p_object->aabb[i].y = p_object->vertex[0].y;
		p_object->aabb[i].z = p_object->vertex[0].z;
	}

	// Now get the 8 AABB vertex making some comparisons to get the 8 extreme vertices of the object
	for (i=1; i<p_object->vertices_qty; i++)
	{
		if (p_object->vertex[i].x < p_object->aabb[0].x) p_object->aabb[0].x = p_object->vertex[i].x;
		if (p_object->vertex[i].y < p_object->aabb[0].y) p_object->aabb[0].y = p_object->vertex[i].y;
		if (p_object->vertex[i].z < p_object->aabb[0].z) p_object->aabb[0].z = p_object->vertex[i].z;

		if (p_object->vertex[i].x > p_object->aabb[1].x) p_object->aabb[1].x = p_object->vertex[i].x;
		if (p_object->vertex[i].y < p_object->aabb[1].y) p_object->aabb[1].y = p_object->vertex[i].y;
		if (p_object->vertex[i].z < p_object->aabb[1].z) p_object->aabb[1].z = p_object->vertex[i].z;

	 	if (p_object->vertex[i].x > p_object->aabb[2].x) p_object->aabb[2].x = p_object->vertex[i].x;
		if (p_object->vertex[i].y > p_object->aabb[2].y) p_object->aabb[2].y = p_object->vertex[i].y;
		if (p_object->vertex[i].z < p_object->aabb[2].z) p_object->aabb[2].z = p_object->vertex[i].z;

		if (p_object->vertex[i].x < p_object->aabb[3].x) p_object->aabb[3].x = p_object->vertex[i].x;
		if (p_object->vertex[i].y > p_object->aabb[3].y) p_object->aabb[3].y = p_object->vertex[i].y;
		if (p_object->vertex[i].z < p_object->aabb[3].z) p_object->aabb[3].z = p_object->vertex[i].z;

		if (p_object->vertex[i].x < p_object->aabb[4].x) p_object->aabb[4].x = p_object->vertex[i].x;
		if (p_object->vertex[i].y < p_object->aabb[4].y) p_object->aabb[4].y = p_object->vertex[i].y;
		if (p_object->vertex[i].z > p_object->aabb[4].z) p_object->aabb[4].z = p_object->vertex[i].z;
	
		if (p_object->vertex[i].x > p_object->aabb[5].x) p_object->aabb[5].x = p_object->vertex[i].x;
		if (p_object->vertex[i].y < p_object->aabb[5].y) p_object->aabb[5].y = p_object->vertex[i].y;
		if (p_object->vertex[i].z > p_object->aabb[5].z) p_object->aabb[5].z = p_object->vertex[i].z;

		if (p_object->vertex[i].x > p_object->aabb[6].x) p_object->aabb[6].x = p_object->vertex[i].x;
		if (p_object->vertex[i].y > p_object->aabb[6].y) p_object->aabb[6].y = p_object->vertex[i].y;
		if (p_object->vertex[i].z > p_object->aabb[6].z) p_object->aabb[6].z = p_object->vertex[i].z;

		if (p_object->vertex[i].x < p_object->aabb[7].x) p_object->aabb[7].x = p_object->vertex[i].x;
		if (p_object->vertex[i].y > p_object->aabb[7].y) p_object->aabb[7].y = p_object->vertex[i].y;
		if (p_object->vertex[i].z > p_object->aabb[7].z) p_object->aabb[7].z = p_object->vertex[i].z;	
	}
}



/*
 * void ObjCalcNormals(obj_type_ptr p_object)
 *
 * This function calculates all the polygons and vertices' normals of the specified object
 *
 * Input parameters: p_object = object
 *
 */

void ObjCalcNormals(obj_type_ptr p_object)
{
	int i;
	p3d_type l_vect1,l_vect2,l_vect3,l_vect_b1,l_vect_b2,l_normal;  //Some local vectors
	int l_connections_qty[MAX_VERTICES]; //Number of poligons around each vertex

    // Resetting vertices' normals...
	for (i=0; i<p_object->vertices_qty; i++)
	{
		p_object->normal[i].x = 0.0;
		p_object->normal[i].y = 0.0;
		p_object->normal[i].z = 0.0;
		l_connections_qty[i]=0;
	}
	
	for (i=0; i<p_object->polygons_qty; i++)
	{
        l_vect1.x = p_object->vertex[p_object->polygon[i].a].x;
        l_vect1.y = p_object->vertex[p_object->polygon[i].a].y;
        l_vect1.z = p_object->vertex[p_object->polygon[i].a].z;
        l_vect2.x = p_object->vertex[p_object->polygon[i].b].x;
        l_vect2.y = p_object->vertex[p_object->polygon[i].b].y;
        l_vect2.z = p_object->vertex[p_object->polygon[i].b].z;
        l_vect3.x = p_object->vertex[p_object->polygon[i].c].x;
        l_vect3.y = p_object->vertex[p_object->polygon[i].c].y;
        l_vect3.z = p_object->vertex[p_object->polygon[i].c].z;         
  
        // Polygon normal calculation
		VectCreate (&l_vect1, &l_vect2, &l_vect_b1); // Vector from the first vertex to the second one
        VectCreate (&l_vect1, &l_vect3, &l_vect_b2); // Vector from the first vertex to the third one
		VectNormalize(&l_vect_b1);
		VectNormalize(&l_vect_b2);
        VectDotProduct (&l_vect_b1, &l_vect_b2, &l_normal); // Dot product between the two vectors
        VectNormalize (&l_normal); //Normalizing the resultant we obtain the polygon normal

		// For each vertex shared by this polygon we increase the number of connections
		l_connections_qty[p_object->polygon[i].a]+=1;
		l_connections_qty[p_object->polygon[i].b]+=1;
		l_connections_qty[p_object->polygon[i].c]+=1;

		// For each vertex shared by this polygon we add the polygon normal
		p_object->normal[p_object->polygon[i].a].x+=l_normal.x;
		p_object->normal[p_object->polygon[i].a].y+=l_normal.y;
		p_object->normal[p_object->polygon[i].a].z+=l_normal.z;
		p_object->normal[p_object->polygon[i].b].x+=l_normal.x;
		p_object->normal[p_object->polygon[i].b].y+=l_normal.y;
		p_object->normal[p_object->polygon[i].b].z+=l_normal.z;
		p_object->normal[p_object->polygon[i].c].x+=l_normal.x;
		p_object->normal[p_object->polygon[i].c].y+=l_normal.y;
		p_object->normal[p_object->polygon[i].c].z+=l_normal.z;	
	}	
	
    for (i=0; i<p_object->vertices_qty; i++)
	{
		if (l_connections_qty[i]>0)
		{
			// Let's now average the polygons' normals to obtain the vertex normal!
			p_object->normal[i].x /= l_connections_qty[i]; 
			p_object->normal[i].y /= l_connections_qty[i];
			p_object->normal[i].z /= l_connections_qty[i];
		}
	}
}



/*
 * void ObjPosition (obj_type_ptr p_object,float p_x,float p_y,float p_z)
 *
 * Object position (world axis system)
 *
 * Input parameters: p_object = object to move
 *                   p_x = x coordinate
 *					 p_y = y coordinate
 *                   p_z = z coordinate
 *
 */

void ObjPosition (obj_type_ptr p_object,float p_x,float p_y,float p_z)
{
	//The position fields in the object matrix are filled with the new values
    p_object->matrix[3][0]=p_x;
    p_object->matrix[3][1]=p_y;
    p_object->matrix[3][2]=p_z;    
}



/*
 * void ObjTranslate (obj_type_ptr p_object,float p_x,float p_y,float p_z)
 *
 * Object translation (local axis system)
 *
 * Input parameters: p_object = object to translate
 *                   p_x = x coordinate
 *					 p_y = y coordinate
 *                   p_z = z coordinate
 *
 */

void ObjTranslate (obj_type_ptr p_object,float p_x,float p_y,float p_z)
{
    int j,k;
    matrix_4x4_type l_matrix, l_res;

    MatrIdentity_4x4(l_matrix);
    l_matrix[3][0]=p_x;
    l_matrix[3][1]=p_y;
    l_matrix[3][2]=p_z;

	//The object matrix is multiplied by a translation matrix
    MatrMul_4x4_4x4(l_matrix,p_object->matrix,l_res);
    for(j=0;j<4;j++)
      for(k=0;k<4;k++)
        p_object->matrix[j][k]=l_res[j][k];
}



/*
 * void ObjTranslateW (obj_type_ptr p_object,float p_x,float p_y,float p_z)
 *
 * Object translation (world axis system)
 *
 * Input parameters: p_object = object
 *                   p_x = x coordinate
 *					 p_y = y coordinate
 *                   p_z = z coordinate
 *
 */

void ObjTranslateW (obj_type_ptr p_object,float p_x,float p_y,float p_z)
{
      p_object->matrix[3][0]+=p_x;
      p_object->matrix[3][1]+=p_y;
      p_object->matrix[3][2]+=p_z;
}



/*
 *
 * void ObjRotate (obj_type_ptr p_object, char p_use_degrees, float p_angle_x,float p_angle_y,float p_angle_z)
 *
 * Rotates the object around it's local axis
 *
 * Input parameters: p_object = object
 *                   p_use_degrees = use radiants or degrees
 *                   p_angle_x = x angle
 *					 p_angle_y = y angle
 *                   p_angle_z = z angle
 *
 */

void ObjRotate (obj_type_ptr p_object, char p_use_degrees, float p_angle_x,float p_angle_y,float p_angle_z)
{
    matrix_4x4_type l_matrix, l_res;
    float l_rotcos,l_rotsin;
	float l_factor=1.0;
    
	if (p_use_degrees)
	{
		l_factor=(float) (3.1415/180.0);
	}
    
    // X Rotation
	if (p_angle_x!=0.0)
    {
        // Matrix creation
        MatrIdentity_4x4(l_matrix);      
        l_rotcos=(float) cos(p_angle_x*l_factor);
        l_rotsin=(float) sin(p_angle_x*l_factor);  
        l_matrix[1][1]=l_rotcos;
        l_matrix[1][2]=l_rotsin;
        l_matrix[2][1]=-l_rotsin; 
        l_matrix[2][2]=l_rotcos;
		// Matrix concatenation and assignments
        MatrMul_4x4_4x4(l_matrix,p_object->matrix,l_res);
        MatrCopy_4x4(p_object->matrix,l_res);
    }
    // Y Rotation
    if (p_angle_y!=0.0)
    {
        // Matrix creation
        MatrIdentity_4x4(l_matrix);  
        l_rotcos=(float) cos(p_angle_y*l_factor);
        l_rotsin=(float) sin(p_angle_y*l_factor);
        l_matrix[0][0]=l_rotcos; 
        l_matrix[0][2]=-l_rotsin; 
        l_matrix[2][0]=l_rotsin; 
        l_matrix[2][2]=l_rotcos;
		// Matrix concatenation and assignments
        MatrMul_4x4_4x4(l_matrix,p_object->matrix,l_res);
        MatrCopy_4x4(p_object->matrix,l_res);
   
    }
    // Z Rotation
    if (p_angle_z!=0.0)
    {
        // Matrix creation
        MatrIdentity_4x4(l_matrix);  
		l_rotcos=(float) cos(p_angle_z*l_factor);
        l_rotsin=(float) sin(p_angle_z*l_factor);
        l_matrix[0][0]=l_rotcos; 
        l_matrix[0][1]=l_rotsin; 
        l_matrix[1][0]=-l_rotsin; 
        l_matrix[1][1]=l_rotcos;
		// Matrix concatenation and assignments
        MatrMul_4x4_4x4(l_matrix,p_object->matrix,l_res);
        MatrCopy_4x4(p_object->matrix,l_res);
    }
}



/*
 * void ObjAccelerate (obj_type_ptr p_object, char p_axis_system, float p_ax, float p_ay,float p_az)
 *
 * Accelerate the object (units are in m/s)
 *
 * Input parameters: p_object = object
 *                   p_axis_system = use world (0) or object-local (1) coordinates
 *                   p_ax = x acceleration
 *					 p_ay = y acceleration
 *                   p_az = z acceleration
 *
 */

void ObjAccelerate (obj_type_ptr p_object, char p_axis_system, float p_ax, float p_ay,float p_az)
{
    matrix_1x4_type l_direction,l_result;

	//World
	if (p_axis_system==0)
	{
		p_object->lin_speed.x+=p_ax;
		p_object->lin_speed.y+=p_ay;
		p_object->lin_speed.z+=p_az;
	}
	else //object-local axis system
	{
		l_direction[0]=p_ax;
		l_direction[1]=p_ay;
		l_direction[2]=p_az;
		l_direction[3]=0;
    
		MatrMul_1x4_4x4 (l_direction,p_object->matrix,l_result);
		p_object->lin_speed.x+=l_result[0];
		p_object->lin_speed.y+=l_result[1];
		p_object->lin_speed.z+=l_result[2];
	}
}



/*
 * void ObjAccelerateRotation (obj_type_ptr p_object,float p_vrx,float p_vry,float p_vrz)
 *
 * Accelerate the angular speed of the object (units are in rad/s)
 *
 * Input parameters: p_object = object
 *                   p_vrx = x acceleration
 *					 p_vry = y acceleration
 *                   p_vrz = z acceleration
 *
 */

void ObjAccelerateRotation (obj_type_ptr p_object,float p_vrx,float p_vry,float p_vrz)
{
    p_object->rot_speed.x+=p_vrx;
    p_object->rot_speed.y+=p_vry;
    p_object->rot_speed.z+=p_vrz;
}



/*
 * void ObjForce(obj_type_ptr p_object, char p_axis_system, float p_ix, float p_iy, float p_iz)
 *
 * Applies force to the object (units are in Newton)
 * 1 Newton = 1 kg -> 1 m/s
 * The result is: Acceleration = Force / Object-Mass
 *
 * Input parameters: p_object = object
 *                   p_axis_system = use world (0) or object-local (1) coordinates
 *                   p_ix = x force
 *					 p_iy = y force
 *                   p_iz = z force
 *
 */

void ObjForce(obj_type_ptr p_object, char p_axis_system, float p_ix, float p_iy, float p_iz)
{
	if (p_object->mass!=0)
	ObjAccelerate(p_object,p_axis_system, p_ix/p_object->mass,
		p_iy/p_object->mass,
		p_iz/p_object->mass);    
}



/*
 * void ObjTorque (obj_type_ptr p_object,float p_mx,float p_my,float p_mz)
 *
 * Applies Torque to the object
 *
 * The result is: Angular Acceleration = Torque / Moment of inertia
 *
 * Input parameters: p_object = object
 *                   p_axis_system = use world (0) or object-local (1) coordinates
 *                   p_mx = x torque
 *					 p_my = y torque
 *                   p_mz = z torque
 *
 * Some math...
 * Torque = F dot r
 * Angular Momentum = P dot r
 * Angular Momentum = mv dot r
 * Angular Momentum = mWr dot r
 * Angular Momentum = mr^2W
 * Moment of inertia = I = mr^2
 * Moment of inertia (approx. for an axis through the center of mass) = I = 1/12*ML^2 (where m is the mass and L)
 * Angular Momentum = IW
 * Torque = dI/dT x dW/dt = I x Angular acceleration
 * Angular acceleration = Torque / I
 *
 */

void ObjTorque (obj_type_ptr p_object,float p_mx,float p_my,float p_mz)
{
	float l_moment_of_inertia=p_object->mass*(p_object->bsphere_radius*2)*(p_object->bsphere_radius*2)/12;
	ObjAccelerateRotation(p_object, p_mx/l_moment_of_inertia, p_my/l_moment_of_inertia, p_mz/l_moment_of_inertia);
}



/*
 * void ObjDrag (obj_type_ptr p_object)
 *
 * Applies Drag force to the object (to decrease object speed until it stops)
 *
 * Input parameters: p_object = object
 *
 */

void ObjDrag (obj_type_ptr p_object)
{
	// Linear Drag
	ObjForce(p_object,0,-p_object->lin_speed.x*100,
						-p_object->lin_speed.y*100,
						-p_object->lin_speed.z*100);
	
	// Angular Drag
	ObjTorque(p_object,-p_object->rot_speed.x*5000,
			 		   -p_object->rot_speed.y*5000,
			 		   -p_object->rot_speed.z*5000);
}



/*
 * void ObjDynamics (obj_type_ptr p_object, float p_time_factor)
 *
 * Applies Dynamics laws to the object (linear and angular moments conservation)
 *
 * Input parameters: p_object = object
 *                   p_time_factor = time factor (it depends on the Physics FPS, time_factor should be equal to 1/FPS)
 *
 */

void ObjDynamics (obj_type_ptr p_object, float p_time_factor)
{
	// Linear momentum conservation
	ObjTranslateW (p_object,p_object->lin_speed.x*p_time_factor,
						    p_object->lin_speed.y*p_time_factor,
						    p_object->lin_speed.z*p_time_factor);
	
	// Angular momentum conservation
	ObjRotate (p_object,0, p_object->rot_speed.x*p_time_factor,
					       p_object->rot_speed.y*p_time_factor,
					       p_object->rot_speed.z*p_time_factor);
}
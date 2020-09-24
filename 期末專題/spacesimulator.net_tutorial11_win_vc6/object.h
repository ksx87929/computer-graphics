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
 * File header
 *  
 */

#ifndef _OBJECT_H
#define _OBJECT_H

#include "mat_vect.h"
#include "mat_matr.h"



/*
 * CONSTANTS DECLARATION
 *  
 */

#define MAX_VERTICES 10000 // Max number of vertices (for each object)
#define MAX_POLYGONS 10000 // Max number of polygons (for each object)
#define MAX_OBJECTS 100 // Max number of objects



/*
 * TYPES DECLARATION
 *  
 */

// Our vertex type
typedef struct{
    float x,y,z;
}vertex_type;

// The polygon (triangle), 3 numbers that aim 3 vertices
typedef struct{
    unsigned short a,b,c;
}polygon_type;

// The mapcoord type, 2 texture coordinates for each vertex
typedef struct{
    float u,v;
}mapcoord_type;

typedef struct{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
}rgba_type;

// The material type
typedef struct {

	rgba_type mat_ambient;
	rgba_type mat_diffuse;
	rgba_type mat_specular;
	unsigned short mat_shininess;

} material_type;

// The object type
typedef struct {

	char name[20]; // Name of the object
	char filename[40]; // 
    
	int vertices_qty; // Number of vertices
    int polygons_qty; // Number of polygons

    p3d_type bsphere_center; // Center of the object calculated using bounding sphere
	p3d_type vertex[MAX_VERTICES]; // Array of vertices
    p3d_type normal[MAX_VERTICES]; // Array of the vertices' normals
    p3d_type aabb[8]; // Array of the vertices' aabb (axis aligned bounding box)
    polygon_type polygon[MAX_POLYGONS]; // Array of polygons (numbers that point to the vertices' list)
    mapcoord_type mapcoord[MAX_VERTICES]; // Array of U,V coordinates for texture mapping

	p3d_type lin_speed; //linear speed in m/s
	p3d_type rot_speed; //angular speed in rad/s
	float mass; //object mass in kg
	float bsphere_radius; //object radius (bounding sphere)
	
    int id_texture; // Number of the texture 

	matrix_4x4_type matrix; // Object matrix
	matrix_4x4_type last_matrix; // Object matrix
	
} obj_type, *obj_type_ptr;



/*
 * VARIABLES DECLARATION
 *  
 */

extern obj_type object[MAX_OBJECTS];
extern int obj_qty;
extern int obj_control;



/*
 * FUNCTIONS DECLARATION
 *  
 */

extern char ObjLoad(char *p_object_name, char *p_texture_name, float p_pos_x, float p_pos_y, float p_pos_z, int p_rot_x, int p_rot_y, int p_rot_z);
extern char ObjLoadFromIni(char *p_ini_filename, char *p_object_name);
extern void ObjSaveToIni(char *p_ini_filename, char *p_object_name);
extern void ObjCreateAABB(obj_type_ptr p_object);
extern void ObjCreateBSphere(obj_type_ptr p_object);
extern void ObjCalcNormals(obj_type_ptr p_object);
extern void ObjPosition (obj_type_ptr p_object,float p_x,float p_y,float p_z);
extern void ObjTranslate (obj_type_ptr p_object,float p_x,float p_y,float p_z);
extern void ObjTranslateW (obj_type_ptr p_object,float p_x,float p_y,float p_z);
extern void ObjRotate (obj_type_ptr p_object, char p_use_degrees, float p_angle_x,float p_angle_y,float p_angle_z);

extern void ObjAccelerate (obj_type_ptr p_object, char p_axis_system, float p_ax, float p_ay,float p_az);
extern void ObjAccelerateRotation (obj_type_ptr p_object,float p_vrx,float p_vry,float p_vrz);
extern void ObjForce(obj_type_ptr p_object, char p_axis_system, float p_ix,float p_iy,float p_iz);
extern void ObjTorque (obj_type_ptr p_object,float p_mx,float p_my,float p_mz);
extern void ObjDrag (obj_type_ptr p_object);
extern void ObjDynamics (obj_type_ptr p_object, float p_time_factor);

#endif
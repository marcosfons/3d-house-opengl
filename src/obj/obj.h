#ifndef OBJ_HEADER
#define OBJ_HEADER

#include <stdio.h>

#include <GL/gl.h>

#include "mtl_parser.h"


// Following the specification from
// http://paulbourke.net/dataformats/obj/

typedef enum {
	VERTEX,
	VERTEX_NORMAL,
	VERTEX_TEXTURE,
	VERTEX_NORMAL_TEXTURE,
} face_types;

typedef struct {
	float x;
	float y;
	float z;
	float w; // Vertex color in range 0 to 1. Defaults to 1
} vertex;

typedef struct {
	float x;
	float y;
	float z;
} normal;

typedef struct {
	float u;
	float v; // Optional. Defaults to 0
	float w; // Optional w. Defaults to 0
} texture_vertex;

// A struct to represent a collection of faces
// If the normal or the texture are not defined to 
// this faces the pointers are null
typedef struct {

	vertex* vertices;
	normal* normals;
	texture_vertex* texture_vertices;

	GLuint vertex_vbo;
	GLuint normal_vbo;
	GLuint texture_vbo;
} faces;

typedef struct {
	// Type of the face. Key of this hashmap
	// The first two bits are referred to the enum face_type
	// The other bits are related to the amount of vertices in the face
	int key;

	faces value;
} faces_type;

typedef struct {
	char* key; // The material name
	
	material* material; // Pointer to the material
	
	faces_type* faces;
} faces_by_material;

typedef struct {
	char* filepath;

	// vertex* vertices;
	// normal* normals;
	// texture_vertex* texture_vertices;

	// GLuint vertex_vbo;
	// GLuint normal_vbo;
	// GLuint texture_vertices_vbo;
	
	// This is like having a map of material -> list of faces
	faces_by_material* faces_by_material;
} obj;


// @brief Parses an obj file from the given filepath
// obj* parse_obj(char* filepath);

// @brief Print out to stdout information about the obj
void print_obj(obj obj);

// @brief Init VBO for the obj
void init_obj_vbos(obj* obj);

// @brief Draw a single obj
void draw_obj(obj* obj);

// @brief Frees the memory used by the obj
void destroy_obj(obj* obj);


#endif

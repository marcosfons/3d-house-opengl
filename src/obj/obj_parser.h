#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include <stdio.h>
#include <stdbool.h>

#include "obj.h"
#include "mtl_parser.h"



#define INITIAL_LIST_SIZE 100
#define LIST_SIZE_MULTIPLIER 3


// TODO(marcosfons): Use flags instead of bools for each value
typedef struct {
	int vertex_index;

	int texture_index;
	bool texture_is_set;

	int normal_index;
	bool normal_is_set;
} element;

typedef struct {
	element* elements;
	int elements_count;

	material* material;
} face;

typedef struct {
	vertex* vertices;
	normal* normals;
	texture_vertex* texture_vertices;

	obj* obj;
} parsing_obj;


// @brief Parses an obj file from the given filepath
obj* parse_obj(char* filepath, char* material_filepath);

// @brief Parses an obj file from the given file pointer
obj* parse_from_fp(FILE* fp, materials* materials);

// @brief Parse one line at a time, modifying the values inside the given obj
//
// Current material here is a pointer to an integer that indexes the specific
// material used. Is a pointer because when reading "usemtl" lines it can 
// be changed inside the function
// It can be null, in this case there isn't a current material
void parse_line(parsing_obj* obj, faces_by_material** current_material, char* line);

// @brief Transform the parsing_obj in the actually obj
// obj* transform_parsing_obj(parsing_obj parsing_obj);


void add_vertex(obj* obj, vertex vertex);
void add_normal(obj* obj, normal normal);
void add_face(parsing_obj* obj, faces_by_material* current_material, face face);
void add_texture_vertex(obj* obj, texture_vertex texture_vertex);
void add_element(face* face, element element);



#endif

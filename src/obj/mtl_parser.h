#ifndef MTL_PARSER_HEADER
#define MTL_PARSER_HEADER


#include <stdio.h>


typedef enum {
	NO_SPECULAR_HIGHLIGHTS,
	SPECULAR_HIGHLIGHTS
} illumination_mode;

typedef struct {
	char* name;

	float* ambient_color;
	float* diffuse_color;
	float* specular_color;

	illumination_mode illumination_mode;

	float shininess;

	float transparency;
} material;

typedef struct {
	material* mtls;
	int count;;
} materials;



materials* parse_mtl(FILE* fp);
void parse_mtl_line(materials* materials, char* line);

void free_materials(materials* materials);

#endif

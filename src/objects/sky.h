#ifndef SKY_HEADER
#define SKY_HEADER


#include "../engine/texture.h"

typedef struct {
	texture* texture;

	float size;

	GLfloat* position;

	GLuint display_list;
} sky;


sky* create_sky(float size);

void init_sky(sky* sky);

void set_sky_position(sky* sky, GLfloat x, GLfloat y, GLfloat z);

void draw_sky(sky* sky);


#endif

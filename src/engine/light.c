#include <stdlib.h>

#include "light.h"


light* create_light(GLenum target) {
	light* new_light = malloc(sizeof(light));

	new_light->target = target;

	new_light->ambient = malloc(4 * sizeof(GLfloat));
	new_light->diffuse = malloc(4 * sizeof(GLfloat));
	new_light->specular = malloc(4 * sizeof(GLfloat));
	new_light->position = malloc(4 * sizeof(GLfloat));

	return new_light;
}

void enable_light(light* light) {
	glEnable(light->target);
}

void disable_light(light* light) {
	glDisable(light->target);
}

void debug_light(light* light) {
	glColor3fv(light->diffuse);
	glPointSize(10);
	glBegin(GL_POINTS);
		glVertex3fv(light->position);
	glEnd();
}

void set_light_ambient(light* light, GLfloat r, GLfloat g, GLfloat b, GLfloat w) {
	light->ambient[0] = r;
	light->ambient[1] = g;
	light->ambient[2] = b;
	light->ambient[3] = w;

  glLightfv(light->target, GL_AMBIENT, light->ambient);
}

void set_light_diffuse(light* light, GLfloat r, GLfloat g, GLfloat b, GLfloat w) {
	light->diffuse[0] = r;
	light->diffuse[1] = g;
	light->diffuse[2] = b;
	light->diffuse[3] = w;

  glLightfv(light->target, GL_DIFFUSE, light->diffuse);
}

void set_light_specular(light* light, GLfloat r, GLfloat g, GLfloat b, GLfloat w) {
	light->specular[0] = r;
	light->specular[1] = g;
	light->specular[2] = b;
	light->specular[3] = w;

  glLightfv(light->target, GL_SPECULAR, light->specular);
}

void set_light_position(light* light, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
	light->position[0] = x;
	light->position[1] = y;
	light->position[2] = z;
	light->position[3] = w;

  glLightfv(light->target, GL_POSITION, light->position);
}

void reset_light_position(light* light) {
  glLightfv(light->target, GL_POSITION, light->position);
}

#include <stdlib.h>

#include <GL/gl.h>
#include <GL/glut.h>

#include "grass_floor.h"


grass_floor* create_grass_floor() {
	grass_floor* new_grass_floor = malloc(sizeof(grass_floor));

	new_grass_floor->size = 100;

	new_grass_floor->color[0] = 86;
	new_grass_floor->color[1] = 125;
	new_grass_floor->color[2] = 70;

	return new_grass_floor;
}

void draw_grass_floor(grass_floor* grass_floor) {
	int size = grass_floor->size;
	float y = -0.001f;

	glColor3ub(grass_floor->color[0], grass_floor->color[1], grass_floor->color[2]);
	glBegin(GL_QUADS);
		glNormal3f(0, 1, 0);
		glVertex3f(-size, y, size);
		glVertex3f(-size, y, -size);
		glVertex3f(+size, y, -size);
		glVertex3f(+size, y, size);
	glEnd();
}

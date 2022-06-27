#include <stdlib.h>
#include <stdio.h>

#include "sky.h"


sky* create_sky(float size) {
	sky* new_sky = malloc(sizeof(sky));

	new_sky->size = size;

	new_sky->position = malloc(4 * sizeof(GLfloat));
	new_sky->position[0] = 0;
	new_sky->position[1] = 0;
	new_sky->position[2] = 0;

	return new_sky;
}

void init_sky(sky* sky) {
	// sky->texture = load_texture("assets/textures/pngegg.png");
	
	char* files[6] = {
		"assets/textures/skybox_optimized/right.jpg",
		"assets/textures/skybox_optimized/left.jpg",
		"assets/textures/skybox_optimized/top.jpg",
		"assets/textures/skybox_optimized/bottom.jpg",
		"assets/textures/skybox_optimized/front.jpg",
		"assets/textures/skybox_optimized/back.jpg",
	};

	sky->texture = load_cubemap(files, GL_TEXTURE0);

}

void set_sky_position(sky* sky, GLfloat x, GLfloat y, GLfloat z) {
	sky->position[0] = x;
	sky->position[1] = y;
	sky->position[2] = z;
}

void draw_sky(sky* sky) {
	// Code from https://stackoverflow.com/questions/58125554/legacy-opengl-rendering-cube-map-layout-understanding-gltexcoord3f-parameters
	float size = sky->size;
	float pnt[8][3] = {
		{+size, -size, +size},
		{+size, +size, +size},
		{-size, +size, +size},
		{-size, -size, +size},
		{+size, -size, -size},
		{+size, +size, -size},
		{-size, +size, -size},
		{-size, -size, -size}
	};
	int tab[24] = {
		0,1,2,3,
		7,6,5,4,
		4,5,1,0,
		5,6,2,1,
		6,7,3,2,
		7,4,0,3
	};
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(-sky->position[0], 0, -sky->position[2]);
	glColor3ub(255, 255, 255);

	// glDisable(GL_TEXTURE0);
	// glDisable(GL_TEXTURE_2D);
	
	glEnable(GL_TEXTURE_CUBE_MAP);
	glActiveTexture(sky->texture->target);
	glBindTexture(GL_TEXTURE_CUBE_MAP, sky->texture->id);
	glBegin(GL_QUADS);
	for (int i=23;i>=0;i--) {
		int j = tab[i];
		glMultiTexCoord3fv(GL_TEXTURE0, pnt[j]);
		glVertex3fv(pnt[j]);
	}
	glEnd();

	glTexCoord3f(0, 0, 0);
	glPopMatrix();

	glEnable(GL_LIGHTING);

}

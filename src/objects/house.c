#include <stdlib.h>

#include <GL/glut.h>

#include "../obj/obj_parser.h"
#include "house.h"


house* create_house() {
	house* new_house = malloc(sizeof(house));

	new_house->wall_texture = NULL;
	new_house->guitar_obj = parse_obj("assets/obj/guitar/guitar.obj", "assets/obj/guitar/guitar.mtl");
	new_house->porsche_obj = parse_obj(
			"assets/obj/porsche/porsche_cayman_gt4.obj", "assets/obj/porsche/porsche_cayman_gt4.mtl"
	);

	return new_house;
}

void init_house(house* house) {
	// house->wall_texture = load_texture("assets/textures/wall/wall_texture2.jpg", GL_TEXTURE1);

	init_obj_vbos(house->guitar_obj);
	init_obj_vbos(house->porsche_obj);
}

// static void draw_wall(float width, float height, float depth) {
// 	glBegin(GL_QUADS);
// 		// Front
// 		glVertex3f(0, 0, 0);
// 		glVertex3f(width, 0, 0);
// 		glVertex3f(width, height, 0);
// 		glVertex3f(0, height, 0);
//
// 		// Behind
// 		glVertex3f(0, height, 0 + depth);
// 		glVertex3f(0, 0, 0 + depth);
// 		glVertex3f(width, 0, 0 + depth);
// 		glVertex3f(width, height, 0 + depth);
//
// 		// Up
// 		glVertex3f(width, height, 0);
// 		glVertex3f(0, height, 0);
// 		glVertex3f(0, height, depth);
// 		glVertex3f(width, height, depth);
//
// 		// Sides
// 		glVertex3f(0, height, 0 + depth);
// 		glVertex3f(0, 0, 0 + depth);
// 		glVertex3f(0, 0, 0);
// 		glVertex3f(0, height, 0);
//
// 		glVertex3f(width, height, 0 + depth);
// 		glVertex3f(width, 0, 0 + depth);
// 		glVertex3f(width, 0, 0);
// 		glVertex3f(width, height, 0);
// 	glEnd();
// }

static void draw_3d_quad(float width, float height, float depth) {
	if (width == 0 || height == 0 || depth == 0) {
		return;
	}
	glBegin(GL_QUADS);
		// Front
		glNormal3f(0, 0, -1);
		glVertex3f(0, 0, 0);
		glNormal3f(0, 0, -1);
		glVertex3f(width, 0, 0);
		glNormal3f(0, 0, -1);
		glVertex3f(width, height, 0);
		glVertex3f(0, height, 0);

		// Behind
		glNormal3f(0, 0, +1);
		glVertex3f(0, height, 0 + depth);
		glVertex3f(0, 0, 0 + depth);
		glVertex3f(width, 0, 0 + depth);
		glVertex3f(width, height, 0 + depth);

		// Up
		glNormal3f(0, 1, 0);
		glVertex3f(width, height, 0);
		glVertex3f(0, height, 0);
		glVertex3f(0, height, depth);
		glVertex3f(width, height, depth);

		// Down
		glNormal3f(0, -1, 0);
		glVertex3f(width, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, depth);
		glVertex3f(width, 0, depth);

		// Sides
		glNormal3f(-1, 0, 0);
		glVertex3f(0, height, 0 + depth);
		glVertex3f(0, 0, 0 + depth);
		glVertex3f(0, 0, 0);
		glVertex3f(0, height, 0);

		glNormal3f(1, 0, 0);
		glVertex3f(width, height, 0 + depth);
		glVertex3f(width, 0, 0 + depth);
		glVertex3f(width, 0, 0);
		glVertex3f(width, height, 0);
	glEnd();
}
static void draw_3d_quad_with_hole(float wall[3], float pos[2], float dimensions[2]) {
	glPushMatrix();

	draw_3d_quad(pos[0], wall[1], wall[2]);

	glTranslatef(pos[0], 0, 0);

	draw_3d_quad(dimensions[0], pos[1], wall[2]);

	glPushMatrix();
	glTranslatef(0, pos[1] + dimensions[1], 0);
	draw_3d_quad(dimensions[0], wall[1] - pos[1] - dimensions[1], wall[2]);
	glPopMatrix();

	glTranslatef(dimensions[0], 0, 0);

	draw_3d_quad(wall[0] - pos[0] - dimensions[0], wall[1], wall[2]);

	glPopMatrix();
}

static void draw_wall(float width, float height) {
	glBegin(GL_QUADS);
		glVertex2f(0, 0);
		glVertex2f(width, 0);
		glVertex2f(width, height);
		glVertex2f(0, height);
	glEnd();
}

static void draw_wall_with_depth(float width, float height, float depth) {
	glBegin(GL_QUADS);
		glVertex2f(0, 0);
		glVertex2f(width, 0);
		glVertex2f(width, height);
		glVertex2f(0, height);
	glEnd();
}

static void draw_wall_with_hole(float width, float height, float pos[2], float dimensions[2]) {
	glBegin(GL_QUADS);
		glVertex2f(0, 0);
		glVertex2f(pos[0], 0);
		glVertex2f(pos[0], height);
		glVertex2f(0, height);

		glVertex2f(width, 0);
		glVertex2f(pos[0] + dimensions[0], 0);
		glVertex2f(pos[0] + dimensions[0], height);
		glVertex2f(width, height);

		glVertex2f(pos[0], pos[1] + dimensions[1]);
		glVertex2f(pos[0], height);
		glVertex2f(pos[0] + dimensions[0], height);
		glVertex2f(pos[0] + dimensions[0], pos[1] + dimensions[1]);

		glVertex2f(pos[0], 0);
		glVertex2f(pos[0], pos[1]);
		glVertex2f(pos[0] + dimensions[0], pos[1]);
		glVertex2f(pos[0] + dimensions[0], 0);
	glEnd();
}

void draw_house(house* house) {
	glTranslatef(0, 0, 10);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	//
	// // glActiveTexture(house->wall_texture->target);
	// // glBindTexture(GL_TEXTURE_2D, house->wall_texture->id);
	// // glEnable(GL_TEXTURE_2D);
	glColor3ub(139, 69, 19);
	// // glColor3ub(255, 255, 255);
	//
	const float wall_depth = 0.3;

	const float height = 5;
	const float depth = 0.2;

	const float first_wall_width = 10;
	const float first_wall_width_depth = 12; // The amount that it goes for Z
	//
	// // Roof
	// // glBegin(GL_QUADS);
	// // 	glVertex3f(0, height, 0);
	// // 	glVertex3f(0, height, 8);
	// // 	glVertex3f(8, height, 8);
	// // 	glVertex3f(8, height, 0);
	// // glEnd();
	//
	// // draw_wall_with_hole(8, height, (float[2]) { 4, 0 }, (float[2]) {2, 3});
	// // draw_3d_quad_with_hole((float[3]) {8, height, 1}, (float[2]) { 4, 0 }, (float[2]) {2, 3});
	glPushMatrix();
	// glTranslatef(4, 0, 0);
	draw_3d_quad_with_hole((float[3]){first_wall_width, height, depth}, (float[2]){-2, 1.2}, (float[2]) {1, 1});
	// draw_3d_quad(-first_wall_width, height, depth);

	//
	// glTranslatef(-8, 0, 0);
	// glRotatef(-90, 0, 1, 0);
	// draw_3d_quad(first_wall_width_depth, 5, depth);
	//
	// glTranslatef(8, 0, 0);
	// glRotatef(-90, 0, 1, 0);
	//
	// draw_3d_quad_with_hole((float[3]){8, 5, depth}, (float[2]) { 4, 1.5 }, (float[2]){3, 2});
	// // draw_wall_with_hole(8, height, (float[2]) { 4, 1.5 }, (float[2]){3, 2});
	// //
	// glTranslatef(8, 0, 0);
	// glRotatef(-90, 0, 1, 0);
	// draw_wall(8, height);
	//
	// // Floor
	// draw_3d_quad(8, 0.001, 8);

	// glDisable(GL_COLOR_MATERIAL);
	// glBindTexture(GL_TEXTURE_2D, 0);
	// glBindTexture(house->wall_texture->target, 0);
	
	glPopMatrix();

	glDisable(GL_COLOR_MATERIAL);

	glPushMatrix();
	glColor3ub(159, 169, 170);
	glTranslatef(4, 0, 0);
	draw_3d_quad(7, 0.001, first_wall_width_depth);
	glTranslatef(0, 0, first_wall_width_depth);
	draw_3d_quad(7, height, depth);
	glTranslatef(7, 0, 0);
	glRotatef(90, 0, 1, 0);
	draw_3d_quad(first_wall_width_depth, height, depth);
	glPopMatrix();

	


	const float porsche_scale = 2;
	glTranslatef(7, 0, 3);
	glScalef(porsche_scale, porsche_scale, porsche_scale);
	
	draw_obj(house->porsche_obj);
	// draw_obj(house->guitar_obj);

	// glBindTexture(GL_TEXTURE_2D, 0);
	// glDisable(GL_TEXTURE_2D);
}

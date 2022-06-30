#include <stdlib.h>

#include <GL/glut.h>

#include "../obj/obj_parser.h"
#include "house.h"


const unsigned char wall_color[3] = {156, 135, 116};
const unsigned char floor_color[3] = {166, 122, 91};
// const unsigned char sidewalk_color[3] = {160, 150, 132};
const unsigned char sidewalk_color[3] = {140, 135, 130};

const float height = 5;
const float depth = 0.2;

const float first_wall_width = 12;
const float first_wall_width_depth = 16; // The amount that it goes for Z

const float room_width = 9.5;
const float room_depth = 11;

const float bathroom_width = 6;
const float bathroom_depth = first_wall_width_depth - room_depth;

const float sidewalk_width = 2;

house* create_house() {
	house* new_house = malloc(sizeof(house));

	new_house->wall_texture = NULL;
	new_house->guitar_obj = parse_obj("assets/obj/guitar/guitar_sla.obj", "assets/obj/guitar/guitar_sla.mtl");
	new_house->porsche_obj = parse_obj(
			"assets/obj/porsche/porsche_cayman_gt4.obj", "assets/obj/porsche/porsche_cayman_gt4.mtl"
	);
	// new_house->porsche_obj = parse_obj(
	// 		"assets/obj/porsche/untitled3.obj", "assets/obj/porsche/untitled3.mtl"
	// );
	new_house->bed_obj = parse_obj("assets/obj/bed/bed.obj", "assets/obj/bed/bed.mtl");
	new_house->toilet_obj = parse_obj("assets/obj/toilet/toilet.obj", "assets/obj/toilet/toilet.mtl");
	new_house->couch_obj = parse_obj("assets/obj/couch/couch.obj", "assets/obj/couch/couch.mtl");
	new_house->teapot_obj = parse_obj("assets/obj/teapot/teapot.obj", NULL);
	// new_house->plant_obj = parse_obj("assets/obj/plant/3d-model.obj", "assets/obj/plant/3d-model.obj.mtl");

	new_house->compiled = false;
	new_house->compiled_transparent = false;

	return new_house;
}

void init_house(house* house) {
	house->wall_texture = load_texture("assets/textures/wall/wall_texture2.jpg", GL_TEXTURE1);

	init_obj_vbos(house->guitar_obj);
	init_obj_vbos(house->porsche_obj);
	init_obj_vbos(house->bed_obj);
	init_obj_vbos(house->toilet_obj);
	init_obj_vbos(house->couch_obj);
	init_obj_vbos(house->teapot_obj);
	// init_obj_vbos(house->plant_obj);

	house->display_list = glGenLists(1);
	house->display_list_transparent = glGenLists(1);
}

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
static void draw_3d_quad_with_hole(float wallX, float wallY, float wallZ, float pos[2], float dimensions[2]) {
	glPushMatrix();

	draw_3d_quad(pos[0], wallY, wallZ);

	glTranslatef(pos[0], 0, 0);

	draw_3d_quad(dimensions[0], pos[1], wallZ);

	glPushMatrix();
	glTranslatef(0, pos[1] + dimensions[1], 0);
	draw_3d_quad(dimensions[0], wallY - pos[1] - dimensions[1], wallZ);
	glPopMatrix();

	glTranslatef(dimensions[0], 0, 0);

	draw_3d_quad(wallX - pos[0] - dimensions[0], wallY, wallZ);

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


static void draw_window_frame(float pos[3], float dimensions[3], float frame_width) {
	glPushMatrix();

	glTranslatef(pos[0], pos[1], pos[2]);
	glColor3ub(60, 60, 60);
	draw_3d_quad(frame_width, dimensions[1], dimensions[2]);
	draw_3d_quad(dimensions[0], frame_width, dimensions[2]);

	glTranslatef(0, dimensions[1] - frame_width, 0);
	draw_3d_quad(dimensions[0], frame_width, dimensions[2]);

	glTranslatef(dimensions[0] - frame_width, 0, 0);
	draw_3d_quad(frame_width, -dimensions[1] + frame_width, dimensions[2]);

	glTranslatef((-dimensions[0] + frame_width) / 2, 0, 0);
	draw_3d_quad(frame_width, -dimensions[1] + frame_width, dimensions[2]);

	glPopMatrix();
}

static void draw_window_glass(float pos[3], float dimensions[3], float frame_width) {
	glPushMatrix();
	glColor4ub(110, 110, 110, 90);
	glTranslatef(pos[0], pos[1], dimensions[2]);
	
	float half_width = dimensions[0] / 2;
	float half_part = dimensions[0] / 2;

	glBegin(GL_QUADS);
		glVertex3f(frame_width, dimensions[1] - frame_width, 0);
		glVertex3f(half_width, dimensions[1] - frame_width, 0);
		glVertex3f(half_width, frame_width, 0);
		glVertex3f(frame_width, frame_width, 0);

		glVertex3f(half_part, dimensions[1] - frame_width, 0);
		glVertex3f(half_part + half_width, dimensions[1] - frame_width, 0);
		glVertex3f(half_part + half_width, frame_width, 0);
		glVertex3f(half_part, frame_width, 0);
	glEnd();
	glPopMatrix();
}

static void draw_door(float posX, float dimensions[3]) {
	glPushMatrix();
	
	glTranslatef(posX + dimensions[2], 0, 0);
	glRotatef(95, 0, -1, 0);

	glColor3ub(93, 41, 6);
	draw_3d_quad(dimensions[0], dimensions[1], dimensions[2]);

	glTranslatef(dimensions[0] - 0.3, dimensions[1] / 2, 0);
	
	glPushMatrix();
	glColor3ub(255, 255, 255);
	draw_3d_quad(0.03, 0.03, -0.07);
	glTranslatef(0.03, 0, -0.07);
	draw_3d_quad(-0.2, 0.03, -0.05);
	glPopMatrix();
	glTranslatef(0, 0, dimensions[2]);
	draw_3d_quad(0.03, 0.03, 0.07);
	glTranslatef(0.03, 0, 0.07);
	draw_3d_quad(-0.2, 0.03, 0.05);


	glPopMatrix();
}

static void draw_roof(float width, float depth, float height) {
	draw_3d_quad(width, 0.1, depth);
	glTranslatef(0, 0.1, 0);

	const float divider = 6;
	// const float space = 0.4;
	const float space = 0.0;
	
	glBegin(GL_TRIANGLES);
		glVertex3f(0, 0, 0);
		glVertex3f(width / divider, height, depth / 2);
		glVertex3f(0, 0, depth);

		glVertex3f(width, 0, 0);
		glVertex3f(width - width / divider, height, depth / 2);
		glVertex3f(width, 0, depth);
	glEnd();

	glBegin(GL_QUADS);
		glVertex3f(0, 0, 0);
		glVertex3f(width / divider, height, depth / 2 - space/2);
		glVertex3f((width / divider) * (divider - 1), height, depth / 2 - space/2);
		glVertex3f(width, 0, 0);

		glVertex3f(0, 0, depth);
		glVertex3f(width / divider, height, depth - depth / 2 + space/2);
		glVertex3f((width / divider) * (divider - 1), height, depth - depth / 2 + space/2);
		glVertex3f(width, 0, depth);
	glEnd();

	glColor3ub(50, 40, 40);
	glLineWidth(0.2);
	glBegin(GL_LINE_LOOP);
		glVertex3f(0, 0, 0);
		glVertex3f(width / divider, height, depth / 2);
		glVertex3f(0, 0, depth);

		glVertex3f(width, 0, 0);
		glVertex3f(width - width / divider, height, depth / 2);
		glVertex3f(width, 0, depth);

		glVertex3f(0, 0, 0);
		glVertex3f(width / divider, height, depth / 2 - space/2);
		glVertex3f((width / divider) * (divider - 1), height, depth / 2 - space/2);
		glVertex3f(width, 0, 0);

		glVertex3f(0, 0, depth);
		glVertex3f(width / divider, height, depth - depth / 2 + space/2);
		glVertex3f((width / divider) * (divider - 1), height, depth - depth / 2 + space/2);
		glVertex3f(width, 0, depth);
	glEnd();
}


static void draw_table() {
	glColor3ub(49,30,1);
	glPushMatrix();
	glTranslatef(5, 1.2, 11);
	draw_3d_quad(3.5, 0.1, 3);

	glTranslatef(0.3, 0, 0.3);
	draw_3d_quad(0.15, -1.2, 0.15);

	glTranslatef(0, 0, 3 - 0.3 - 0.3 -0.15);
	draw_3d_quad(0.15, -1.2, 0.15);

	glTranslatef(3.5 - 0.3 - 0.3 - 0.15, 0, 0);
	draw_3d_quad(0.15, -1.2, 0.15);

	glTranslatef(0, 0, -3 + 0.3 + 0.3 +0.15);
	draw_3d_quad(0.15, -1.2, 0.15);

	glPopMatrix();
}


void draw_house(house* house) {
	const float wall_depth = 0.3;
	const float window_frame_space = 0.1;
	const float door_dimensions[2] = {2.3, 3.5};

	if (house->compiled) {
		glCallList(house->display_list);
	} else {
	glNewList(house->display_list, GL_COMPILE_AND_EXECUTE);
	glTranslatef(0, 0, 10);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	//
	// glActiveTexture(house->wall_texture->target);
	// glBindTexture(GL_TEXTURE_2D, house->wall_texture->id);
	// glEnable(GL_TEXTURE_2D);
	glColor3ubv(wall_color);
	// // glColor3ub(255, 255, 255);
	
	//
	// // Roof
	// glBegin(GL_QUADS);
	// 	glVertex3f(0, height, 0);
	// 	glVertex3f(0, height, 8);
	// 	glVertex3f(8, height, 8);
	// 	glVertex3f(8, height, 0);
	// glEnd();
	

	// Front door
	glPushMatrix();
	glTranslatef(4 - first_wall_width/2, 0, 0);
	draw_window_frame((float[3]){1, 2, 0}, (float[3]) {3, 2, depth}, window_frame_space);
	glColor3ubv(wall_color);
	draw_3d_quad_with_hole(first_wall_width/2, height, depth, (float[2]){1, 2}, (float[2]) {3, 2});
	glTranslatef(-first_wall_width/2, 0, 0);
	draw_door(1, (float[3]) {2.3, 3.5, depth});
	draw_table();
	glColor3ubv(wall_color);
	draw_3d_quad_with_hole(first_wall_width/2, height, depth, (float[2]){1, 0}, (float[2]) {2.3, 3.5});



	// Right wall
	glRotatef(90, 0, -1, 0);

	// Room 
	glPushMatrix();
	// draw_3d_quad_with_hole(room_depth, height, depth, (float[2]){8, 0}, (float[2]) {2.3, 3.5});
	draw_3d_quad(room_depth, height, depth);
	// draw_door(8 + 2.3, (float[3]){2.3, 3.5, depth});
	// glColor3ubv(wall_color);

	glTranslatef(room_depth, 0, 0);
	glRotatef(90, 0, -1, 0);
	// draw_3d_quad(room_width, height, depth);
	draw_3d_quad_with_hole(room_width, height, depth, (float[2]){0.5, 0}, (float[2]) {2.3, 3.5});
	draw_door(0.5, (float[3]){2.3, 3.5, depth});
	glTranslatef(room_width, 0, 0);
	glRotatef(90, 0, -1, 0);

	draw_window_frame((float[3]){5, 2, 0}, (float[3]) {3, 2, depth}, window_frame_space);
	glColor3ubv(wall_color);
	draw_3d_quad_with_hole(room_depth, height, depth, (float[2]){5, 2}, (float[3]){3, 2, depth});

	glTranslatef(room_depth, 0, 0);
	glRotatef(90, 0, -1, 0);
	draw_3d_quad(room_width, height, depth);
	// draw_window_frame((float[3]){3, 2, 0}, (float[3]) {3, 2, depth}, window_frame_space);
	// glColor3ubv(wall_color);
	// draw_3d_quad_with_hole(room_depth, height, depth, (float[2]){3, 2}, (float[3]){3, 2, depth});
	
	glPopMatrix();

	// Bathroom door
	glPushMatrix();
	glTranslatef(room_depth, 0, room_width - bathroom_width);
	draw_3d_quad_with_hole(bathroom_depth, height, depth, (float[2]){1.5, 0}, (float[2]) {2, 3.5});
	draw_door(1.5, (float[3]){2, 3.5, depth});
	glColor3ubv(wall_color);

	glTranslatef(bathroom_depth, 0, 0);
	glRotatef(90, 0, -1, 0);
	glTranslatef(bathroom_width, 0, 0);
	glRotatef(90, 0, -1, 0);

	// Bathroom window
	draw_window_frame((float[3]){1.5, 3, 0}, (float[3]) {1.5, 1.5, depth}, window_frame_space);
	glColor3ubv(wall_color);
	draw_3d_quad_with_hole(bathroom_depth, height, depth, (float[2]){1.5, 3}, (float[3]){1.5, 1.5, depth});
	// draw_3d_quad_with_hole(bathroom_depth, height, depth);

	// glTranslatef(bathroom_width, 0, 0);
	glRotatef(90, 0, -1, 0);
	draw_3d_quad(room_width, height, depth);
	glPopMatrix();


	// Back wall
	glTranslatef(first_wall_width_depth, 0, 0);
	glRotatef(90, 0, 1, 0);
	draw_window_frame((float[3]){4, 1.5, 0}, (float[3]) {3, 2, depth}, window_frame_space);
	glColor3ubv(wall_color);
	draw_3d_quad_with_hole(first_wall_width, 5, depth, (float[2]) { 4, 1.5 }, (float[2]){3, 2});

	// Left wall
	glTranslatef(first_wall_width, 0, 0);
	glRotatef(90, 0, 1, 0);
	draw_window_frame((float[3]){10, 1.5, 0}, (float[3]) {3.5, 2.3, depth}, window_frame_space);
	glColor3ubv(wall_color);
	draw_3d_quad_with_hole(first_wall_width_depth, 5, depth, (float[2]) { 10, 1.5 }, (float[2]){3.5, 2.3});
	
	// Floor
	glRotatef(90, 0, 1, 0);
	glColor3ubv(floor_color);
	draw_3d_quad(first_wall_width + room_width, 0.1, first_wall_width_depth);

	// glColor3ub(164,74,74);
	glColor3ub(157, 96, 85);
	const float roof_border = 1;
	glTranslatef(-7 - depth - roof_border, height, -depth - roof_border);
	draw_roof(7 + 4 + first_wall_width + 5.7 + (2*roof_border), first_wall_width_depth + depth + (2*roof_border), 4);

	// glDisable(GL_COLOR_MATERIAL);
	// glBindTexture(GL_TEXTURE_2D, 0);
	// glBindTexture(house->wall_texture->target, 0);
	
	glPopMatrix();


	
	// Sidewalk
	glPushMatrix();
	glColor3ubv(sidewalk_color);

	glPushMatrix();
	glTranslatef(-6.8, 0, -4);
	draw_3d_quad(sidewalk_width, 0.01, 1.5);
	glTranslatef(0, 0, -2);
	draw_3d_quad(sidewalk_width, 0.01, 1.5);
	glTranslatef(0, 0, -2);
	draw_3d_quad(sidewalk_width, 0.01, 1.5);
	glTranslatef(0, 0, -2);
	draw_3d_quad(sidewalk_width, 0.01, 1.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(4.2, 0, -10);
	draw_3d_quad(7, 0.01, 10);
	glPopMatrix();


	glTranslatef(7 + 4 + sidewalk_width, 0, -sidewalk_width);

	draw_3d_quad(
		-7 - first_wall_width - room_width - (2 * sidewalk_width), 
		0.05, 
		first_wall_width_depth + (2 * sidewalk_width)
	);
	
	glPopMatrix();



	glPushMatrix();
	glColor3ubv(wall_color);
	glTranslatef(4, 0, 0);
	draw_3d_quad(7, 0.001, first_wall_width_depth);
	glTranslatef(0, 0, first_wall_width_depth);
	draw_3d_quad(7, height, depth);
	glTranslatef(7, 0, 0);
	glRotatef(90, 0, 1, 0);
	draw_3d_quad(first_wall_width_depth, height, depth);
	glPopMatrix();

	glDisable(GL_COLOR_MATERIAL);

	
	glEndList();
	house->compiled = true;
}

	const float porsche_scale = 2;
	glPushMatrix();
	glTranslatef(7, 0, 3);
	glScalef(porsche_scale, porsche_scale, porsche_scale);
	
	draw_obj(house->porsche_obj);


	glPushMatrix();
	glTranslatef(-first_wall_width + 0.8, 0, bathroom_depth + 0.13);
	glRotatef(90, 0, 1, 0);
	glScalef(1.3, 1.3, 1.3);
	draw_obj(house->toilet_obj);
	glPopMatrix();

	
	glPushMatrix();
	glTranslatef(-5, 0.4, 1);
	glRotatef(90, 0, 1, 0);
	draw_obj(house->couch_obj);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-4.1, 0.75, 4.8);
	glScalef(0.003, 0.003, 0.003);
	draw_obj(house->teapot_obj);
	glPopMatrix();

	glTranslatef(-room_width - 1, 0, -0.88);
	glPushMatrix();
	glTranslatef(+1.5, 0.5, -0.14);
	glScalef(0.12, 0.12, 0.12);
	draw_obj(house->guitar_obj);
	glPopMatrix();

	glRotatef(90, 0, -1, 0);
	glScalef(0.015, 0.015, 0.015);
	draw_obj(house->bed_obj);


	glPopMatrix();

	
	if (house->compiled_transparent) {
		glCallList(house->display_list_transparent);
	} else {
		glNewList(house->display_list_transparent, GL_COMPILE_AND_EXECUTE);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glPushMatrix();
		
		glTranslatef(4 - first_wall_width/2, 0, 0);
		draw_window_glass((float[3]){1, 2, 0}, (float[3]) {3, 2, depth}, window_frame_space);
		glTranslatef(-first_wall_width/2, 0, 0);
		glRotatef(90, 0, -1, 0);

		glPushMatrix();
		glTranslatef(room_depth, 0, 0);
		glRotatef(90, 0, -1, 0);
		glTranslatef(room_width, 0, 0);
		glRotatef(90, 0, -1, 0);
		draw_window_glass((float[3]){5, 2, 0}, (float[3]) {3, 2, depth}, window_frame_space);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(room_depth, 0, room_width - bathroom_width);
		glTranslatef(bathroom_depth, 0, 0);
		glRotatef(90, 0, -1, 0);
		glTranslatef(bathroom_width, 0, 0);
		glRotatef(90, 0, -1, 0);

		draw_window_glass((float[3]){1.5, 3, 0}, (float[3]) {1.5, 1.5, depth}, window_frame_space);
		glRotatef(90, 0, -1, 0);
		glPopMatrix();
		glTranslatef(first_wall_width_depth, 0, 0);
		glRotatef(90, 0, 1, 0);
		draw_window_glass((float[3]){4, 1.5, 0}, (float[3]) {3, 2, depth}, window_frame_space);
		glTranslatef(first_wall_width, 0, 0);
		glRotatef(90, 0, 1, 0);
		draw_window_glass((float[3]){10, 1.5, 0}, (float[3]) {3.5, 2.3, depth}, window_frame_space);

		glPopMatrix();

		glDisable(GL_BLEND);
		glEndList();
	}
}

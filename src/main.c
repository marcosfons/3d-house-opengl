#include <time.h>
#include <stdlib.h>

#include <GL/glut.h>
// #include <GL/gl.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb/stb_image.h"

#define STB_DS_IMPLEMENTATION
#include "../include/stb/stb_ds.h"

#include "engine/core.h"
#include "engine/light.h"

#include "objects/grass_floor.h"
#include "objects/house.h"
#include "objects/sky.h"


light* plight = NULL;
sky* psky = NULL;
grass_floor* pgrass_floor = NULL;
house* phouse = NULL;

// Using float because what the keyboard_handler uses
float light_enabled = 0;
float sky_enabled = 0;

void init() {
	set_light_ambient(plight, 0.3, 0.3, 0.3, 1.0);
	set_light_diffuse(plight, 1.0, 1.0, 1.0, 1.0);
	set_light_specular(plight, 1.0, 1.0, 1.0, 1.0);
	set_light_position(plight, 0.0, 10.0, 0.0, 1.0);
	enable_light(plight);

	init_house(phouse);
	init_sky(psky);
}

void draw() {
	reset_light_position(plight);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	
	glEnable(GL_COLOR_MATERIAL);
	
	if (light_enabled == 1) {
		debug_light(plight);
	}
	glColor3ub(255, 255, 255);

	draw_grass_floor(pgrass_floor);

	if (sky_enabled == 0) {
		draw_sky(psky);
	}
	draw_house(phouse);
}

void on_camera_move(camera* camera) {
	set_sky_position(psky, camera->center.x, camera->center.y, camera->center.z);
}

int main(int argc, char** argv) {
	engine* engine = create_engine("3D house", 80, 1150, 450);

	pgrass_floor = create_grass_floor();
	plight = create_light(GL_LIGHT0);
	phouse = create_house();
	psky = create_sky(300);

	// add_key(engine->keyboard_handler, "Enable/Disable light points", 'l', &light_enabled, SWITCH, 0);
	// add_key(engine->keyboard_handler, "Enable/Disable light points", 'L', &light_enabled, SWITCH, 0);
	add_key(engine->keyboard_handler, "Enable/Disable sky", 't', &sky_enabled, SWITCH, 0);
	add_key(engine->keyboard_handler, "Enable/Disable sky", 'T', &sky_enabled, SWITCH, 0);

	add_key(engine->keyboard_handler, "Enable/Disable light points", 'l', &(plight->position[0]), INCREMENT, 0.5);
	add_key(engine->keyboard_handler, "Enable/Disable light points", 'L', &(plight->position[0]), INCREMENT, 0.5);
	add_key(engine->keyboard_handler, "Enable/Disable light points", 'h', &(plight->position[0]), INCREMENT, -0.5);
	add_key(engine->keyboard_handler, "Enable/Disable light points", 'H', &(plight->position[0]), INCREMENT, -0.5);
	add_key(engine->keyboard_handler, "Enable/Disable light points", 'k', &(plight->position[2]), INCREMENT, 0.5);
	add_key(engine->keyboard_handler, "Enable/Disable light points", 'K', &(plight->position[2]), INCREMENT, 0.5);
	add_key(engine->keyboard_handler, "Enable/Disable light points", 'j', &(plight->position[2]), INCREMENT, -0.5);
	add_key(engine->keyboard_handler, "Enable/Disable light points", 'J', &(plight->position[2]), INCREMENT, -0.5);
	
	add_init_function(engine, init);

	add_draw_function(engine, draw);

	add_on_camera_move_function(engine, on_camera_move);

	init_engine(engine, argc, argv);	

	return EXIT_SUCCESS;
}

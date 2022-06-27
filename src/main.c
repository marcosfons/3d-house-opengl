#include <time.h>
#include <stdlib.h>

#include <GL/glut.h>
#include <GL/gl.h>

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

int inite = 0;

void init() {
	printf("INit\n");
	set_light_ambient(plight, 0.3, 0.3, 0.3, 1.0);
	set_light_diffuse(plight, 1.0, 1.0, 1.0, 1.0);
	set_light_specular(plight, 1.0, 1.0, 1.0, 1.0);
	set_light_position(plight, -10.0, 15.0, 0.0, 1.0);
	set_light_position(plight, 0.0, 10.0, 4.0, 1.0);
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
	debug_light(plight);

	draw_grass_floor(pgrass_floor);

	draw_house(phouse);

	draw_sky(psky);
}

void on_camera_move(camera* camera) {
	set_sky_position(psky, camera->center.x, camera->center.y, camera->center.z);
}

int main(int argc, char** argv) {
	engine* engine = create_engine("3D house", 80, 1200, 500);

	pgrass_floor = create_grass_floor();
	plight = create_light(GL_LIGHT0);
	phouse = create_house();
	psky = create_sky(100);
	
	add_init_function(engine, init);

	add_draw_function(engine, draw);

	add_on_camera_move_function(engine, on_camera_move);

	init_engine(engine, argc, argv);	

	return EXIT_SUCCESS;
}

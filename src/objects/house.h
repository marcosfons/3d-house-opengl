#ifndef HOUSE_HEADER
#define HOUSE_HEADER


#include "../obj/obj.h"
#include "../engine/texture.h"


typedef struct {
	texture* wall_texture;
		
	obj* guitar_obj;
	obj* porsche_obj;
	obj* bed_obj;
	obj* toilet_obj;
	obj* couch_obj;
	obj* teapot_obj;
	obj* plant_obj;

	GLuint display_list;
	GLuint display_list_transparent;
	bool compiled;
	bool compiled_transparent;
} house;


// @brief Create a new house
house* create_house();

// @brief Init all the values of the house
void init_house(house* house);

// @brief Draw the house on the screen
void draw_house(house* house);



#endif

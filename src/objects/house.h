#ifndef HOUSE_HEADER
#define HOUSE_HEADER


#include "../obj/obj.h"
#include "../engine/texture.h"


typedef struct {
	texture* wall_texture;
		
	obj* guitar_obj;
	obj* porsche_obj;
} house;


// @brief Create a new house
house* create_house();

// @brief Init all the values of the house
void init_house(house* house);

// @brief Draw the house on the screen
void draw_house(house* house);



#endif

#ifndef GRASS_FLOOR_HEADER
#define GRASS_FLOOR_HEADER


typedef struct {
	// The "radius" around the origin
	// But it's a cube
	float size;

	unsigned char color[3];
} grass_floor;


grass_floor* create_grass_floor();

void draw_grass_floor(grass_floor* grass_floor);


#endif

#ifndef ENGINE_CORE_HEADER
#define ENGINE_CORE_HEADER


#include <stdbool.h>
#include <float.h>

#include "camera.h"

#define MOUSE_BOUND_SCREEN_LIMIT 100
#define MOUSE_DELTA_LIMIT 30


typedef void (*init_function)();

typedef void (*draw_function)();

// @brief Type of an update function
// @param Current time provided by the engine
// @return A boolean indicating if any value was changed, and there must be a new draw call
typedef bool (*update_function)(long time);

typedef void (*on_camera_move_function)(camera* camera);

typedef struct {
	// Screen
	int window_id;
	int fps;
	char* title;
	int width;
	int height;
	
	long last_update_call_time;

	// Camera
	camera camera;

	// Mouse
	double old_mouse[2];

	draw_function* init_functions;

	draw_function* draw_functions;

	update_function* update_functions;

	on_camera_move_function* on_camera_move_functions;
} engine;


engine* create_engine(char* window_title, int fps, int width, int height);

void init_engine(engine* engine, int argc, char** argv);

void engine_draw();
void engine_update();
void resize(int width, int height);

void add_init_function(engine* engine, init_function func);
void add_draw_function(engine* engine, draw_function func);
void add_update_function(engine* engine, update_function func);
void add_on_camera_move_function(engine* engine, on_camera_move_function func);

void destroy_engine();

#endif

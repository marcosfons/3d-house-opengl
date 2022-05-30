#ifndef ENGINE_CORE_HEADER
#define ENGINE_CORE_HEADER


#include <float.h>

#include "camera.h"

#define MOUSE_BOUND_SCREEN_LIMIT 100
#define MOUSE_DELTA_LIMIT 30


typedef void (*draw_function)();

typedef void (*update_function)(long time);

typedef struct {
	// Screen
	int window_id;
	int fps;
	char* title;
	int width;
	int height;
	
	long last_draw_call_time;

	// Camera
	camera camera;

	// Mouse
	double old_mouse[2];

	int draw_count;
	draw_function* draw_functions;

	int update_count;
	update_function* update_functions;
} engine;


engine* create_engine(char* window_title, int fps, int width, int height);

void init_engine(engine* engine, int argc, char** argv);

void draw();
void update(int id);
void resize(int width, int height);

void add_draw_function(engine* engine, draw_function func);
void add_update_function(engine* engine, update_function func);

void destroy_engine();

// glutDisplayFunc(draw);
// glutReshapeFunc(resizeWindow);
//
// glutKeyboardFunc(onKeyPress);
// glutTimerFunc(REFRESH_TIME, animationTimer, 1);

#endif

#ifndef ENGINE_CORE_HEADER
#define ENGINE_CORE_HEADER


#include "camera.h"


typedef void (*draw_function)();

typedef void (*update_function)(long time);

typedef struct {
	int fps;
	char* title;
	
	long last_draw_call_time;

	camera camera;

	int draw_count;
	draw_function* draw_functions;

	int update_count;
	update_function* update_functions;
} engine;


engine* create_engine();

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

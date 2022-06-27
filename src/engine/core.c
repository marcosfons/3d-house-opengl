#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>

#include "../../include/stb/stb_ds.h"

#include "camera.h"
#include "core.h"


// ASCII
#define SPACEBAR_KEY 32

// Special
#define CONTROL_KEY 114


static engine* current_engine = NULL;

static bool left_pressed = false;
static bool up_pressed = false;
static bool down_pressed = false;
static bool right_pressed = false;
static bool space_pressed = false;
static bool control_pressed = false;


engine* create_engine(char* window_title, int fps, int width, int height) {
	if (current_engine != NULL) {
		printf("Two engines can't run at the same time");
		exit(EXIT_FAILURE);
	}

	current_engine = malloc(sizeof(engine));

	current_engine->fps = fps;
	current_engine->title = window_title;
	current_engine->width = width;
	current_engine->height = height;

	current_engine->last_update_call_time = 0;

	current_engine->camera = create_camera();

	current_engine->init_functions = NULL;
	current_engine->draw_functions = NULL;
	current_engine->update_functions = NULL;
	current_engine->on_camera_move_functions = NULL;

	current_engine->old_mouse[0] = -DBL_MAX;
	current_engine->old_mouse[1] = -DBL_MAX;

	return current_engine;
}

static void generic_key_press(unsigned char key, bool is_pressed) {
	switch (key) {
		case 'a': case 'A':
			left_pressed = is_pressed;
			break;
		case 'd': case 'D':
			right_pressed = is_pressed;
			break;
		case 'w': case 'W':
			up_pressed = is_pressed;
			break;
		case 's': case 'S':
			down_pressed = is_pressed;
			break;
		case SPACEBAR_KEY:
			space_pressed = is_pressed;
			break;
	}
}

void on_key_press(unsigned char key, int x, int y) {
  switch (key) {
    case 'q': case 'Q':
			destroy_engine();
			glutDestroyWindow(glutGetWindow());
			break;
		default:
			generic_key_press(key, true);
			break;
	}
}

void on_key_released(unsigned char key, int x, int y) {
	generic_key_press(key, false);
}

static void special_key_press(int key, bool is_pressed) {
	switch (key) {
		case CONTROL_KEY:
			control_pressed = is_pressed;
			break;
	}
}

void on_special_key_press(int key, int x, int y) {
	special_key_press(key, true);
}

void on_special_key_released(int key, int x, int y) {
	special_key_press(key, false);
}

// Code based on https://learnopengl.com/Getting-started/Camera
void on_mouse(int x, int y) {
	float sensitivity = 0.1;
	// float dx = (current_engine->old_mouse[0] - x) * sensitivity;
	// float dy = (y - current_engine->old_mouse[1]) * sensitivity; 
	// Inverse
	float dx = (x - current_engine->old_mouse[0]) * sensitivity;
	float dy = (current_engine->old_mouse[1] - y) * sensitivity;

	current_engine->old_mouse[0] = x;
	current_engine->old_mouse[1] = y;

	if ((dx > MOUSE_DELTA_LIMIT || dx < -MOUSE_DELTA_LIMIT) ||
	    (dy > MOUSE_DELTA_LIMIT || dy < -MOUSE_DELTA_LIMIT)) {
		return;
	}

	mouse_movement(&current_engine->camera, dx, dy);
	for (int i = 0; i < arrlen(current_engine->on_camera_move_functions); i++) {
		current_engine->on_camera_move_functions[i](&(current_engine->camera));
	}

	double center_width = (double) current_engine->width / 2;
	double center_height = (double) current_engine->height / 2;
	if ((x < MOUSE_BOUND_SCREEN_LIMIT || x > (current_engine->width) - MOUSE_BOUND_SCREEN_LIMIT) ||
	    (y < MOUSE_BOUND_SCREEN_LIMIT || y > (current_engine->height) - MOUSE_BOUND_SCREEN_LIMIT)) {
		current_engine->old_mouse[0] = center_width;
		current_engine->old_mouse[1] = center_height;

		glutWarpPointer(center_width, center_height); // Center the cursor
	}

	glutPostRedisplay();
}

void init_engine(engine* engine, int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(engine->width, engine->height);
	glutInitWindowPosition(50, 50);

	engine->window_id = glutCreateWindow(engine->title);

	glutDisplayFunc(engine_draw);
	glutReshapeFunc(resize);

	glutKeyboardFunc(on_key_press);
	glutKeyboardUpFunc(on_key_released);
	glutSpecialFunc(on_special_key_press);
	glutSpecialUpFunc(on_special_key_released);

	glutPassiveMotionFunc(on_mouse);

	glClearColor(0.1, 0.1, 0.1, 0);
  glShadeModel(GL_SMOOTH);
	
	// float am[4] = {0.3, 0.3, 0.3, 1};
	// glLightModelfv(GL_LIGHT_MODEL_AMBIENT, am);

  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);

	glutSetCursor(GLUT_CURSOR_NONE);

	look_at(current_engine->camera);

	engine_update();
	glutIdleFunc(engine_update);

	glewInit();

	for (int i = 0; i < arrlen(current_engine->init_functions); i++) {
		current_engine->init_functions[i]();
	}

	glutMainLoop();
}

void engine_draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

	look_at(current_engine->camera);

	for (int i = 0; i < arrlen(current_engine->draw_functions); i++) {
		current_engine->draw_functions[i]();
	}
    
  glutSwapBuffers();
}

void resize(int width, int height) {
  glViewport(0, 0, (GLsizei) width, (GLsizei) height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
	
	// glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
	// gluPerspective(1180 * 3.141 / 180, (float) width / height, 1.5, 100.0);
	// gluPerspective(1000, 1, 1.5, 100.0);
	gluPerspective(49.134, (float) width / height, 1, 1000.0);

	current_engine->width = width;
	current_engine->height = height;
}

// TODO(marcosfons): Check update function
void engine_update() {
	bool should_redraw = false;
	int since_start = glutGet(GLUT_ELAPSED_TIME);
	int delta = since_start - current_engine->last_update_call_time;
	current_engine->last_update_call_time = since_start;
	double step = 0.01 * delta;

	for (int i = 0; i < arrlen(current_engine->update_functions); i++) {
		should_redraw = current_engine->update_functions[i](delta) || should_redraw;
	}

	should_redraw = should_redraw || 
									up_pressed || down_pressed || right_pressed || left_pressed || 
									space_pressed || control_pressed;
	if (up_pressed) {
		move_forward(&current_engine->camera, step);
	}
	if (down_pressed) {
		move_backward(&current_engine->camera, step);
	}
	if (left_pressed) {
		move_right(&current_engine->camera, step);
	}
	if (right_pressed) {
		move_left(&current_engine->camera, step);
	}
	if (space_pressed) {
		move_up(&current_engine->camera, step);
	}
	if (control_pressed) {
		move_down(&current_engine->camera, step);
	}

	if (should_redraw) {
		for (int i = 0; i < arrlen(current_engine->on_camera_move_functions); i++) {
			current_engine->on_camera_move_functions[i](&(current_engine->camera));
		}
		glutPostRedisplay();
	}
}

void add_init_function(engine* engine, init_function func) {
	arrput(engine->init_functions, func);
}

void add_draw_function(engine* engine, draw_function func) {
	arrput(engine->draw_functions, func);
}

void add_update_function(engine* engine, update_function func) {
	arrput(engine->update_functions, func);
}

void add_on_camera_move_function(engine* engine, on_camera_move_function func) {
	arrput(engine->on_camera_move_functions, func);
}

void destroy_engine() {
	arrfree(current_engine->draw_functions);
	arrfree(current_engine->draw_functions);
	arrfree(current_engine->update_functions);
	arrfree(current_engine->on_camera_move_functions);

	free(current_engine);
	current_engine = NULL;
}

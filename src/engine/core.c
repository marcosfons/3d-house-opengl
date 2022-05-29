#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>

#include <GL/gl.h>
#include <GL/glut.h>

#include "camera.h"
#include "core.h"


static engine* current_engine = NULL;

static bool left_pressed = false;
static bool up_pressed = false;
static bool down_pressed = false;
static bool right_pressed = false;


engine* create_engine(char* window_title, int fps) {
	if (current_engine != NULL) {
		printf("Two engines can't run at the same time");
		exit(EXIT_FAILURE);
	}

	current_engine = malloc(sizeof(engine));

	current_engine->fps = fps;
	current_engine->title = window_title;

	current_engine->camera = create_camera();

	current_engine->draw_count = 0;
	current_engine->draw_functions = malloc(sizeof(draw_function));

	current_engine->update_count = 0;
	current_engine->update_functions = malloc(sizeof(update_function));

	return current_engine;
}

static void generic_key_press(unsigned char key, bool is_pressed) {
	switch (key) {
		case 'a':
		case 'A':
			left_pressed = is_pressed;
			break;
		case 'd':
		case 'D':
			right_pressed = is_pressed;
			break;
		case 'w':
		case 'W':
			up_pressed = is_pressed;
			break;
		case 's':
		case 'S':
			down_pressed = is_pressed;
			break;
	}
}

void on_key_press(unsigned char key, int x, int y) {
  switch (key) {
    case 'q': 
    case 'Q':
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

void on_mouse(int x, int y) {
	vector3 mouse_delta = { 
		x - current_engine->old_mouse[0], 
		y - current_engine->old_mouse[1], 
		0 
	};
	if (magnitude(mouse_delta) < 50) {
		glRotatef(300, 0, 0, 0);
		// current_engine->camera.eye = cross(multiply_by_scalar(mouse_delta, 0.8), current_engine->camera.eye);
	}

	current_engine->old_mouse[0] = x;
	current_engine->old_mouse[1] = y;
}

void init_engine(engine* engine, int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 400);
	glutInitWindowPosition(50, 50);

	glutCreateWindow(engine->title);

	glutDisplayFunc(draw);
	glutReshapeFunc(resize);

	glutKeyboardFunc(on_key_press);
	glutKeyboardUpFunc(on_key_released);

	glutPassiveMotionFunc(on_mouse);

	glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_COLOR_MATERIAL);

  GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light_position[] = { 10.0, 10.0, 10.0, 0.0 };
	
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);

  //Primeira cor, para depois mudarmos com os eventos
  // glColor3f(0.5, 1.0, 0.5);

	current_engine->old_mouse[0] = 0;
	current_engine->old_mouse[1] = 0;

	look_at(current_engine->camera);
	update(0);

	glutMainLoop();
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

	look_at(current_engine->camera);

	for (int i = 0; i < current_engine->draw_count; i++) {
		current_engine->draw_functions[i]();
	}
    
  glFlush();
}

void resize(int width, int height) {
  glViewport(0, 0, (GLsizei) width, (GLsizei) height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
	
 //  if (width <= height) {
 //    // glOrtho(-5, 5, -5 * (GLfloat)height / (GLfloat)width, 5 * (GLfloat)height/(GLfloat)width, -10.0, 10.0);
 //    glOrtho(-5, 5, -5 * (GLfloat)height / (GLfloat)width, 5 * (GLfloat)height/(GLfloat)width, -10.0, 10.0);
	// } else {
 //    glOrtho(-5 * (GLfloat)width/(GLfloat)height, 5*(GLfloat)width/(GLfloat)height, -5, 5, -10.0, 10.0);
	// }

	// glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 200.0);
	gluPerspective(1000.0, 1.0, 1.5, 200.0);

}

static long current_time_millis() {
  struct timeval time;
  gettimeofday(&time, NULL);

  return time.tv_sec * 1000 + time.tv_usec / 1000;
}

// TODO(marcosfons): Check update function
void update(int id) {
	if (glutGetWindow() == 0 || current_engine == NULL) {
		return;
	}

	long elapsed = current_engine->last_draw_call_time - current_time_millis();
	for (int i = 0; i < current_engine->update_count; i++) {
		current_engine->update_functions[i](elapsed);
	}

	double angle = current_engine->last_draw_call_time / 100 % 360 * 0.017453;
	
	double step = 0.000001;
	if (left_pressed) {
		strafeLeft(&current_engine->camera, step);
	}
	if (right_pressed) {
		strafeRight(&current_engine->camera, step);
	}

	if (up_pressed) {
		moveForward(&current_engine->camera, step);
	}
	if (down_pressed) {
		moveBackward(&current_engine->camera, step);
	}

	// if (left_pressed || right_pressed || up_pressed || down_pressed) {
	// 	print_vector(current_engine->camera.eye);
	// 	printf("    ");
	// 	print_vector(current_engine->camera.center);
	// 	printf("    ");
	// 	print_vector(current_engine->camera.up);
	// 	printf("\n");
	// }
		
	// printf("Angle: %f\n", angle);

	// int radius = 100;
	//
	// current_engine->camera.centerX = sin(angle) * radius;
	// current_engine->camera.centerY = cos(angle) * radius;
	//
	// printf("Enabled: %d %d %d %d\n", up_pressed, right_pressed, down_pressed, left_pressed);
	
	glutPostRedisplay();

	glutTimerFunc(1 / current_engine->fps * 1000, update, id);
	current_engine->last_draw_call_time = current_time_millis();
}

void add_draw_function(engine* engine, draw_function func) {
	engine->draw_functions = realloc(engine->draw_functions, engine->draw_count + 1);
	engine->draw_functions[engine->draw_count] = func;
	engine->draw_count += 1;
}

void add_update_function(engine* engine, update_function func) {
	engine->update_functions = realloc(engine->update_functions, engine->update_count + 1);
	engine->update_functions[engine->update_count] = func;
	engine->update_count += 1;
}

void destroy_engine() {
	free(current_engine->draw_functions);
	free(current_engine->update_functions);

	free(current_engine);
	current_engine = NULL;
}

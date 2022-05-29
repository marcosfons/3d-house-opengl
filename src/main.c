#include <time.h>
#include <stdlib.h>

#include <GL/glut.h>
#include <GL/gl.h>

#include "engine/core.h"


void teapot() {
	glColor3ub(255, 0, 0);
	glScalef(5, 5, 5);
	glutSolidCube(1.0f);
}

int main(int argc, char** argv) {
	engine* engine = create_engine("3D house", 60);
	
	add_draw_function(engine, teapot);
	init_engine(engine, argc, argv);	

	return EXIT_SUCCESS;
}

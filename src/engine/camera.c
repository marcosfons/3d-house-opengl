#include <GL/glut.h>

#include "camera.h"


camera create_camera() {
	camera new_camera;

	new_camera.eyeX = 0;
	new_camera.eyeY = 0;
	new_camera.eyeZ = 0;

	new_camera.centerX = 0;
	new_camera.centerY = 0;
	new_camera.centerZ = 0;

	new_camera.upX = 0;
	new_camera.upY = 0;
	new_camera.upZ = 0;

	return new_camera;
}

void look_at(camera camera) {
	gluLookAt(
		camera.eyeX, camera.eyeY, camera.eyeZ,
		camera.centerX + camera.eyeX, camera.centerY, camera.centerZ + camera.eyeZ,
		camera.upX,	camera.upY, camera.upZ
	);
}

// void moveForward(camera* camera, float step) {
// 	camera->center
//
// }

void moveBackward(camera* camera, float step) {

}

void moveLeft(camera* camera, float step) {

}

void moveRight(camera* camera, float step) {

}

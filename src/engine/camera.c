#include <GL/glut.h>

#include "camera.h"


camera create_camera() {
	camera new_camera;

	new_camera.eye =    (vector3) { 0, 0, 1 };
	new_camera.center = (vector3) { 0, 0, -8 };
	new_camera.up =     (vector3) { 0, 1, 0 };

	return new_camera;
}

void look_at(camera camera) {
	vector3 center = sum(camera.eye, camera.center);
	gluLookAt(
		camera.center.x, camera.center.y, camera.center.z,
		center.x, center.y, center.z,
		camera.up.x,	camera.up.y, camera.up.z
	);
}

void moveForward(camera* camera, float step) {
	camera->center = sum(camera->center, multiply_by_scalar(camera->eye, step));
}

void moveBackward(camera* camera, float step) {
	camera->center = sum(camera->center, multiply_by_scalar(camera->eye, -step));
}

void strafeLeft(camera* camera, float step) {
	vector3 strafeDirection = cross(camera->eye, camera->up);
	camera->center = sum(camera->center, multiply_by_scalar(strafeDirection, step));
}

void strafeRight(camera* camera, float step) {
	vector3 strafeDirection = cross(camera->eye, camera->up);
	camera->center = sum(camera->center, multiply_by_scalar(strafeDirection, -step));
}


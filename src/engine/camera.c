#include <math.h>
#include <stdio.h>

#include <GL/glut.h>

#include "camera.h"

#define RADIANS(degree) degree * M_PI / 180


camera create_camera() {
	camera new_camera;

	new_camera.eye =    (vector3) { 0, 0, 1 };
	new_camera.center = (vector3) { 0, 2, 0 };
	new_camera.up =     (vector3) { 0, 1, 0 };

	new_camera.yaw = 90;
	new_camera.pitch = 0;

	return new_camera;
}

void look_at(camera camera) {
	vector3 center = sum(camera.eye, camera.center);
	gluLookAt(
		camera.center.x, camera.center.y, camera.center.z,
		center.x, center.y, center.z,
		camera.up.x, camera.up.y, camera.up.z
	);
}

void move_forward(camera* camera, float step) {
	vector3 v = multiply_by_scalar(camera->eye, step);
	camera->center.x += v.x;
	camera->center.z += v.z;
}

void move_backward(camera* camera, float step) {
	vector3 v = multiply_by_scalar(camera->eye, step);
	camera->center.x -= v.x;
	camera->center.z -= v.z;
}

void move_up(camera* camera, float step) {
	camera->center.y += step;
}

void move_down(camera* camera, float step) {
	camera->center.y -= step;
}

void move_left(camera* camera, float step) {
	camera->center = sum(
		camera->center,
		multiply_by_scalar(normalize(cross(camera->eye, camera->up)), step)
	);
}

void move_right(camera* camera, float step) {
	camera->center = subtract(
		camera->center,
		multiply_by_scalar(normalize(cross(camera->eye, camera->up)), step)
	);
}

void mouse_movement(camera* camera, double dx, double dy) {
	camera->yaw += dx;
	camera->pitch += dy;

	// TODO(marcosfons): Check if need to be 89
	if(camera->pitch > 89.99999) {
		camera->pitch = 89.99999;
	} else if(camera->pitch < -89.99999) {
		camera->pitch = -89.99999;
	}

	camera->eye.x = cos(RADIANS(camera->yaw)) * cos(RADIANS(camera->pitch));
	camera->eye.y = sin(RADIANS(camera->pitch));
	camera->eye.z = sin(RADIANS(camera->yaw)) * cos(RADIANS(camera->pitch));

	camera->eye = normalize(camera->eye);
}


#ifndef CAMERA_ENGINE_HEADER
#define CAMERA_ENGINE_HEADER


#include "../utils/vector.h"


typedef struct {
	vector3 eye;
	vector3 center;
	vector3 up;

	double yaw;
	double pitch;
} camera;

camera create_camera();

void look_at(camera camera);

void move_forward(camera* camera, float step);
void move_backward(camera* camera, float step);
void move_left(camera* camera, float step);
void move_right(camera* camera, float step);

void mouse_movement(camera* camera, double dx, double dy);

#endif

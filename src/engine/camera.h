#ifndef CAMERA_ENGINE_HEADER
#define CAMERA_ENGINE_HEADER


#include "../utils/vector.h"


typedef struct {
	vector3 eye;
	vector3 center;
	vector3 up;
} camera;

camera create_camera();

void look_at(camera camera);

void moveForward(camera* camera, float step);
void moveBackward(camera* camera, float step);
void strafeLeft(camera* camera, float step);
void strafeRight(camera* camera, float step);

#endif

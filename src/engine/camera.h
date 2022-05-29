#ifndef CAMERA_ENGINE_HEADER
#define CAMERA_ENGINE_HEADER


// typedef struct {
// 	double x;
// 	double y;
// 	double z;
// } vector3;
//
// vector3 sum(vector3 v1, vector3 v2);
// vector3 cross(vector3 v1, vector3 v2);

typedef struct {
	double eyeX;
	double eyeY;
	double eyeZ;

	double centerX;
	double centerY;
	double centerZ;

	double upX;
	double upY;
	double upZ;
} camera;

camera create_camera();

void look_at(camera camera);

void moveForward(camera* camera, float step);
void moveBackward(camera* camera, float step);
void moveLeft(camera* camera, float step);
void moveRight(camera* camera, float step);

#endif

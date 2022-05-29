#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "vector.h"


vector3 empty_vector() {
	return (vector3) {0, 0, 0};
}

vector3 sum(vector3 v1, vector3 v2) {
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;

	return v1;
}

vector3 subtract(vector3 v1, vector3 v2) {
	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;

	return v1;
}

vector3 multiply_by_scalar(vector3 v, double k) {
	v.x *= k;
	v.y *= k;
	v.z *= k;

	return v;
}

vector3 divide_by_scalar(vector3 v, double k) {
	v.x /= k;
	v.y /= k;
	v.z /= k;

	return v;
}

double magnitude(vector3 v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

double dot(vector3 v1, vector3 v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

vector3 cross(vector3 v1, vector3 v2) {
	vector3 vector;

	vector.x = v1.y * v2.z - v1.z * v2.y;
	vector.y = v1.z * v2.x - v1.x * v2.z;
	vector.z = v1.x * v2.y - v1.y * v2.x;

	return vector;
}

vector3 normalize(vector3 v) {
	double mag = magnitude(v);
	if (mag > 0.0f) {
		return multiply_by_scalar(v, 1.0f / mag);
	}

	return v;
}

void print_vector(vector3 v) {
	printf("(%f, %f, %f)", v.x, v.y, v.z);
}

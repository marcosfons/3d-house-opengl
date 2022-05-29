#ifndef VECTOR_ENGINE_HEADER
#define VECTOR_ENGINE_HEADER

typedef struct {
	double x;
	double y;
	double z;
} vector3;

vector3 empty_vector();

vector3 sum(vector3 v1, vector3 v2);
vector3 subtract(vector3 v1, vector3 v2);

vector3 multiply_by_scalar(vector3 v, double k);
vector3 divide_by_scalar(vector3 v, double k);

double magnitude(vector3 v);

double dot(vector3 v1, vector3 v2);

vector3 cross(vector3 v1, vector3 v2);

vector3 normalize(vector3 v);

void print_vector(vector3 v);

#endif

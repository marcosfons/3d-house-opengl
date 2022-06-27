#ifndef ENGINE_LIGHT_HEADER
#define ENGINE_LIGHT_HEADER


#include <GL/gl.h>

typedef struct {
	GLenum target;

  GLfloat* ambient;
  GLfloat* diffuse;
  GLfloat* specular;
  GLfloat* position;
} light;


light* create_light(GLenum target);

void enable_light(light* light);
void disable_light(light* light);

void debug_light(light* light);

void set_light_ambient(light* light, GLfloat r, GLfloat g, GLfloat b, GLfloat w);
void set_light_diffuse(light* light, GLfloat r, GLfloat g, GLfloat b, GLfloat w);
void set_light_specular(light* light, GLfloat r, GLfloat g, GLfloat b, GLfloat w);
void set_light_position(light* light, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

void reset_light_position(light* light);


#endif

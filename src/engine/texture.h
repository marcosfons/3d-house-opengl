#ifndef TEXTURE_HEADER
#define TEXTURE_HEADER

#include <GL/gl.h>


typedef struct {
	GLuint id;

	GLenum target;
} texture;


texture* load_texture(char* filepath, GLenum target);

texture* load_cubemap(char* faces[6], GLenum target);


#endif

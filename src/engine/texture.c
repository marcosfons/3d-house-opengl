#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../../include/stb/stb_image.h"

#include "texture.h"


texture* load_texture(char* filepath, GLenum target) {
	stbi_set_flip_vertically_on_load(true);

	int width, height, bpp;

	uint8_t* rgb_image = stbi_load(filepath, &width, &height, &bpp, STBI_rgb);
	if (rgb_image == NULL) {
		fprintf(stderr, "Error while loading texture from file: %s\n", filepath);
		exit(EXIT_FAILURE);
	}

	texture* new_texture = malloc(sizeof(texture));

	new_texture->target = target;

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &new_texture->id);

	glActiveTexture(target);
	glBindTexture(GL_TEXTURE_2D, new_texture->id);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb_image);

	stbi_image_free(rgb_image);

	return new_texture;
}


texture* load_cubemap(char* faces[6], GLenum target) {
	stbi_set_flip_vertically_on_load(false);
	texture* new_texture = malloc(sizeof(texture));
	new_texture->target = target;

	glBindTexture(GL_TEXTURE_2D, 0);
	glGenTextures(1, &new_texture->id);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glActiveTexture(target);
	glBindTexture(GL_TEXTURE_CUBE_MAP, new_texture->id);

	int width, height, bpp;
	for (GLuint i = 0; i < 6; i++) {
		uint8_t* rgb_image = stbi_load(faces[i], &width, &height, &bpp, STBI_rgb);
		if (rgb_image == NULL) {
			fprintf(stderr, "Error while loading cubemap texture from file: %s\n", faces[i]);
			exit(EXIT_FAILURE);
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb_image);

		stbi_image_free(rgb_image);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0); 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);

	return new_texture;
}




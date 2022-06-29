#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "mtl_parser.h"


materials* parse_mtl(FILE* fp) {
	materials* mtls = malloc(sizeof(materials));
	mtls->mtls = NULL;
	mtls->count = 0;

	char* line = NULL;
	size_t length = 0;
	long int nread;

	while ((nread = getline(&line, &length, fp)) != -1) {
		parse_mtl_line(mtls, line);
	}

	free(line);

	// Missing count
	return mtls;
}

// Here the token is not required.
// I am using it just to make it clear that it is using strtok
// It's not needed because strtok uses static variables to store
// the state
static float* parse_color(char* token) {
	float* color = malloc(4 * sizeof(float));
	// TODO(marcosfons): Check this value
	color[3] = 1;
	int count = 0;

	const char* sep = " \n\0";
	token = strtok(NULL, sep);

	while (token != NULL) {
		float value = strtof(token, NULL);
		if (errno) {
			perror("Syntax error while converting color to float");
			exit(EXIT_FAILURE);
		}

		color[count] = value;
		if (count > 2) {
			fprintf(stderr, "Syntax error color with 4 values");
			exit(EXIT_FAILURE);
		}

		token = strtok(NULL, sep);
		count += 1;
	}

	return color;
}

void parse_mtl_line(materials* materials, char* line) {
	const char* sep = " \n\0";

	char* token = strtok(line, sep);

	if (token == NULL ||
			token[0] == '#' || 
			token[0] == ' ' || 
			token[0] == '\n' || 
			token[0] == '\0') {
		return;
	} else if (strcmp(token, "newmtl") == 0) {
		token = strtok(NULL, sep);
		if (token == NULL) {
			fprintf(stderr, "The material needs a name");
			token = "";
		}

		materials->count += 1;
		materials->mtls = realloc(materials->mtls, materials->count * sizeof(material));
		materials->mtls[materials->count - 1].name = strdup(token);
	} else if (strcmp(token, "Ns") == 0) {
		token = strtok(NULL, sep);

		float value = strtof(token, NULL);
		if (errno) {
			perror("Syntax error while converting Ns to float");
			exit(EXIT_FAILURE);
		}
		materials->mtls[materials->count - 1].shininess = value;
	} else if (strcmp(token, "Ka") == 0) {
		materials->mtls[materials->count - 1].ambient_color = parse_color(token);
	} else if (strcmp(token, "Kd") == 0) {
		materials->mtls[materials->count - 1].diffuse_color = parse_color(token);
	} else if (strcmp(token, "Ks") == 0) {
		materials->mtls[materials->count - 1].specular_color = parse_color(token);
	} else if (strcmp(token, "Ni") == 0) {
		// I don't know what Ni is
	} else if (strcmp(token, "d") == 0 || strcmp(token, "Tr") == 0) {
		token = strtok(NULL, sep);

		float value = strtof(token, NULL);
		if (errno) {
			perror("Syntax error while converting d to float");
			exit(EXIT_FAILURE);
		}
		materials->mtls[materials->count - 1].transparency = value;

		if (materials->mtls[materials->count - 1].diffuse_color) {
			materials->mtls[materials->count - 1].diffuse_color[3] = value;
		}
	} else if (strcmp(token, "illum") == 0) {
		token = strtok(NULL, sep);

		int value = strtol(token, NULL, 10);
		if (errno) {
			perror("Syntax error while converting illum to int");
			exit(EXIT_FAILURE);
		}
		materials->mtls[materials->count - 1].illumination_mode = value;
	}
}

void free_materials(materials* materials) {
	for (int i = 0; i < materials->count; i++) {
		if (materials->mtls[i].name != NULL) {
			free(materials->mtls[i].name);
		}

		if (materials->mtls[i].ambient_color != NULL) {
			free(materials->mtls[i].ambient_color);
		}
		if (materials->mtls[i].diffuse_color != NULL) {
			free(materials->mtls[i].diffuse_color);
		}
		if (materials->mtls[i].specular_color != NULL) {
			free(materials->mtls[i].specular_color);
		}
	}

	free(materials->mtls);
	free(materials);
}


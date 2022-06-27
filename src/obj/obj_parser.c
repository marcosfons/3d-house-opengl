#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "obj_parser.h"


#include "../../include/stb/stb_ds.h"

obj* parse_obj(char* filepath, char* material_filepath) {
	FILE* obj_file = fopen(filepath, "r");
	if (obj_file == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	printf("Parsing obj from file: %s\n", filepath);

	materials* mtls = NULL;
	if (material_filepath != NULL) {
		FILE* mtl_file = fopen(material_filepath, "r");
		if (mtl_file == NULL) {
			errno = 0; // TODO(marcosfons): Check if there is a better way to do it
		} else {
			printf("Parsing materials from file: %s\n", material_filepath);
			mtls = parse_mtl(mtl_file);
			fclose(mtl_file);
		}
	}

	// TODO(marcosfons): Add materials
	obj* new_obj = parse_from_fp(obj_file, mtls);

	fclose(obj_file);

	return new_obj;
}

void add_face(obj* obj, faces_by_material* current_material, face face) {
	face_types type = face.elements[0].normal_is_set + (face.elements[0].texture_is_set << 1);

	int key = type + (face.elements_count << 2);

	if (hmgeti(current_material->faces, key) < 0) {
		faces new_faces = { NULL, NULL, NULL, -1, -1, -1 };
		hmput(current_material->faces, key, new_faces);
	}

	faces* fs = &hmget(current_material->faces, key);

	for (int i = 0; i < face.elements_count; i++) {
		arrput(fs->vertex_index, face.elements[i].vertex_index - 1);
		if (face.elements[i].normal_is_set) {
			arrput(fs->normal_index, face.elements[i].normal_index - 1);
		}
		if (face.elements[i].texture_is_set) {
			arrput(fs->texture_index, face.elements[i].texture_index - 1);
		}
	} 
}

void add_element(face* face, element element) {
	face->elements_count += 1;
	face->elements = realloc(face->elements, face->elements_count * sizeof(element));

	face->elements[face->elements_count - 1] = element;
}

obj* parse_from_fp(FILE* fp, materials* materials) {
	obj* new_obj = malloc(sizeof(obj));

	new_obj->vertices = NULL;
	new_obj->normals = NULL;
	new_obj->texture_vertices = NULL;

	new_obj->vertex_vbo = -1;
	new_obj->normal_vbo = -1;
	new_obj->texture_vertices_vbo = -1;

	// obj->faces = malloc(INITIAL_LIST_SIZE * sizeof(face));
	// obj->faces_count = 0;
	// obj->faces_size = INITIAL_LIST_SIZE;

	faces_by_material default_f = { "", NULL, NULL };
	new_obj->faces_by_material = NULL;
	shputs(new_obj->faces_by_material, default_f);

	if (materials != NULL) {
		for (int i = 0; i < materials->count; i++) {	
			faces_by_material new_faces = { 
				materials->mtls[i].name, 
				materials->mtls + i,
				NULL
			};
			shputs(new_obj->faces_by_material, new_faces);
		}
	}

	char* line = NULL;
	size_t length = 0;
	long int nread;

	faces_by_material* current_material = &shgets(new_obj->faces_by_material, "");
	
	while ((nread = getline(&line, &length, fp)) != -1) {
		parse_line(new_obj, &current_material, line);
	}

	free(line);
	if (nread == -1 && getc(fp) != EOF) {
		perror("getline");
	}

	return new_obj;
}

void parse_line(obj* obj, faces_by_material** current_material, char* line) {
	const char* sep = " \n\0";

	char* token = strtok(line, sep);

	if (token == NULL ||
			token[0] == '#' || 
			token[0] == ' ' || 
			token[0] == '\n' || 
			token[0] == '\0') {
		return;
	} else if (strcmp(token, "v") == 0) {
		vertex new_vertex;
		new_vertex.w = 1;

		int count = 0;
		token = strtok(NULL, sep);

		while (token != NULL) {
			float value = strtof(token, NULL);
			if (errno) {
				perror("Syntax error while converting to float");
				exit(EXIT_FAILURE);
			}

			if (count == 0) {
				new_vertex.x = value;
			} else if (count == 1) {
				new_vertex.y = value;
			} else if (count == 2) {
				new_vertex.z = value;
			} else if (count == 3) {
				if (value < 0 || value > 1) {
					fprintf(stderr, "Syntax error in vertex w must be between 0 and 1");
					exit(EXIT_FAILURE);
				}
				new_vertex.w = value;
			} else {
				fprintf(stderr, "Syntax error normal with 5 values");
				exit(EXIT_FAILURE);
			}

			token = strtok(NULL, sep);
			count += 1;
		}

		arrput(obj->vertices, new_vertex);
	} else if (strcmp(token, "vn") == 0) {
		normal new_normal;

		int count = 0;
		token = strtok(NULL, sep);

		while (token != NULL) {
			float value = strtof(token, NULL);
			if (errno) {
				perror("Syntax error while converting to float");
				exit(EXIT_FAILURE);
			}

			if (count == 0) {
				new_normal.x = value;
			} else if (count == 1) {
				new_normal.y = value;
			} else if (count == 2) {
				new_normal.z = value;
			} else {
				fprintf(stderr, "Syntax error normal with 4 values");
				exit(EXIT_FAILURE);
			}

			token = strtok(NULL, sep);
			count += 1;
		}

		arrput(obj->normals, new_normal);
	} else if (strcmp(token, "f") == 0) {
		face new_face;
		// new_face.material = obj->current_material;

		new_face.elements_count = 0;
		new_face.elements = NULL;

		token = strtok(NULL, sep);

		while (token != NULL) {
			element new_element;
			new_element.normal_is_set = false;
			new_element.texture_is_set = false;

			int count = 0;
			char* element_token = strtok_r(NULL, "/", &token);

			while (element_token != NULL) {
				int value = strtol(element_token, NULL, 10);
				if (errno) {
					perror("Syntax error while converting to integer");
					exit(EXIT_FAILURE);
				}

				if (count == 0) {
					new_element.vertex_index = value;
				} else if (count == 1) {
					if ((token - 2)[0] == '/') {
						count += 1;
					} else {
						new_element.texture_index = value;
						new_element.texture_is_set = true;
					}
				} else if (count == 2) {
					new_element.normal_index = value;
					new_element.normal_is_set = true;
				} else {
					fprintf(stderr, "Syntax error element with 4 values");
					exit(EXIT_FAILURE);
				}

				element_token = strtok_r(NULL, "/", &token);
				count += 1;
			}

			add_element(&new_face, new_element);

			token = strtok(token + 1, sep);
		}

		if (new_face.elements_count < 3) {
			fprintf(stderr, "Syntax error face with less than 3 elements");
			exit(EXIT_FAILURE);
		}

		add_face(obj, *current_material, new_face);
	} else if (strcmp(token, "vt") == 0) {
		texture_vertex new_texture_vertex;
		new_texture_vertex.v = 0;
		new_texture_vertex.w = 0;

		int count = 0;
		token = strtok(NULL, sep);

		while (token != NULL) {
			float value = strtof(token, NULL);
			if (errno) {
				perror("Syntax error while converting to float");
				exit(EXIT_FAILURE);
			}

			if (count == 0) {
				new_texture_vertex.u = value;
			} else if (count == 1) {
				new_texture_vertex.v = value;
			} else if (count == 2) {
				new_texture_vertex.w = value;
			} else {
				fprintf(stderr, "Syntax error texture vertex with 4 values");
				exit(EXIT_FAILURE);
			}

			token = strtok(NULL, sep);
			count += 1;
		}

		arrput(obj->texture_vertices, new_texture_vertex);
	} else if (strcmp(token, "usemtl") == 0) {
		token = strtok(NULL, sep);

		if (shgeti(obj->faces_by_material, token) > 0) {
			*current_material = &shgets(obj->faces_by_material, token);
		} else {
			*current_material = &shgets(obj->faces_by_material, "");
		}
	}
}


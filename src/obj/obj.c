#include <stdlib.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>

#include "../../include/stb/stb_ds.h"

#include "obj.h"
#include "obj_parser.h"

void init_obj_vbos(obj* obj) {
	if (obj->vertex_vbo == -1) {
		glGenBuffers(1, &(obj->vertex_vbo));
		glBindBuffer(GL_ARRAY_BUFFER, obj->vertex_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * arrlen(obj->vertices), obj->vertices, GL_STATIC_DRAW);

		int material_count = shlen(obj->faces_by_material);
		for (int i = material_count == 1 ? 0 : 1; i < material_count; i++) {
			for (int j = 0; j < hmlen(obj->faces_by_material[i].faces); j++) {
				faces_type* pfaces = obj->faces_by_material[i].faces + j;
				// face_types type = pfaces->key & (1 << 2) - 1;

				if (pfaces->value.vertex_index_vbo == -1) {
					glGenBuffers(1, &pfaces->value.vertex_index_vbo);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pfaces->value.vertex_index_vbo);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * arrlen(pfaces->value.vertex_index), pfaces->value.vertex_index, GL_STATIC_DRAW);
				}

				// if (type == VERTEX_NORMAL || type == VERTEX_NORMAL_TEXTURE) {
				// 	glGenBuffers(1, &pfaces->value.normal_index_vbo);
				// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pfaces->value.normal_index_vbo);
				// 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, arrlen(pfaces->value.normal_index), pfaces->value.normal_index, GL_STATIC_DRAW);
				// }

				// if (type == VERTEX_TEXTURE || type == VERTEX_NORMAL_TEXTURE) {
				// 	glGenBuffers(1, &pfaces->value.texture_index_vbo);
				// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pfaces->value.texture_index_vbo);
				// 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, arrlen(pfaces->value.texture_index), pfaces->value.texture_index, GL_STATIC_DRAW);
				// }
			}
		}
	}
}

static void set_obj_material(material* material) {
	GLenum face = GL_FRONT;
	if (material->ambient_color) {
		// glMaterialfv(face, GL_AMBIENT, material->ambient_color);
	}
	if (material->diffuse_color) {
		glMaterialfv(face, GL_AMBIENT_AND_DIFFUSE, material->diffuse_color);
		// glMaterialfv(face, GL_DIFFUSE, material->diffuse_color);
	}

	if (material->specular_color != NULL) {
		glMaterialfv(face, GL_SPECULAR, material->specular_color);
	}
	glMaterialf(face, GL_SHININESS, material->shininess);
}

void draw_obj(obj* obj) {
	glBindBuffer(GL_ARRAY_BUFFER, obj->vertex_vbo);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	int material_count = shlen(obj->faces_by_material);
	for (int i = material_count == 1 ? 0 : 1; i < material_count; i++) {
		if (obj->faces_by_material[i].material) {
			set_obj_material(obj->faces_by_material[i].material);
		}

		for (int j = 0; j < hmlen(obj->faces_by_material[i].faces); j++) {
			faces_type* pfaces = obj->faces_by_material[i].faces + j;
			unsigned mask = (1 << 2) - 1;
			// face_types type = pfaces->key & mask;
			int elements = pfaces->key >> 2;
			
			if (elements == 2) {
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pfaces->value.vertex_index_vbo);
				glDrawElements(GL_LINES, arrlen(pfaces->value.vertex_index), GL_UNSIGNED_INT, 0);
			} else if (elements == 3) {
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pfaces->value.vertex_index_vbo);
				glDrawElements(GL_TRIANGLES, arrlen(pfaces->value.vertex_index), GL_UNSIGNED_INT, 0);
			} else if (elements == 4) {
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pfaces->value.vertex_index_vbo);
				glDrawElements(GL_QUADS, arrlen(pfaces->value.vertex_index), GL_UNSIGNED_INT, 0);
			} else {
				for (int k = 0; k < arrlen(pfaces->value.vertex_index); k += elements) {
					glBegin(GL_POLYGON);
					for (int l = 0; l < elements; l++) {
						vertex v = obj->vertices[pfaces->value.vertex_index[k + l]];
						glVertex3f(v.x, v.y, v.z);
					}
					glEnd();
				}
			}
		}
	}

	glDisableVertexAttribArray(0);
}

void print_obj(obj obj) {
	if (obj.filepath != NULL) {
		printf("Filepath: %s\n\n", obj.filepath);
	}

	// TODO(marcosfons): Redo this code
	// printf("Quantity of faces: %d\n", obj.faces_count);
	// printf("How many vertices per face: %d\n", obj.polygon_vertex_count);

	printf("Vertex count: %lu\n", obj.vertices != NULL ? arrlen(obj.vertices) : 0);
	printf("There are normals: %lu\n", obj.normals != NULL ? arrlen(obj.normals) : 0);
	printf("There are texture: %lu\n", obj.texture_vertices != NULL ? arrlen(obj.texture_vertices) : 0);
}

void destroy_obj(obj* obj) {
	// TODO(marcosfons): Redo this code
	// free(obj->element_count);
	free(obj->vertices);

	if (obj->filepath != NULL) {
		free(obj->filepath);
	}

	if (obj->normals != NULL) {
		free(obj->normals);
	}

	if (obj->texture_vertices != NULL) {
		free(obj->texture_vertices);
	}

	free(obj);
}

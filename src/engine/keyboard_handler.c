#include <stdlib.h>

#include <GL/glut.h>

#include "../../include/stb/stb_ds.h"

#include "keyboard_handler.h"


keyboard_handler* create_keyboard_handler() {
	keyboard_handler* handler = malloc(sizeof(keyboard_handler));

	handler->keys = NULL;
	handler->special_keys = NULL;

	return handler;
}

void add_key(keyboard_handler* handler, const char* name, int key, float* value, key_mode_func mode, float optional) {
	execute_key_func func = NULL;

	if (mode == SWITCH) {
		func = key_switch_func;
	} else if (mode == INCREMENT) {
		func = key_increment_func;
	} else if (mode == SET) {
		func = key_set_func;
	}
	key_handler ka = (key_handler) { key, value, func, name, false, optional};
	hmputs(handler->keys, ka);
}

void add_special_key(keyboard_handler* handler, const char* name, int key, float* value, key_mode_func mode, float optional) {
	execute_key_func func = NULL;

	if (mode == SWITCH) {
		func = key_switch_func;
	} else if (mode == INCREMENT) {
		func = key_increment_func;
	} else if (mode == SET) {
		func = key_set_func;
	}
	key_handler ka = (key_handler) { key, value, func, name, false, optional};
	hmputs(handler->special_keys, ka);
}


void execute_key(keyboard_handler* handler, int key, bool is_pressed) {
	int index = hmgeti(handler->keys, key);
	if (index < 0) return;

	if (!handler->keys[index].is_pressed && is_pressed) {
		if (handler->keys[index].func != NULL) {
			handler->keys[index].func(handler->keys + index);
		}
	}

	handler->keys[index].is_pressed = is_pressed;
	glutPostRedisplay();
}

void execute_special_key(keyboard_handler* handler, int key, bool is_pressed) {
	int index = hmgeti(handler->special_keys, key);
	if (index < 0) return;

	if (!handler->special_keys[index].is_pressed && is_pressed) {
		if (handler->special_keys[index].func != NULL) {
			handler->special_keys[index].func(handler->special_keys + index);
		}
	}

	handler->special_keys[index].is_pressed = is_pressed;
}

key_handler* get_key(keyboard_handler* handler, int key) {
	if (handler->keys == NULL) return NULL;
	int index = hmgeti(handler->keys, key);
	return index >= 0 ? handler->keys + index : NULL; 
}
key_handler* get_special_key(keyboard_handler* handler, int key) {
	if (handler->special_keys == NULL) return NULL;
	int index = hmgeti(handler->special_keys, key);
	return index >= 0 ? handler->special_keys + index : NULL; 
}

void key_switch_func(key_handler* key) {
	if (*key->value == 0) {
		*key->value = 1;
	} else {
		*key->value = 0;
	}
}

void key_increment_func(key_handler* key) {
	*key->value += key->optional;
}

void key_set_func(key_handler* key) {
	*key->value = key->optional;
}


void print_key_help(keyboard_handler* handler) {
	if (handler->keys == NULL) return;
}

void print_special_key_help(keyboard_handler* handler) {
	if (handler->special_keys == NULL) return;
}

void destroy_keyboard_handler(keyboard_handler* handler) {
	hmfree(handler->keys);
	hmfree(handler->special_keys);

	free(handler);
}

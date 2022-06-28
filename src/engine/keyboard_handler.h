#ifndef ENGINE_KEYBOARD_HANDLER_HEADER
#define ENGINE_KEYBOARD_HANDLER_HEADER


#include <stdbool.h>

typedef struct key_t key_handler;

typedef void (*execute_key_func)(key_handler*);

typedef enum {
	SWITCH,
	INCREMENT,
	SET,
	NONE // In this case it only uses the is_pressed
} key_mode_func;

typedef struct key_t{
	int key;
	float* value;
	execute_key_func func;
	const char* name;
	bool is_pressed;

	// An optional value to help in functions
	// INCREMENT the increment value
	// SET the set value
	// SWITCH unused
	float optional;
} key_handler;

typedef struct {
	key_handler* keys;

	key_handler* special_keys;
} keyboard_handler;


keyboard_handler* create_keyboard_handler();

void add_key(keyboard_handler* handler, const char* name, int key, float* value, key_mode_func mode, float optional);
void add_special_key(keyboard_handler* handler, const char* name, int key, float* value, key_mode_func mode, float optional);

void execute_key(keyboard_handler* handler, int key, bool is_pressed);
void execute_special_key(keyboard_handler* handler, int key, bool is_pressed);

key_handler* get_key(keyboard_handler* handler, int key);
key_handler* get_special_key(keyboard_handler* handler, int key);

void key_switch_func(key_handler* key);
void key_increment_func(key_handler* key);
void key_set_func(key_handler* key);

void print_key_help(keyboard_handler* handler);

void print_special_key_help(keyboard_handler* handler);

void destroy_keyboard_handler(keyboard_handler* handler);


#endif

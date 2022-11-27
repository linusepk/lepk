#define LEPK_DA_IMPLEMENTATION
#define LEPK_DA_TEST
#include "lepk_da.h"

#define LEPK_FILE_IMPLEMENTATION
#define LEPK_FILE_TEST
#include "lepk_file.h"

#define LEPK_HT_IMPLEMENTATION
#define LEPK_HT_TEST
#include "lepk_ht.h"

/* #define LEPK_WINDOW_IMPLEMENTATION */
/* #include "lepk_window.h" */

/* void resize_callback(LepkWindow *window, int width, int height) { */
/* 	(void) window; */
/* 	printf("Resize: %d, %d\n", width, height); */
/* } */
/* void key_callback(LepkWindow *window, LepkKey keycode, int scancode, bool pressed, LepkMod mods) { */
/* 	(void) window; */
/* 	printf("Key: %2d, %2d, %2d, %-8s\n", keycode, scancode, mods, pressed ? "pressed" : "released"); */
/* } */


int main(void) {
	lepk_da_test();
	lepk_file_test();
	lepk_ht_test();

	/* LepkWindow *window = lepk_window_create(800, 600, "Linux Window", true); */
	/* lepk_window_callback_resize(window, resize_callback); */
	/* lepk_window_callback_key(window, key_callback); */

	/* while (lepk_window_is_open(window)) { */
	/* 	lepk_window_poll_events(window); */
	/* } */

	/* lepk_window_destroy(window); */
	return 0;
}

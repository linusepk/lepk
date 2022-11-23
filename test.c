#define LEPK_DA_IMPLEMENTATION
#define LEPK_DA_TEST
#include "lepk_da.h"

#define LEPK_FILE_IMPLEMENTATION
#define LEPK_FILE_TEST
#include "lepk_file.h"

#define LEPK_WINDOW_IMPLEMENTATION
#include "lepk_window.h"

#include <stdio.h>

void resize_callback(LepkWindow *window, int width, int height) {
	(void) window;
	printf("%d, %d\n", width, height);
}

int main(void) {
	lepk_da_test();
	lepk_file_test();

	LepkWindow *window = lepk_window_create(800, 600, "Linux Window", false);
	lepk_window_callback_resize(window, resize_callback);

	while (lepk_window_is_open(window)) {
		lepk_window_poll_events(window);
	}

	lepk_window_destroy(window);
	return 0;
}

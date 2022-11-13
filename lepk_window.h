#ifndef LEPK_WINDOW_H
#define LEPK_WINDOW_H

#include <stdbool.h>

#ifdef LEPK_WINDOW_STATIC
#define LEPKWINDOW static
#else /* LEPK_WINDOW_STATIC */
#define LEPKWINDOW extern
#endif /* LEPK_WINDOW_STATIC */

typedef struct LepkWindow LepkWindow;

LEPKWINDOW LepkWindow *lepk_window_create(int width, int height, const char *title, bool resizable);
LEPKWINDOW void lepk_window_destroy(LepkWindow *window);
LEPKWINDOW bool lepk_window_is_open(const LepkWindow *window);
LEPKWINDOW void lepk_window_poll_events(LepkWindow *window);

#ifdef LEPK_WINDOW_VULKAN
#ifdef __linux__
#define VK_USE_PLATFORM_XCB_KHR
#endif /* __linux__ */
#ifdef _WIN32
#define VK_UES_PLATFORM_WIN32_KHR
#endif /* _WIN32 */
#include <vulkan/vulkan.h>
LEPKWINDOW VkSurfaceKHR lepk_window_get_surface(const LepkWindow *window, VkInstance instance);
#endif /* LEPK_WINDOW_VULKAN */

#ifdef LEPK_WINDOW_IMPLEMENTATION
/*
 * Linux
 */
#ifdef __linux__

#include <xcb/xcb.h>
#include <malloc.h>
#include <stddef.h>
#include <string.h>

struct LepkWindow {
	xcb_connection_t *connection;
	xcb_window_t window;
	xcb_screen_t *screen;
	xcb_atom_t wm_protocols;
	xcb_atom_t wm_delete_win;
	bool is_open;
};

LepkWindow *lepk_window_create(int width, int height, const char *title, bool resizable) {
	LepkWindow *window = malloc(sizeof(LepkWindow));
	window->is_open = 1;

	int screenp = 0;
	window->connection = xcb_connect(NULL, &screenp);
	if (xcb_connection_has_error(window->connection)) {
		return NULL;
	}

	xcb_screen_iterator_t iter = xcb_setup_roots_iterator(xcb_get_setup(window->connection));
	for (int i = screenp; i > 0; i--) {
		xcb_screen_next(&iter);
	}
	window->screen = iter.data;

	/* Create window. */
	window->window = xcb_generate_id(window->connection);
	int event_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	int value_list[] = { window->screen->black_pixel, 0 };

	xcb_create_window(
		window->connection,
		XCB_COPY_FROM_PARENT,
		window->window,
		window->screen->root,
		0, 0, // Position
		width, height,
		0,
		XCB_WINDOW_CLASS_INPUT_OUTPUT,
		window->screen->root_visual,
		event_mask,
		value_list
	);

	/* Set title. */
	xcb_change_property(
		window->connection,
		XCB_PROP_MODE_REPLACE,
		window->window,
		XCB_ATOM_WM_NAME,
		XCB_ATOM_STRING,
		8,
		strlen(title),
		title
	);

	/* Change resizability. */
	if (!resizable) {
		typedef struct {
			unsigned int flags;
			int x, y;
			int width, height;
			int min_width, min_height;
			int max_width, max_height;
			int width_inc, height_inc;
			int min_apsect_num, min_aspec_den;
			int max_apsect_num, max_aspec_den;
			int base_width, base_height;
			unsigned int win_gravity;
		} WMSizeHints;
		enum {
			WM_SIZE_HINT_US_POSITION   = 1U << 0,
			WM_SIZE_HINT_US_SIZE       = 1U << 1,
			WM_SIZE_HINT_P_POSITION    = 1U << 2,
			WM_SIZE_HINT_P_SIZE        = 1U << 3,
			WM_SIZE_HINT_P_MIN_SIZE    = 1U << 4,
			WM_SIZE_HINT_P_MAX_SIZE    = 1U << 5,
			WM_SIZE_HINT_P_RESIZE_INC  = 1U << 6,
			WM_SIZE_HINT_P_ASPECT      = 1U << 7,
			WM_SIZE_HINT_BASE_SIZE     = 1U << 8,
			WM_SIZE_HINT_P_WIN_GRAVITY = 1U << 9
		};

		WMSizeHints hints = {
			.flags = WM_SIZE_HINT_P_WIN_GRAVITY,
			.win_gravity = XCB_GRAVITY_STATIC
		};

		hints.flags |= WM_SIZE_HINT_P_MIN_SIZE | WM_SIZE_HINT_P_MAX_SIZE;
		hints.min_width = width;
		hints.min_height = height;
		hints.max_width = width;
		hints.max_height = height;

		xcb_change_property(window->connection, XCB_PROP_MODE_REPLACE, window->window, XCB_ATOM_WM_NORMAL_HINTS, XCB_ATOM_WM_SIZE_HINTS, 32, sizeof(WMSizeHints) >> 2, &hints);
	}

	/* Close button. */
	xcb_intern_atom_cookie_t wm_delete_cookie = xcb_intern_atom(window->connection, 0, strlen("WM_DELETE_WINDOW"), "WM_DELETE_WINDOW");
	xcb_intern_atom_cookie_t wm_protocols_cookie = xcb_intern_atom(window->connection, 0, strlen("WM_PROTOCOLS"), "WM_PROTOCOLS");
	xcb_intern_atom_reply_t *wm_delete_reply = xcb_intern_atom_reply(window->connection, wm_delete_cookie, NULL);
	xcb_intern_atom_reply_t *wm_protocols_reply = xcb_intern_atom_reply(window->connection, wm_protocols_cookie, NULL);
	window->wm_delete_win = wm_delete_reply->atom;
	window->wm_protocols = wm_protocols_reply->atom;

	xcb_change_property(window->connection, XCB_PROP_MODE_REPLACE, window->window, wm_protocols_reply->atom, 4, 32, 1, &wm_delete_reply->atom);

	// Send window to the X server
	xcb_map_window(window->connection, window->window);
	xcb_flush(window->connection);

	return window;
}

void lepk_window_destroy(LepkWindow *window) {
	xcb_destroy_window(window->connection, window->window);
}

bool lepk_window_is_open(const LepkWindow *window) {
	return window->is_open;
}

void lepk_window_poll_events(LepkWindow *window) {
	xcb_generic_event_t *event;
	xcb_client_message_event_t *cm;

	while ((event = xcb_poll_for_event(window->connection))) {
		switch (event->response_type & ~0x80) {
			case XCB_CLIENT_MESSAGE:
				cm = (xcb_client_message_event_t *) event;
				/* Window close event. */
				if (cm->data.data32[0] == window->wm_delete_win) {
					window->is_open = false;
				}
				break;
			default:
				break;
		}
		free(event);
	}
}

#ifdef LEPK_WINDOW_VULKAN
VkSurfaceKHR lepk_window_get_surface(const LepkWindow *window, VkInstance instance) {
	VkSurfaceKHR surface;

	VkXcbSurfaceCreateInfoKHR surface_create_info = {0};
	surface_create_info.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
	surface_create_info.pNext = NULL;
	surface_create_info.flags = 0;
	surface_create_info.connection = window->connection;
	surface_create_info.window = window->window;

	if (vkCreateXcbSurfaceKHR(instance, &surface_create_info, NULL, &surface) != VK_SUCCESS) {
		return NULL;
	}

	return surface;
}
#endif /* LEPK_WINDOW_VULKAN */

#endif /* __linux__ */

/*
 * Windows
 */
#ifdef _WIN32
#endif /* _WIN32 */

#endif /* LEPK_WINDOW_IMPLEMENTATION */
#endif /* LEPK_WINDOW_H */

#include "lepk_window.h"

#undef LEPKWINDOW
#ifdef LEPK_WINDOW_STATIC
#define LEPKWINDOW static
#else /* LEPK_WINDOW_STATIC */
#define LEPKWINDOW
#endif /* LEPK_WINDOW_STATIC */

/* Linux. */
#ifdef LEPK_WINDOW_OS_LINUX
#include <malloc.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/XKBlib.h>

typedef struct {
	Display *display;
	Window window;
	bool is_open;
	Atom wm_delete_window;
	int width;
	int height;

	LepkKey key_table[256];
	unsigned long scan_table[256];

	/* Callbacks. */
	LepkResizeCallback resize_callback;
	LepkKeyCallback key_callback;
} Lepk__LinuxWindow;

LEPKWINDOW LepkWindow *lepk_window_create(int width, int height, const char *title, bool resizable) {
	Lepk__LinuxWindow *window = malloc(sizeof(Lepk__LinuxWindow));

	window->display = XOpenDisplay(NULL);
	if (window->display == NULL) {
		return NULL;
	}

	int root = DefaultRootWindow(window->display);
	int screen = DefaultScreen(window->display);

	/* Check if display is compatible. */
	int screen_bit_depth = 24;
	XVisualInfo visinfo = {0};
	if (!XMatchVisualInfo(window->display, screen, screen_bit_depth, TrueColor, &visinfo)) {
		return NULL;
	}

	/* Configure window attributes. */
	XSetWindowAttributes window_attrs = {0};
	window_attrs.background_pixel = 0;
	window_attrs.colormap = XCreateColormap(window->display, root, visinfo.visual, AllocNone);
	/* Events that the window will accept. */
	window_attrs.event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask | FocusChangeMask;
	unsigned long attirubte_mask = CWBackPixel | CWColormap | CWEventMask;

	/* Create window. */
	window->window = XCreateWindow(window->display, root, 0, 0, width, height, 0, visinfo.depth, InputOutput, visinfo.visual, attirubte_mask, &window_attrs);
	if (!window->window) {
		return NULL;
	}

	/* Set title. */
	XStoreName(window->display, window->window, title);

	/* Set resizability. */
	if (!resizable) {
		XSizeHints hints = {0};
		hints.flags = PMinSize | PMaxSize;
		hints.min_width  = width;
		hints.min_height = height;
		hints.max_width  = width;
		hints.max_height = height;
		XSetWMNormalHints(window->display, window->window, &hints);
	}

	/* Show window. */
	XMapWindow(window->display, window->window);
	/* Send all X commands to X-server. */
	XFlush(window->display);

	/* Get close command. */
	window->wm_delete_window = XInternAtom(window->display, "WM_DELETE_WINDOW", false);
	if (!XSetWMProtocols(window->display, window->window, &window->wm_delete_window, 1)) {
		return NULL;
	}

	/* Create key lookup table. */
	/* NOTE: Support for most of the keyboard, no cluse about the US layout though. */
	const struct {
		LepkKey key;
		long x_key;
	} keymap[] = {
		/* Letters */
		{LEPK_KEY_A, XK_a},
		{LEPK_KEY_B, XK_b},
		{LEPK_KEY_C, XK_c},
		{LEPK_KEY_D, XK_d},
		{LEPK_KEY_E, XK_e},
		{LEPK_KEY_F, XK_f},
		{LEPK_KEY_G, XK_g},
		{LEPK_KEY_H, XK_h},
		{LEPK_KEY_I, XK_i},
		{LEPK_KEY_J, XK_j},
		{LEPK_KEY_K, XK_k},
		{LEPK_KEY_L, XK_l},
		{LEPK_KEY_M, XK_m},
		{LEPK_KEY_N, XK_n},
		{LEPK_KEY_O, XK_o},
		{LEPK_KEY_P, XK_p},
		{LEPK_KEY_Q, XK_q},
		{LEPK_KEY_R, XK_r},
		{LEPK_KEY_S, XK_s},
		{LEPK_KEY_T, XK_t},
		{LEPK_KEY_U, XK_u},
		{LEPK_KEY_V, XK_v},
		{LEPK_KEY_W, XK_w},
		{LEPK_KEY_X, XK_x},
		{LEPK_KEY_Y, XK_y},
		{LEPK_KEY_Z, XK_z},

		/* Numbers. */
		{LEPK_KEY_0, XK_0},
		{LEPK_KEY_1, XK_1},
		{LEPK_KEY_2, XK_2},
		{LEPK_KEY_3, XK_3},
		{LEPK_KEY_4, XK_4},
		{LEPK_KEY_5, XK_5},
		{LEPK_KEY_6, XK_6},
		{LEPK_KEY_7, XK_7},
		{LEPK_KEY_8, XK_8},
		{LEPK_KEY_9, XK_9},

		/* Function keys. */
		{LEPK_KEY_F1,  XK_F1},
		{LEPK_KEY_F2,  XK_F2},
		{LEPK_KEY_F3,  XK_F3},
		{LEPK_KEY_F4,  XK_F4},
		{LEPK_KEY_F5,  XK_F5},
		{LEPK_KEY_F6,  XK_F6},
		{LEPK_KEY_F7,  XK_F7},
		{LEPK_KEY_F8,  XK_F8},
		{LEPK_KEY_F9,  XK_F9},
		{LEPK_KEY_F10, XK_F10},
		{LEPK_KEY_F11, XK_F11},
		{LEPK_KEY_F12, XK_F12},
		{LEPK_KEY_F13, XK_F13},
		{LEPK_KEY_F14, XK_F14},
		{LEPK_KEY_F15, XK_F15},
		{LEPK_KEY_F16, XK_F16},
		{LEPK_KEY_F17, XK_F17},
		{LEPK_KEY_F18, XK_F18},
		{LEPK_KEY_F19, XK_F19},
		{LEPK_KEY_F20, XK_F20},
		{LEPK_KEY_F21, XK_F21},
		{LEPK_KEY_F22, XK_F22},
		{LEPK_KEY_F23, XK_F23},
		{LEPK_KEY_F24, XK_F24},

		/* Mod keys. */
		{LEPK_KEY_SHIFT_L, XK_Shift_L},
		{LEPK_KEY_SHIFT_R, XK_Shift_R},
		{LEPK_KEY_CTRL_L,  XK_Control_L},
		{LEPK_KEY_CTRL_R,  XK_Control_R},
		{LEPK_KEY_ALT_L,   XK_Alt_L},
		{LEPK_KEY_ALT_R,   XK_Alt_R},
		{LEPK_KEY_SUPER_L, XK_Super_L},
		{LEPK_KEY_SUPER_R, XK_Super_R},

		{LEPK_KEY_BACKSPACE, XK_BackSpace},
		{LEPK_KEY_ENTER,     XK_Return},
		{LEPK_KEY_TAB,       XK_Tab},
		{LEPK_KEY_SPACE,     XK_space},
		{LEPK_KEY_ESCAPE,    XK_Escape},

		/* Arrows. */
		{LEPK_KEY_LEFT,  XK_Left},
		{LEPK_KEY_DOWN,  XK_Down},
		{LEPK_KEY_UP,    XK_Up},
		{LEPK_KEY_RIGHT, XK_Right},

		{LEPK_KEY_PERIOD,     XK_period},
		{LEPK_KEY_COMMA,      XK_comma},
		{LEPK_KEY_MINUS,      XK_minus},
		{LEPK_KEY_PLUS,       XK_plus},
		{LEPK_KEY_APOSTROPHE, XK_apostrophe},
		{LEPK_KEY_SECTION,    XK_section},
		{LEPK_KEY_LESS,       XK_less},
		{LEPK_KEY_GREATER,    XK_greater},
		{LEPK_KEY_BRACKET_L,  XK_bracketleft},
		{LEPK_KEY_BRACKET_R,  XK_bracketright},
	};

	for (int scancode = 0; scancode < 256; scancode++) {
		KeySym sym = XkbKeycodeToKeysym(window->display, scancode, 0, 0);
		for (int i = 0; i < (int) (sizeof(keymap) / sizeof(keymap[0])); i++) {
			if ((const long) sym == keymap[i].x_key) {
				window->key_table[scancode] = keymap[i].key;
				/* Store with key index for easy translation. */
				window->scan_table[keymap[i].key] = scancode;
				break;
			}
			window->key_table[scancode] = LEPK_KEY_NULL;
		}
	}

	return window;
}

LEPKWINDOW void lepk_window_destroy(LepkWindow *window) {
	Lepk__LinuxWindow *_window = window;

	XCloseDisplay(_window->display);

	free(_window);
}

LEPKWINDOW bool lepk_window_is_open(const LepkWindow *window) {
	return ((Lepk__LinuxWindow *) window)->is_open;
}

LEPKWINDOW void lepk_window_poll_events(LepkWindow *window) {
	Lepk__LinuxWindow *_window = window;
	XEvent ev = {0};
	while (XPending(_window->display) > 0) {
		XNextEvent(_window->display, &ev);
		switch (ev.type) {
			case DestroyNotify: {
				XDestroyWindowEvent *e = (XDestroyWindowEvent *) &ev;
				if (e->window == _window->window) {
					_window->is_open = false;
				}
			} break;
			case ClientMessage: {
				XClientMessageEvent *e = (XClientMessageEvent *) &ev;
				if ((Atom) e->data.l[0] == _window->wm_delete_window) {
					XDestroyWindow(_window->display, _window->window);
					_window->is_open = false;
				}
			} break;
			case ConfigureNotify: {
			    XConfigureEvent *e = (XConfigureEvent *) &ev;
			    if (_window->resize_callback) {
					if (_window->width != e->width || _window->height != e->height) {
						_window->width  = e->width;
						_window->height = e->height;
						_window->resize_callback(window, e->width, e->height);
					}
			    }
			} break;

			/* Keyboard input. */
			case KeyPress:
			case KeyRelease: {
				XKeyEvent *e  = (XKeyEvent *) &ev;
				if (_window->key_callback) {
					_window->key_callback(window, _window->key_table[e->keycode], e->keycode, e->type == KeyPress, e->state);
				}
			} break;

			/* Window focus. */
			case FocusIn:
			case FocusOut: {
				/* Only disable key repeats when window is in focuse because X disables it system wide for some reason. */
				XFocusChangeEvent *e = (XFocusChangeEvent *) &ev;
				if (e->type == FocusIn) {
					XAutoRepeatOff(_window->display);
				} else {
					XAutoRepeatOn(_window->display);
				}
			}
		}
	}
}

LEPKWINDOW void lepk_window_callback_resize(LepkWindow *window, LepkResizeCallback callback) { ((Lepk__LinuxWindow *) window)->resize_callback = callback; }
LEPKWINDOW void lepk_window_callback_key(LepkWindow *window, LepkKeyCallback callback)       { ((Lepk__LinuxWindow *) window)->key_callback    = callback; }
#endif /* LEPK_WINDOW_OS_LINUX */

/* Windows */
#ifdef LEPK_WINDOW_OS_WINDOWS
#error "Windows is currently unsupported."
#endif /* LEPK_WINDOW_OS_WINDOWS */

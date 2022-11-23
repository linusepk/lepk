#ifndef LEPK_WINDOW_H
#define LEPK_WINDOW_H

#ifndef LEPK_WINDOW_STATIC
#define LEPKWINDOW extern
#else /* LEPK_WINDOW_STATIC */
#define LEPKWINDOW static
#endif /* LEPK_WINDOW_STATIC */

#include <stdbool.h>

/* Use void as type to hinder access to window variables. */
typedef void LepkWindow;
/* Resize callback. */
typedef void (* LepkResizeCallback)(LepkWindow *window, int width, int height);

LEPKWINDOW LepkWindow *lepk_window_create(int width, int height, const char *title, bool resizable);
LEPKWINDOW void lepk_window_destroy(LepkWindow *window);
LEPKWINDOW bool lepk_window_is_open(const LepkWindow *window);
LEPKWINDOW void lepk_window_poll_events(LepkWindow *window);

/* Set resize callback for window. */
LEPKWINDOW void lepk_window_callback_resize(LepkWindow *window, LepkResizeCallback callback);

#ifdef LEPK_WINDOW_IMPLEMENTATION
#undef LEPKWINDOW
#ifdef LEPK_WINDOW_STATIC
#define LEPKWINDOW static
#else /* LEPK_WINDOW_STATIC */
#define LEPKWINDOW
#endif /* LEPK_WINDOW_STATIC */

/* Linux. */
#ifdef LEPK_WINDOW_OS_LINUX
#include <malloc.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

typedef struct {
	Display *display;
	Window window;
	bool is_open;
	Atom wm_delete_window;
	int width;
	int height;

	/* Callbacks. */
	LepkResizeCallback resize_callback;
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
				bool press = (e->type == KeyPress);

				printf("%-7s    %3d    %3d\n", press ? "Press" : "Release", e->keycode, e->state);
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

LEPKWINDOW void lepk_window_callback_resize(LepkWindow *window, LepkResizeCallback callback) {
	((Lepk__LinuxWindow *) window)->resize_callback = callback;
}
#endif /* LEPK_WINDOW_OS_LINUX */

/* Windows */
#ifdef LEPK_WINDOW_OS_WINDOWS
#error "Windows is currently unsupported."
#endif /* LEPK_WINDOW_OS_WINDOWS */
#endif /*LEPK_WINDOW_IMPLEMENTATION*/
#endif /* LEPK_WINDOW_H */

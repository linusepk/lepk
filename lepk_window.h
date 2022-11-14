/* Version: 1.2 */

/*
 * MIT License
 * 
 * Copyright (c) 2022 Linus Erik Pontus Kåreblom
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * Windowing, single header library.
 * Currently supporting windows and linux, with xcb.
 *
 * Add:
 *     #define LEPK_WINDOW_IMPLEMENTATION
 * in one C or C++ file, before including this file, to create the implementation.
 * 
 * For vulkan support add:
 *     #define LEPK_WINDOW_VULKAN
 * before including this file. Then the function 'lepk_window_get_surface' will be available.
 *
 * When compiling, link to 'X11', 'X11-xcb' and 'xcb' if your compiling for linux.
 */

/*
 * === Documentation ===
 * Usage:
 * LepkWindow *window = lepk_window_create(800, 600, "Window", true);
 * lepk_window_set_key_callback(window, callback);
 *  
 * while (lepk_window_is_open(window)) {
 *     // Do graphics stuff.
 *     lepk_window_poll_events(window);
 * }
 *
 * lepk_window_destroy(window);
 */

/*
 * TODO:
 * 1. Add keyboard input handling.
 * 2. Add mouse input handling.
 * 3. Add resize callback.
 */

#ifndef LEPK_WINDOW_H
#define LEPK_WINDOW_H

#include <stdbool.h>

#ifdef LEPK_WINDOW_STATIC
#define LEPKWINDOW static
#else /* LEPK_WINDOW_STATIC */
#define LEPKWINDOW extern
#endif /* LEPK_WINDOW_STATIC */

typedef enum {
	LEPK_KEY_NULL = -1,
	LEPK_KEY_BACKSPACE,
	LEPK_KEY_TAB,
	LEPK_KEY_ENTER,
	LEPK_KEY_ESCAPE,
	LEPK_KEY_DELETE,

	/* Arrows. */
	LEPK_KEY_LEFT,
	LEPK_KEY_UP,
	LEPK_KEY_RIGHT,
	LEPK_KEY_DOWN,

	/* Keypad. */
	LEPK_KEY_NUMLOCK,
	LEPK_KEY_PAD_SPACE,
	LEPK_KEY_PAD_TAB,
	LEPK_KEY_PAD_ENTER,
	LEPK_KEY_PAD_F1,
	LEPK_KEY_PAD_F2,
	LEPK_KEY_PAD_F3,
	LEPK_KEY_PAD_F4,
	LEPK_KEY_PAD_EQUAL,
	LEPK_KEY_PAD_ASTERISK,
	LEPK_KEY_PAD_PLUS,
	LEPK_KEY_PAD_COMMA,
	LEPK_KEY_PAD_MINUS,
	LEPK_KEY_PAD_PERIOD,
	LEPK_KEY_PAD_SLASH,
	LEPK_KEY_PAD_0,
	LEPK_KEY_PAD_1,
	LEPK_KEY_PAD_2,
	LEPK_KEY_PAD_3,
	LEPK_KEY_PAD_4,
	LEPK_KEY_PAD_5,
	LEPK_KEY_PAD_6,
	LEPK_KEY_PAD_7,
	LEPK_KEY_PAD_8,
	LEPK_KEY_PAD_9,

	/* F keys. */
	LEPK_KEY_F1,
	LEPK_KEY_F2,
	LEPK_KEY_F3,
	LEPK_KEY_F4,
	LEPK_KEY_F5,
	LEPK_KEY_F6,
	LEPK_KEY_F7,
	LEPK_KEY_F8,
	LEPK_KEY_F9,
	LEPK_KEY_F10,
	LEPK_KEY_F11,
	LEPK_KEY_F12,
	LEPK_KEY_F13,
	LEPK_KEY_F14,
	LEPK_KEY_F15,
	LEPK_KEY_F16,
	LEPK_KEY_F17,
	LEPK_KEY_F18,
	LEPK_KEY_F19,
	LEPK_KEY_F20,
	LEPK_KEY_F21,
	LEPK_KEY_F22,
	LEPK_KEY_F23,
	LEPK_KEY_F24,

	LEPK_KEY_SHIFT_L,
	LEPK_KEY_SHIFT_R,
	LEPK_KEY_CTRL_L,
	LEPK_KEY_CTRL_R,
	LEPK_KEY_ALT_L,
	LEPK_KEY_ALT_R,
	LEPK_KEY_SUPER_L,
	LEPK_KEY_SUPER_R,

	LEPK_KEY_CAPSLOCK,
	LEPK_KEY_SHIFTLOCK,

	LEPK_KEY_SPACE,
	LEPK_KEY_EXCLAMATION_MARK, /* ! */
	LEPK_KEY_DOUBLE_QUOTE,	   /* " */
	LEPK_KEY_HASH,             /* # */
	LEPK_KEY_DOLLAR,           /* $ */
	LPEK_KEY_PERCENT,          /* % */
	LEPK_KEY_AMPERSAND,        /* & */
	LPEK_KEY_APOSTROPHE,       /* ' */
	LEPK_KEY_PARENTHESIS_L,    /* ( */
	LEPK_KEY_PARENTHESIS_R,    /* ) */
	LEPK_KEY_ASTERISK,         /* * */
	LEPK_KEY_PLUS,             /* + */
	LEPK_KEY_COMMA,            /* , */
	LEPK_KEY_MINUS,            /* - */
	LEPK_KEY_PERIOD,           /* . */
	LEPK_KEY_SLASH,            /* / */
	LEPK_KEY_COLON,            /* : */
	LEPK_KEY_SEMICOLON,        /* ; */
	LEPK_KEY_LESS,             /* < */
	LEPK_KEY_EQUAL,            /* = */
	LEPK_KEY_GREATER,          /* > */
	LEPK_KEY_QUESTION_MARK,    /* ? */
	LEPK_KEY_AT,               /* @ */
	LEPK_KEY_BRACKET_L,        /* [ */
	LEPK_KEY_BRACKET_R,        /* ] */
	LEPK_KEY_BACKSLASH,        /* \ */
	LEPK_KEY_UNDERSCORE,       /* _ */
	LEPK_KEY_GRAVE,            /* ` */
	LEPK_KEY_BRACE_L,          /* { */
	LEPK_KEY_BRACE_R,          /* } */
	LEPK_KEY_BAR,              /* | */
	LEPK_KEY_TILDE,            /* ~ */

	LEPK_KEY_0, /* 0 */
	LEPK_KEY_1, /* 1 */
	LEPK_KEY_2, /* 2 */
	LEPK_KEY_3, /* 3 */
	LEPK_KEY_4, /* 4 */
	LEPK_KEY_5, /* 5 */
	LEPK_KEY_6, /* 6 */
	LEPK_KEY_7, /* 7 */
	LEPK_KEY_8, /* 8 */
	LEPK_KEY_9, /* 9 */

	LEPK_KEY_A, /* a */
	LEPK_KEY_B, /* b */
	LEPK_KEY_C, /* c */
	LEPK_KEY_D, /* d */
	LEPK_KEY_E, /* e */
	LEPK_KEY_F, /* f */
	LEPK_KEY_G, /* g */
	LEPK_KEY_H, /* h */
	LEPK_KEY_I, /* i */
	LEPK_KEY_J, /* j */
	LEPK_KEY_K, /* k */
	LEPK_KEY_L, /* l */
	LEPK_KEY_M, /* m */
	LEPK_KEY_N, /* n */
	LEPK_KEY_O, /* o */
	LEPK_KEY_P, /* p */
	LEPK_KEY_Q, /* q */
	LEPK_KEY_R, /* r */
	LEPK_KEY_S, /* s */
	LEPK_KEY_T, /* t */
	LEPK_KEY_U, /* u */
	LEPK_KEY_V, /* v */
	LEPK_KEY_W, /* w */
	LEPK_KEY_X, /* x */
	LEPK_KEY_Y, /* y */
	LEPK_KEY_Z, /* z */

	LEPK_KEY_COUNT,
} LepkKey;

typedef enum {
	LEPK_KEY_MOD_SHIFT    = 1 << 0,
	LEPK_KEY_MOD_CAPSLOCK = 1 << 1,
	LEPK_KEY_MOD_CTRL     = 1 << 2,
	LEPK_KEY_MOD_ALT      = 1 << 3,
	LEPK_KEY_MOD_SUPER    = 1 << 6,
	LEPK_KEY_MOD_ALTGR    = 1 << 7,

	LEPK_KEY_MOD_COUNT,
} LepkKeyMod;

typedef void (*LepkKeyCallback)(LepkKey key, LepkKeyMod mod, bool pressed);

/*
 * Window object. Definition is based on OS.
 */
typedef struct LepkWindow LepkWindow;

/* Create window. */
LEPKWINDOW LepkWindow *lepk_window_create(int width, int height, const char *title, bool resizable);
/* Free window. */
LEPKWINDOW void lepk_window_destroy(LepkWindow *window);
/* Check if window is still open or a close event has happened. */
LEPKWINDOW bool lepk_window_is_open(const LepkWindow *window);
/* Poll window events. */
LEPKWINDOW void lepk_window_poll_events(LepkWindow *window);
/* Set key event callback for window. */
LEPKWINDOW void lepk_window_set_key_callback(LepkWindow *window, LepkKeyCallback callback);

#ifdef LEPK_WINDOW_VULKAN
#ifdef __linux__
#define VK_USE_PLATFORM_XCB_KHR
#endif /* __linux__ */
#ifdef _WIN32
#define VK_UES_PLATFORM_WIN32_KHR
#endif /* _WIN32 */
#include <vulkan/vulkan.h>
/* Get vulkan surface for window. */
LEPKWINDOW VkSurfaceKHR lepk_window_get_surface(const LepkWindow *window, VkInstance instance);
#endif /* LEPK_WINDOW_VULKAN */

#ifdef LEPK_WINDOW_IMPLEMENTATION
/*
 * Linux
 */
#ifdef __linux__

#include <X11/XKBlib.h>
#include <X11/Xlib-xcb.h>
#include <X11/keysym.h>
#include <malloc.h>
#include <stddef.h>
#include <string.h>

struct LepkWindow {
	Display *display;
	xcb_connection_t *connection;
	xcb_window_t window;
	xcb_screen_t *screen;
	xcb_atom_t wm_protocols;
	xcb_atom_t wm_delete_win;
	bool is_open;
	LepkKeyCallback key_callback;
};

static LepkKey lepk__translate_keysym(KeySym sym) {
	switch (sym) {
		case XK_BackSpace:
			return LEPK_KEY_BACKSPACE;
		case XK_Return:
			return LEPK_KEY_ENTER;
		case XK_Tab:
			return LEPK_KEY_TAB;

		case XK_Escape:
			return LEPK_KEY_ESCAPE;
		case XK_Delete:
			return LEPK_KEY_DELETE;

		/* Arrows. */
		case XK_Left:
			return LEPK_KEY_LEFT;
		case XK_Up:
			return LEPK_KEY_UP;
		case XK_Right:
			return LEPK_KEY_RIGHT;
		case XK_Down:
			return LEPK_KEY_DOWN;

		/* Keypad. */
		case XK_Num_Lock:
			return LEPK_KEY_NUMLOCK;
		case XK_KP_Space:
			return LEPK_KEY_PAD_SPACE;
		case XK_KP_Tab:
			return LEPK_KEY_PAD_TAB;
		case XK_KP_Enter:
			return LEPK_KEY_PAD_ENTER;
		case XK_KP_F1:
			return LEPK_KEY_PAD_F1;
		case XK_KP_F2:
			return LEPK_KEY_PAD_F2;
		case XK_KP_F3:
			return LEPK_KEY_PAD_F3;
		case XK_KP_F4:
			return LEPK_KEY_PAD_F4;
		case XK_KP_Equal:
			return LEPK_KEY_PAD_EQUAL;
		case XK_KP_Multiply:
			return LEPK_KEY_PAD_ASTERISK;
		case XK_KP_Add:
			return LEPK_KEY_PAD_PLUS;
		case XK_KP_Separator:
			return LEPK_KEY_PAD_COMMA;
		case XK_KP_Decimal:
			return LEPK_KEY_PAD_PERIOD;
		case XK_KP_Divide:
			return LEPK_KEY_PAD_SLASH;
		case XK_KP_0:
			return LEPK_KEY_PAD_0;
		case XK_KP_1:
			return LEPK_KEY_PAD_1;
		case XK_KP_2:
			return LEPK_KEY_PAD_2;
		case XK_KP_3:
			return LEPK_KEY_PAD_3;
		case XK_KP_4:
			return LEPK_KEY_PAD_4;
		case XK_KP_5:
			return LEPK_KEY_PAD_5;
		case XK_KP_6:
			return LEPK_KEY_PAD_6;
		case XK_KP_7:
			return LEPK_KEY_PAD_7;
		case XK_KP_8:
			return LEPK_KEY_PAD_8;
		case XK_KP_9:
			return LEPK_KEY_PAD_9;

		/* F keys. */
		case XK_F1:
			return LEPK_KEY_F1;
		case XK_F2:
			return LEPK_KEY_F2;
		case XK_F3:
			return LEPK_KEY_F3;
		case XK_F4:
			return LEPK_KEY_F4;
		case XK_F5:
			return LEPK_KEY_F5;
		case XK_F6:
			return LEPK_KEY_F6;
		case XK_F7:
			return LEPK_KEY_F7;
		case XK_F8:
			return LEPK_KEY_F8;
		case XK_F9:
			return LEPK_KEY_F9;
		case XK_F10:
			return LEPK_KEY_F10;
		case XK_F11:
			return LEPK_KEY_F11;
		case XK_F12:
			return LEPK_KEY_F12;
		case XK_F13:
			return LEPK_KEY_F13;
		case XK_F14:
			return LEPK_KEY_F14;
		case XK_F15:
			return LEPK_KEY_F15;
		case XK_F16:
			return LEPK_KEY_F16;
		case XK_F17:
			return LEPK_KEY_F17;
		case XK_F18:
			return LEPK_KEY_F18;
		case XK_F19:
			return LEPK_KEY_F19;
		case XK_F20:
			return LEPK_KEY_F20;
		case XK_F21:
			return LEPK_KEY_F21;
		case XK_F22:
			return LEPK_KEY_F22;
		case XK_F23:
			return LEPK_KEY_F23;
		case XK_F24:
			return LEPK_KEY_F24;

		case XK_Shift_L:
			return LEPK_KEY_SHIFT_L;
		case XK_Shift_R:
			return LEPK_KEY_SHIFT_R;
		case XK_Control_L:
			return LEPK_KEY_CTRL_L;
		case XK_Control_R:
			return LEPK_KEY_CTRL_R;
		case XK_Alt_L:
			return LEPK_KEY_ALT_L;
		case XK_Alt_R:
			return LEPK_KEY_ALT_R;
		case XK_Super_L:
			return LEPK_KEY_SUPER_L;
		case XK_Super_R:
			return LEPK_KEY_SUPER_R;

		case XK_Caps_Lock:
			return LEPK_KEY_CAPSLOCK;
		case XK_Shift_Lock:
			return LEPK_KEY_SHIFTLOCK;

		case XK_exclam:
			return LEPK_KEY_EXCLAMATION_MARK;
		case XK_quotedbl:
			return LEPK_KEY_DOUBLE_QUOTE;
		case XK_numbersign:
			return LEPK_KEY_DOLLAR;
		case XK_percent:
			return LEPK_KEY_DOLLAR;
		case XK_ampersand:
			return LEPK_KEY_AMPERSAND;
		case XK_quoteright:
			return LPEK_KEY_APOSTROPHE;
		case XK_parenleft:
			return LEPK_KEY_PARENTHESIS_L;
		case XK_parenright:
			return LEPK_KEY_PARENTHESIS_R;
		case XK_asterisk:
			return LEPK_KEY_ASTERISK;
		case XK_plus:
			return LEPK_KEY_PLUS;
		case XK_comma:
			return LEPK_KEY_COMMA;
		case XK_minus:
			return LEPK_KEY_MINUS;
		case XK_period:
			return LEPK_KEY_PERIOD;
		case XK_slash:
			return LEPK_KEY_SLASH;
		case XK_colon:
			return LEPK_KEY_COLON;
		case XK_semicolon:
			return LEPK_KEY_SEMICOLON;
		case XK_less:
			return LEPK_KEY_LESS;
		case XK_equal:
			return LEPK_KEY_EQUAL;
		case XK_greater:
			return LEPK_KEY_GREATER;
		case XK_question:
			return LEPK_KEY_QUESTION_MARK;
		case XK_at:
			return LEPK_KEY_AT;
		case XK_bracketleft:
			return LEPK_KEY_BRACKET_L;
		case XK_bracketright:
			return LEPK_KEY_BRACKET_R;
		case XK_backslash:
			return LEPK_KEY_BACKSLASH;
		case XK_underscore:
			return LEPK_KEY_UNDERSCORE;
		case XK_quoteleft:
			return LEPK_KEY_GRAVE;
		case XK_braceleft:
			return LEPK_KEY_BRACE_L;
		case XK_braceright:
			return LEPK_KEY_BRACE_R;
		case XK_bar:
			return LEPK_KEY_BAR;
		case XK_asciitilde:
			return LEPK_KEY_TILDE;

		case XK_space:
			return LEPK_KEY_SPACE;

		/* Numbers. */
		case XK_0:
			return LEPK_KEY_0;
		case XK_1:
			return LEPK_KEY_1;
		case XK_2:
			return LEPK_KEY_2;
		case XK_3:
			return LEPK_KEY_3;
		case XK_4:
			return LEPK_KEY_4;
		case XK_5:
			return LEPK_KEY_5;
		case XK_6:
			return LEPK_KEY_6;
		case XK_7:
			return LEPK_KEY_7;
		case XK_8:
			return LEPK_KEY_8;
		case XK_9:
			return LEPK_KEY_9;

		/* Letters. */
		case XK_a:
		case XK_A:
			return LEPK_KEY_A;
		case XK_b:
		case XK_B:
			return LEPK_KEY_B;
		case XK_c:
		case XK_C:
			return LEPK_KEY_C;
		case XK_d:
		case XK_D:
			return LEPK_KEY_D;
		case XK_e:
		case XK_E:
			return LEPK_KEY_E;
		case XK_f:
		case XK_F:
			return LEPK_KEY_F;
		case XK_g:
		case XK_G:
			return LEPK_KEY_G;
		case XK_h:
		case XK_H:
			return LEPK_KEY_H;
		case XK_i:
		case XK_I:
			return LEPK_KEY_I;
		case XK_j:
		case XK_J:
			return LEPK_KEY_J;
		case XK_k:
		case XK_K:
			return LEPK_KEY_K;
		case XK_l:
		case XK_L:
			return LEPK_KEY_L;
		case XK_m:
		case XK_M:
			return LEPK_KEY_M;
		case XK_n:
		case XK_N:
			return LEPK_KEY_N;
		case XK_o:
		case XK_O:
			return LEPK_KEY_O;
		case XK_p:
		case XK_P:
			return LEPK_KEY_P;
		case XK_q:
		case XK_Q:
			return LEPK_KEY_Q;
		case XK_r:
		case XK_R:
			return LEPK_KEY_R;
		case XK_s:
		case XK_S:
			return LEPK_KEY_S;
		case XK_t:
		case XK_T:
			return LEPK_KEY_T;
		case XK_u:
		case XK_U:
			return LEPK_KEY_U;
		case XK_v:
		case XK_V:
			return LEPK_KEY_V;
		case XK_w:
		case XK_W:
			return LEPK_KEY_W;
		case XK_x:
		case XK_X:
			return LEPK_KEY_X;
		case XK_y:
		case XK_Y:
			return LEPK_KEY_Y;
		case XK_z:
		case XK_Z:
			return LEPK_KEY_Z;
	}

	return LEPK_KEY_NULL;
}

LepkWindow *lepk_window_create(int width, int height, const char *title, bool resizable) {
	LepkWindow *window = malloc(sizeof(LepkWindow));
	window->is_open = 1;
	window->key_callback = NULL;

	window->display = XOpenDisplay(NULL);
	window->connection = XGetXCBConnection(window->display);
	if (xcb_connection_has_error(window->connection)) {
		return NULL;
	}

	XAutoRepeatOff(window->display);

	xcb_screen_iterator_t iter = xcb_setup_roots_iterator(xcb_get_setup(window->connection));
	window->screen = iter.data;

	/* Create window. */
	window->window = xcb_generate_id(window->connection);
	int mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	int events = 0;
	/* Keyboard events. */
	events |= XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE;
	int value_list[] = { window->screen->black_pixel, events };

	xcb_create_window(
		window->connection,
		XCB_COPY_FROM_PARENT,
		window->window,
		window->screen->root,
		0, 0, /* Position */
		width, height,
		0,
		XCB_WINDOW_CLASS_INPUT_OUTPUT,
		window->screen->root_visual,
		mask,
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

	/* Send window to the X server. */
	xcb_map_window(window->connection, window->window);
	xcb_flush(window->connection);

	return window;
}

void lepk_window_destroy(LepkWindow *window) {
	XAutoRepeatOn(window->display);
	xcb_destroy_window(window->connection, window->window);
	free(window);
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
			case XCB_KEY_RELEASE:
			case XCB_KEY_PRESS:
				if (window->key_callback != NULL) {
					xcb_key_press_event_t *kp = (xcb_key_press_event_t *) event;
					KeySym key = XkbKeycodeToKeysym(window->display, (KeyCode) kp->detail, 0, 0);
					window->key_callback(lepk__translate_keysym(key), kp->state, event->response_type == XCB_KEY_PRESS);
				}
				break;
			default:
				break;
		}
		free(event);
	}
}

void lepk_window_set_key_callback(LepkWindow *window, LepkKeyCallback callback) {
	window->key_callback = callback;
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

#include <Windows.h>

struct LepkWindow {
	HINSTANCE instance;
	HWND window;
	bool is_open;
	LepkKeyCallback key_callback;
};

static LRESULT CALLBACK lepk__process_message(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
	switch (msg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		default:
			break;
	}

	return DefWindowProcA(hwnd, msg, w_param, l_param);
}

LepkWindow *lepk_window_create(int width, int height, const char *title, bool resizable) {
	LepkWindow *window = malloc(sizeof(LepkWindow));
	window->is_open = true;

	/* Get instance. */
	window->instance = GetModuleHandleA(0);

	const char *class_name = "lepk_window_class";
	WNDCLASSA wc = {0};
	wc.lpfnWndProc = lepk__process_message;
	wc.hInstance = window->instance;
	wx.lpszClassName = class_name;
	wc.hCuesor = LoadCursor(NULL, IDC_ARROW);

	if (!RegisterClassA(&wc)) {
		return NULL;
	}

	DWORD style = WS_OVERLAPPED | WS_SYS_MENU | WS_CAPTION | WS_MINIMIZEBOX;
	if (resizable) {
		style |= WS_MAXIMIZEBOX;
		style |= WS_THICKFRAME;
	}

	/* Correct window sizing. */
	RECT border_rect = {0};
	AdjustWindowRectEx(&border_rect, style, 0, 0);
	int border_margin_x = border_rect.right - border_rect.left;
	int border_margin_y = border_rect.bottom - border_rect.top;

	/* Create window. */
	window->window = CreateWindowExA(
		0,
		class_name,
		title,
		style,
		CW_USEDEFAULT, CW_USEDEFAULT, /* Position */
		width + border_margin_x, height + border_margin_y,
		0,
		0,
		window->instance,
		0
	);
	if (window->window == NULL) {
		return NULL;
	}

	/* Present window. */
	ShowWindow(window->window, SW_SHOW);

	return window;
}

void lepk_window_destroy(LepkWindow *window) {
	DestroyWindow(window->window);
	free(window);
}

bool lepk_window_is_open(const LepkWindow *window) {
	return window->is_open;
}

void lepk_window_poll_events(LepkWindow *window) {
	MSG msg = {0};
	while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			window->closed = true;
		} else {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}
}

void lepk_window_set_key_callback(LepkWindow *window, LepkKeyCallback callback) {
	window->key_callback = callback;
}

#ifdef LEPK_WINDOW_VULKAN
VkSurfaceKHR lepk_window_get_surface(const LepkWindow *window, VkInstance instance) {
	VkSurfaceKHR surface;

	VkWin32SurfaceCreateInfoKHR surface_create_info = {0};
	surface_create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surface_create_info.pNext = NULL;
	surface_create_info.flags = 0;
	surface_create_info.hinstance = window->instance;
	surface_create_info.hwnd = window->window;

	if (vkCreateWin32SurfaceKHR(instance, &surface_create_info, NULL, &surface) != VK_SUCCESS) {
		return NULL;
	}

	return surface;
}
#endif /* LEPK_WINDOW_VULKAN */

#endif /* _WIN32 */

#endif /* LEPK_WINDOW_IMPLEMENTATION */
#endif /* LEPK_WINDOW_H */

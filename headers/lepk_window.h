#ifndef LEPK_WINDOW_H
#define LEPK_WINDOW_H

#ifndef LEPK_WINDOW_STATIC
#define LEPKWINDOW extern
#else /* LEPK_WINDOW_STATIC */
#define LEPKWINDOW static
#endif /* LEPK_WINDOW_STATIC */

#include <stdbool.h>

typedef enum {
	LEPK_KEY_NULL = -1,

	/* Letters. */
	LEPK_KEY_A,
	LEPK_KEY_B,
	LEPK_KEY_C,
	LEPK_KEY_D,
	LEPK_KEY_E,
	LEPK_KEY_F,
	LEPK_KEY_G,
	LEPK_KEY_H,
	LEPK_KEY_I,
	LEPK_KEY_J,
	LEPK_KEY_K,
	LEPK_KEY_L,
	LEPK_KEY_M,
	LEPK_KEY_N,
	LEPK_KEY_O,
	LEPK_KEY_P,
	LEPK_KEY_Q,
	LEPK_KEY_R,
	LEPK_KEY_S,
	LEPK_KEY_T,
	LEPK_KEY_U,
	LEPK_KEY_V,
	LEPK_KEY_W,
	LEPK_KEY_X,
	LEPK_KEY_Y,
	LEPK_KEY_Z,

	/* Numbers. */
	LEPK_KEY_0,
	LEPK_KEY_1,
	LEPK_KEY_2,
	LEPK_KEY_3,
	LEPK_KEY_4,
	LEPK_KEY_5,
	LEPK_KEY_6,
	LEPK_KEY_7,
	LEPK_KEY_8,
	LEPK_KEY_9,

	/* Function keys. */
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

	/* Mod keys. */
	LEPK_KEY_SHIFT_L,
	LEPK_KEY_SHIFT_R,
	LEPK_KEY_CTRL_L,
	LEPK_KEY_CTRL_R,
	LEPK_KEY_ALT_L,
	LEPK_KEY_ALT_R, /* Not alt gr. */
	LEPK_KEY_SUPER_L,
	LEPK_KEY_SUPER_R,

	LEPK_KEY_BACKSPACE,
	LEPK_KEY_ENTER,
	LEPK_KEY_TAB,
	LEPK_KEY_SPACE,
	LEPK_KEY_ESCAPE,

	/* Arrows. */
	LEPK_KEY_LEFT,
	LEPK_KEY_DOWN,
	LEPK_KEY_UP,
	LEPK_KEY_RIGHT,

	LEPK_KEY_PERIOD,     /* . */
	LEPK_KEY_COMMA,      /* , */
	LEPK_KEY_MINUS,      /* - */
	LEPK_KEY_PLUS,       /* + */
	LEPK_KEY_APOSTROPHE, /* ' */
	LEPK_KEY_SECTION,    /* § */
	LEPK_KEY_LESS,       /* < */
	LEPK_KEY_GREATER,    /* > */
	LEPK_KEY_BRACKET_L,  /* [ */
	LEPK_KEY_BRACKET_R,  /* ] */

	LEPK_KEY_COUNT,
} LepkKey;

typedef enum {
	LEPK_MOD_SHIFT     = 1 << 0,
	LEPK_MOD_CAPS_LOCK = 1 << 1,
	LEPK_MOD_CTRL      = 1 << 2,
	LEPK_MOD_ALT       = 1 << 3,
	LEPK_MOD_NUM_LOCK  = 1 << 4,
	LEPK_MOD_SUPER     = 1 << 6,
} LepkMod;

/* Use void as type to hinder access to window variables. */
typedef void LepkWindow;
/* Resize callback. */
typedef void (*LepkResizeCallback)(LepkWindow *window, int width, int height);
typedef void (*LepkKeyCallback)(LepkWindow *window, LepkKey keycode, int scancode, bool pressed, LepkMod mods);

LEPKWINDOW LepkWindow *lepk_window_create(int width, int height, const char *title, bool resizable);
LEPKWINDOW void lepk_window_destroy(LepkWindow *window);
LEPKWINDOW bool lepk_window_is_open(const LepkWindow *window);
LEPKWINDOW void lepk_window_poll_events(LepkWindow *window);

/* Set resize callback for window. */
LEPKWINDOW void lepk_window_callback_resize(LepkWindow *window, LepkResizeCallback callback);
LEPKWINDOW void lepk_window_callback_key(LepkWindow *window, LepkKeyCallback callback);

#endif /* LEPK_WINDOW_H */

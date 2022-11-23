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

#endif /* LEPK_WINDOW_H */

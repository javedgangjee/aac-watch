#pragma once

#include <stdbool.h>
#include "screen_cycle.h"

typedef void (*ui_tap_cb_t)(void);

/* Return true if the swipe opened the settings screen. A swipe that returns false acts as a tap. */
typedef bool (*ui_swipe_cb_t)(void);

/* Make the screen and its label. on_swipe_down runs for a swipe down that starts near the top
 * edge. Call with the display lock held. */
void ui_init(ui_tap_cb_t on_tap, ui_swipe_cb_t on_swipe_down);

/* Draw the given screen. Call with the display lock held. */
void ui_show(screen_t screen);

/* Load the yes and no screen again after the settings screen. Call with the display lock held. */
void ui_load(void);

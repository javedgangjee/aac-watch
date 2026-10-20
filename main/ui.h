#pragma once

#include "screen_cycle.h"

typedef void (*ui_tap_cb_t)(void);

/* Make the screen and its label. Call with the display lock held. */
void ui_init(ui_tap_cb_t on_tap);

/* Draw the given screen. Call with the display lock held. */
void ui_show(screen_t screen);

#pragma once

#include "settings.h"

/* on_close gets the settings as the screen left them. */
typedef void (*ui_settings_close_cb_t)(const settings_t *s);

/* Show the settings screen with a copy of current. on_close runs when the person taps Done or
 * swipes up from the bottom edge. Call with the display lock held. */
void ui_settings_open(const settings_t *current, ui_settings_close_cb_t on_close);

/* Close the settings screen as Done does. Call with the display lock held. */
void ui_settings_close(void);

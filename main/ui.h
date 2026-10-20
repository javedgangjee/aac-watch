#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "lvgl.h"
#include "screen_cycle.h"

/* A swipe that opens or closes the settings screen must start this close to the edge. */
#define UI_SWIPE_EDGE 60

typedef void (*ui_tap_cb_t)(void);

/* Return true if the swipe opened the settings screen. A swipe that returns false acts as a tap. */
typedef bool (*ui_swipe_cb_t)(void);

/* Make the screen, its label, and the hidden halves of the split screen. on_swipe_down runs for
 * a swipe down that starts near the top edge. Call with the display lock held. */
void ui_init(ui_tap_cb_t on_tap, ui_swipe_cb_t on_swipe_down);

/* Draw the given screen of the cycle mode. Call with the display lock held. */
void ui_show(screen_t screen);

/* Draw the split screen, with yes on the top half and no on the bottom half. Call with the
 * display lock held. */
void ui_show_split(void);

/* Load the yes and no screen again after the settings screen. Call with the display lock held. */
void ui_load(void);

/* Record where each press on obj starts. Call with the display lock held. */
void ui_track_press(lv_obj_t *obj);

/* Return the y position where the last press on a tracked object started. */
int32_t ui_press_start_y(void);

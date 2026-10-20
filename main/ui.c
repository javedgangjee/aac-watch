#include "lvgl.h"
#include "ui.h"

LV_FONT_DECLARE(font_yes_no_160);

#define UI_COLOR_YES 0x007A29
#define UI_COLOR_NO  0xC62828
/* A swipe down must start this close to the top edge to open the settings screen. */
#define UI_SWIPE_EDGE 60

static lv_obj_t *s_screen;
static lv_obj_t *s_label;
static ui_tap_cb_t s_on_tap;
static ui_swipe_cb_t s_on_swipe_down;
static int32_t s_press_y;

static void ui_tap_event_cb(lv_event_t *e)
{
    (void)e;
    if (s_on_tap) {
        s_on_tap();
    }
}

static void ui_press_event_cb(lv_event_t *e)
{
    (void)e;
    lv_point_t p;
    lv_indev_get_point(lv_indev_active(), &p);
    s_press_y = p.y;
}

static void ui_gesture_event_cb(lv_event_t *e)
{
    (void)e;
    lv_indev_t *indev = lv_indev_active();
    if (lv_indev_get_gesture_dir(indev) != LV_DIR_BOTTOM || s_press_y >= UI_SWIPE_EDGE) {
        return;
    }
    if (s_on_swipe_down && s_on_swipe_down()) {
        /* The release must not also count as a tap. */
        lv_indev_wait_release(indev);
    }
}

void ui_init(ui_tap_cb_t on_tap, ui_swipe_cb_t on_swipe_down)
{
    s_on_tap = on_tap;
    s_on_swipe_down = on_swipe_down;

    s_screen = lv_screen_active();
    lv_obj_remove_flag(s_screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(s_screen, LV_OPA_COVER, 0);
    lv_obj_add_event_cb(s_screen, ui_tap_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(s_screen, ui_press_event_cb, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(s_screen, ui_gesture_event_cb, LV_EVENT_GESTURE, NULL);

    s_label = lv_label_create(s_screen);
    lv_obj_set_style_text_font(s_label, &font_yes_no_160, 0);
    lv_obj_set_style_text_color(s_label, lv_color_white(), 0);
    lv_obj_center(s_label);
}

void ui_show(screen_t screen)
{
    if (screen == SCREEN_YES) {
        lv_obj_set_style_bg_color(s_screen, lv_color_hex(UI_COLOR_YES), 0);
        lv_label_set_text_static(s_label, "Yes");
    } else {
        lv_obj_set_style_bg_color(s_screen, lv_color_hex(UI_COLOR_NO), 0);
        lv_label_set_text_static(s_label, "No");
    }
}

void ui_load(void)
{
    lv_screen_load(s_screen);
}

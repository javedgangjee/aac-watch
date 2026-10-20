#include "lvgl.h"
#include "ui.h"

LV_FONT_DECLARE(font_yes_no_160);

#define UI_COLOR_YES 0x007A29
#define UI_COLOR_NO  0xC62828

static lv_obj_t *s_screen;
static lv_obj_t *s_label;
static ui_tap_cb_t s_on_tap;

static void ui_tap_event_cb(lv_event_t *e)
{
    (void)e;
    if (s_on_tap) {
        s_on_tap();
    }
}

void ui_init(ui_tap_cb_t on_tap)
{
    s_on_tap = on_tap;

    s_screen = lv_screen_active();
    lv_obj_remove_flag(s_screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(s_screen, LV_OPA_COVER, 0);
    lv_obj_add_event_cb(s_screen, ui_tap_event_cb, LV_EVENT_CLICKED, NULL);

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

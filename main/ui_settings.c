#include "lvgl.h"
#include "ui.h"
#include "ui_settings.h"

LV_FONT_DECLARE(font_settings_28);
LV_FONT_DECLARE(font_settings_48);

#define UI_MARGIN            20
#define UI_NAME_GAP          6
#define UI_BUTTON_H          64
#define UI_STEP_BUTTON_W     96
#define UI_RADIUS            16

#define UI_COLOR_BUTTON      0x333333
#define UI_COLOR_PRESSED     0x555555
#define UI_COLOR_LIT         0xFFFFFF
#define UI_COLOR_DIM_BUTTON  0x1A1A1A
#define UI_COLOR_DIM_TEXT    0x5A5A5A

/* A dimmed button still gets presses, so a swipe that starts on it is seen. Its tap does
 * nothing because the value is at its limit. */
#define UI_STATE_DIM         LV_STATE_USER_1

static lv_obj_t *s_screen;
static lv_obj_t *s_btn_samantha;
static lv_obj_t *s_btn_reed;
static lv_obj_t *s_btn_cycle_down;
static lv_obj_t *s_btn_cycle_up;
static lv_obj_t *s_btn_volume_down;
static lv_obj_t *s_btn_volume_up;
static lv_obj_t *s_cycle_value;
static lv_obj_t *s_volume_value;
static settings_t s_settings;
static ui_settings_close_cb_t s_on_close;

void ui_settings_close(void)
{
    if (s_on_close) {
        s_on_close(&s_settings);
    }
}

/* Show a button as dimmed when its value is at the limit. */
static void set_enabled(lv_obj_t *btn, bool enabled)
{
    if (enabled) {
        lv_obj_remove_state(btn, UI_STATE_DIM);
    } else {
        lv_obj_add_state(btn, UI_STATE_DIM);
    }
}

static void draw_values(void)
{
    if (s_settings.voice == SETTINGS_VOICE_REED) {
        lv_obj_remove_state(s_btn_samantha, LV_STATE_CHECKED);
        lv_obj_add_state(s_btn_reed, LV_STATE_CHECKED);
    } else {
        lv_obj_add_state(s_btn_samantha, LV_STATE_CHECKED);
        lv_obj_remove_state(s_btn_reed, LV_STATE_CHECKED);
    }

    lv_label_set_text_fmt(s_cycle_value, "%u s", (unsigned)s_settings.cycle_s);
    set_enabled(s_btn_cycle_down, s_settings.cycle_s > SETTINGS_CYCLE_MIN_S);
    set_enabled(s_btn_cycle_up, s_settings.cycle_s < SETTINGS_CYCLE_MAX_S);

    lv_label_set_text_fmt(s_volume_value, "%u", (unsigned)s_settings.volume);
    set_enabled(s_btn_volume_down, s_settings.volume > SETTINGS_VOLUME_MIN);
    set_enabled(s_btn_volume_up, s_settings.volume < SETTINGS_VOLUME_MAX);
}

static void gesture_event_cb(lv_event_t *e)
{
    (void)e;
    lv_indev_t *indev = lv_indev_active();
    if (lv_indev_get_gesture_dir(indev) != LV_DIR_TOP) {
        return;
    }
    if (ui_press_start_y() < lv_obj_get_height(s_screen) - UI_SWIPE_EDGE) {
        return;
    }
    /* The release must not also count as a tap on a button. */
    lv_indev_wait_release(indev);
    ui_settings_close();
}

static void voice_event_cb(lv_event_t *e)
{
    s_settings.voice = (settings_voice_t)(intptr_t)lv_event_get_user_data(e);
    draw_values();
}

static void cycle_event_cb(lv_event_t *e)
{
    if (settings_cycle_step(&s_settings, (int)(intptr_t)lv_event_get_user_data(e))) {
        draw_values();
    }
}

static void volume_event_cb(lv_event_t *e)
{
    if (settings_volume_step(&s_settings, (int)(intptr_t)lv_event_get_user_data(e))) {
        draw_values();
    }
}

static void done_event_cb(lv_event_t *e)
{
    (void)e;
    ui_settings_close();
}

/* Make a container with no look of its own that lets presses through to the screen. */
static lv_obj_t *make_box(lv_obj_t *parent, lv_flex_flow_t flow)
{
    lv_obj_t *box = lv_obj_create(parent);
    lv_obj_remove_style_all(box);
    lv_obj_remove_flag(box, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_size(box, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(box, flow);
    return box;
}

static lv_obj_t *make_button(lv_obj_t *parent, const char *text, const lv_font_t *font,
                             int32_t width, lv_event_cb_t cb, intptr_t arg)
{
    lv_obj_t *btn = lv_button_create(parent);
    lv_obj_remove_style_all(btn);
    lv_obj_set_size(btn, width, UI_BUTTON_H);
    lv_obj_set_style_radius(btn, UI_RADIUS, 0);
    lv_obj_set_style_bg_opa(btn, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(btn, lv_color_hex(UI_COLOR_BUTTON), 0);
    lv_obj_set_style_text_color(btn, lv_color_white(), 0);
    lv_obj_set_style_bg_color(btn, lv_color_hex(UI_COLOR_PRESSED), LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(btn, lv_color_hex(UI_COLOR_LIT), LV_STATE_CHECKED);
    lv_obj_set_style_text_color(btn, lv_color_black(), LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(btn, lv_color_hex(UI_COLOR_DIM_BUTTON), UI_STATE_DIM);
    lv_obj_set_style_text_color(btn, lv_color_hex(UI_COLOR_DIM_TEXT), UI_STATE_DIM);
    ui_track_press(btn);
    lv_obj_add_event_cb(btn, cb, LV_EVENT_CLICKED, (void *)arg);

    lv_obj_t *label = lv_label_create(btn);
    lv_obj_set_style_text_font(label, font, 0);
    lv_label_set_text_static(label, text);
    lv_obj_center(label);
    return btn;
}

static void make_name(lv_obj_t *parent, const char *text)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text_static(label, text);
}

/* Make a row with a minus button, a value label, and a plus button. Return the value label. */
static lv_obj_t *make_step_row(lv_obj_t *parent, lv_event_cb_t cb, lv_obj_t **down, lv_obj_t **up)
{
    lv_obj_t *row = make_box(parent, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    *down = make_button(row, "-", &font_settings_48, UI_STEP_BUTTON_W, cb, -1);
    lv_obj_t *value = lv_label_create(row);
    lv_obj_set_style_text_font(value, &font_settings_48, 0);
    *up = make_button(row, "+", &font_settings_48, UI_STEP_BUTTON_W, cb, +1);
    return value;
}

static lv_obj_t *make_group(lv_obj_t *parent, const char *name)
{
    lv_obj_t *group = make_box(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(group, UI_NAME_GAP, 0);
    make_name(group, name);
    return group;
}

static void build_screen(void)
{
    s_screen = lv_obj_create(NULL);
    lv_obj_remove_flag(s_screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(s_screen, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(s_screen, lv_color_black(), 0);
    lv_obj_set_style_text_color(s_screen, lv_color_white(), 0);
    lv_obj_set_style_text_font(s_screen, &font_settings_28, 0);
    lv_obj_set_style_pad_all(s_screen, UI_MARGIN, 0);
    lv_obj_set_flex_flow(s_screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(s_screen, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    ui_track_press(s_screen);
    lv_obj_add_event_cb(s_screen, gesture_event_cb, LV_EVENT_GESTURE, NULL);

    int32_t content_w = lv_display_get_horizontal_resolution(NULL) - 2 * UI_MARGIN;
    int32_t voice_w = (content_w - UI_NAME_GAP) / 2;

    lv_obj_t *voice = make_group(s_screen, "Voice");
    lv_obj_t *voice_row = make_box(voice, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(voice_row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    s_btn_samantha = make_button(voice_row, "Samantha", &font_settings_28, voice_w, voice_event_cb,
                                 SETTINGS_VOICE_SAMANTHA);
    s_btn_reed = make_button(voice_row, "Reed", &font_settings_28, voice_w, voice_event_cb,
                             SETTINGS_VOICE_REED);

    lv_obj_t *cycle = make_group(s_screen, "Cycle time");
    s_cycle_value = make_step_row(cycle, cycle_event_cb, &s_btn_cycle_down, &s_btn_cycle_up);

    lv_obj_t *volume = make_group(s_screen, "Volume");
    s_volume_value = make_step_row(volume, volume_event_cb, &s_btn_volume_down, &s_btn_volume_up);

    make_button(s_screen, "Done", &font_settings_28, content_w, done_event_cb, 0);
}

void ui_settings_open(const settings_t *current, ui_settings_close_cb_t on_close)
{
    if (!s_screen) {
        build_screen();
    }
    s_settings = *current;
    s_on_close = on_close;
    draw_values();
    lv_screen_load(s_screen);
}

#include "lvgl.h"
#include "ui.h"

LV_FONT_DECLARE(font_yes_no_160);
LV_FONT_DECLARE(font_icons_150);
LV_FONT_DECLARE(font_icons_80);

#define UI_COLOR_YES 0x007A29
#define UI_COLOR_NO  0xC62828

/* The gap between the icon and the word in cycle mode and in split mode. */
#define UI_ICON_GAP_CYCLE 32
#define UI_ICON_GAP_SPLIT 16

static lv_obj_t *s_screen;
static lv_obj_t *s_box;
static lv_obj_t *s_label;
static lv_obj_t *s_icon;
static lv_obj_t *s_icon_yes;
static lv_obj_t *s_icon_no;
static lv_obj_t *s_half_yes;
static lv_obj_t *s_half_no;
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

void ui_track_press(lv_obj_t *obj)
{
    lv_obj_add_event_cb(obj, ui_press_event_cb, LV_EVENT_PRESSED, NULL);
}

int32_t ui_press_start_y(void)
{
    return s_press_y;
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

/* Make a box with no look of its own in the center of parent. The box does not take presses, so
 * they reach the screen. */
static lv_obj_t *make_box(lv_obj_t *parent, lv_flex_flow_t flow, int32_t gap)
{
    lv_obj_t *box = lv_obj_create(parent);
    lv_obj_remove_style_all(box);
    lv_obj_remove_flag(box, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_size(box, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(box, flow);
    lv_obj_set_flex_align(box, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(box, gap, 0);
    lv_obj_set_style_pad_column(box, gap, 0);
    lv_obj_center(box);
    return box;
}

/* Make a white label in font. The label does not take presses. */
static lv_obj_t *make_label(lv_obj_t *parent, const lv_font_t *font)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_obj_remove_flag(label, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_text_font(label, font, 0);
    lv_obj_set_style_text_color(label, lv_color_white(), 0);
    return label;
}

/* Make one hidden half of the split screen at row y with the icon and the word on one line in its
 * center. Return the half and put its icon label in icon. The half does not take presses, so they
 * reach the screen. */
static lv_obj_t *make_half(int32_t y, int32_t height, uint32_t color, const char *symbol,
                           const char *word, lv_obj_t **icon)
{
    lv_obj_t *half = lv_obj_create(s_screen);
    lv_obj_remove_style_all(half);
    lv_obj_remove_flag(half, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(half, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_size(half, lv_display_get_horizontal_resolution(NULL), height);
    lv_obj_set_pos(half, 0, y);
    lv_obj_set_style_bg_opa(half, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(half, lv_color_hex(color), 0);

    /* The capitals fill the top 112 of the 113 rows of the word label, so the row centers the
     * icon on the capitals. */
    lv_obj_t *box = make_box(half, LV_FLEX_FLOW_ROW, UI_ICON_GAP_SPLIT);
    *icon = make_label(box, &font_icons_80);
    lv_label_set_text_static(*icon, symbol);
    lv_obj_t *label = make_label(box, &font_yes_no_160);
    lv_label_set_text_static(label, word);
    return half;
}

void ui_init(ui_tap_cb_t on_tap, ui_swipe_cb_t on_swipe_down)
{
    s_on_tap = on_tap;
    s_on_swipe_down = on_swipe_down;

    s_screen = lv_screen_active();
    lv_obj_remove_flag(s_screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(s_screen, LV_OPA_COVER, 0);
    lv_obj_add_event_cb(s_screen, ui_tap_event_cb, LV_EVENT_CLICKED, NULL);
    ui_track_press(s_screen);
    lv_obj_add_event_cb(s_screen, ui_gesture_event_cb, LV_EVENT_GESTURE, NULL);

    /* In cycle mode, the icon sits above the word and the two sit in the center as one group. */
    s_box = make_box(s_screen, LV_FLEX_FLOW_COLUMN, UI_ICON_GAP_CYCLE);
    s_icon = make_label(s_box, &font_icons_150);
    s_label = make_label(s_box, &font_yes_no_160);

    int32_t half_h = (lv_display_get_vertical_resolution(NULL) - SCREEN_CYCLE_SPLIT_GAP) / 2;
    s_half_yes = make_half(0, half_h, UI_COLOR_YES, LV_SYMBOL_OK, "Yes", &s_icon_yes);
    s_half_no = make_half(half_h + SCREEN_CYCLE_SPLIT_GAP, half_h, UI_COLOR_NO, LV_SYMBOL_CLOSE, "No",
                          &s_icon_no);
}

/* Show obj if show is true, and hide it if not. A hidden label takes no space in its box. */
static void set_shown(lv_obj_t *obj, bool show)
{
    if (show) {
        lv_obj_remove_flag(obj, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
    }
}

void ui_set_icons(bool show)
{
    set_shown(s_icon, show);
    set_shown(s_icon_yes, show);
    set_shown(s_icon_no, show);
}

void ui_show(screen_t screen)
{
    lv_obj_add_flag(s_half_yes, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(s_half_no, LV_OBJ_FLAG_HIDDEN);
    lv_obj_remove_flag(s_box, LV_OBJ_FLAG_HIDDEN);

    if (screen == SCREEN_YES) {
        lv_obj_set_style_bg_color(s_screen, lv_color_hex(UI_COLOR_YES), 0);
        lv_label_set_text_static(s_icon, LV_SYMBOL_OK);
        lv_label_set_text_static(s_label, "Yes");
    } else {
        lv_obj_set_style_bg_color(s_screen, lv_color_hex(UI_COLOR_NO), 0);
        lv_label_set_text_static(s_icon, LV_SYMBOL_CLOSE);
        lv_label_set_text_static(s_label, "No");
    }
}

void ui_show_split(void)
{
    /* The black screen behind the halves draws the band between them. */
    lv_obj_set_style_bg_color(s_screen, lv_color_black(), 0);
    lv_obj_add_flag(s_box, LV_OBJ_FLAG_HIDDEN);
    lv_obj_remove_flag(s_half_yes, LV_OBJ_FLAG_HIDDEN);
    lv_obj_remove_flag(s_half_no, LV_OBJ_FLAG_HIDDEN);
}

void ui_load(void)
{
    lv_screen_load(s_screen);
}

#include "lvgl.h"
#include "bsp/esp-bsp.h"
#include "bsp/display.h"
#include "screen_cycle.h"
#include "settings.h"
#include "settings_store.h"
#include "audio.h"
#include "ui.h"
#include "ui_settings.h"

#define TICK_PERIOD_MS 50
/* Time without a touch before the settings screen goes back by itself. */
#define SETTINGS_IDLE_MS 30000

/* What the panel shows. */
typedef enum {
    VIEW_NONE,
    VIEW_YES,
    VIEW_NO,
    VIEW_SPLIT,
} view_t;

static screen_cycle_t s_cycle;
static uint32_t s_last_tick;
static view_t s_view;
static bool s_lit;
/* The swap that the panel shows, and its word. */
static bool s_swap_shown;
static screen_t s_swap_word;
/* The settings in use. */
static settings_t s_settings;
/* The settings as the last good save left them in flash. */
static settings_t s_saved;
static bool s_in_settings;

/* Bring the panel in line with the cycle state. Runs with the display lock held. */
static void apply_state(void)
{
    if (!screen_cycle_is_on(&s_cycle)) {
        if (s_lit) {
            /* The screen wakes with its normal colors. */
            if (s_swap_shown) {
                ui_set_swap(false, s_swap_word);
                s_swap_shown = false;
            }
            bsp_display_brightness_set(0);
            s_lit = false;
        }
        return;
    }

    screen_t screen = screen_cycle_current(&s_cycle);
    view_t view = VIEW_SPLIT;
    if (!screen_cycle_is_split(&s_cycle)) {
        view = (screen == SCREEN_YES) ? VIEW_YES : VIEW_NO;
    }
    if (view != s_view) {
        if (view == VIEW_SPLIT) {
            ui_show_split();
        } else {
            ui_show(screen);
        }
        s_view = view;
        /* A new view starts with its normal colors. */
        s_swap_shown = false;
    }

    screen_t word = s_swap_word;
    bool swap = screen_cycle_spoken(&s_cycle, &word);
    if (swap != s_swap_shown || (swap && word != s_swap_word)) {
        ui_set_swap(swap, word);
        s_swap_shown = swap;
        s_swap_word = word;
    }
    if (!s_lit) {
        /* Draw the new screen before the panel lights up, so the old screen does not flash. */
        lv_refr_now(NULL);
        bsp_display_brightness_set(100);
        s_lit = true;
    }
}

/* Start the screen at yes with the saved cycle time and mode. */
static void start_screen(void)
{
    screen_cycle_init(&s_cycle, SCREEN_YES, settings_cycle_ms(&s_settings));
    screen_cycle_set_split(&s_cycle, s_settings.mode == SETTINGS_MODE_SPLIT);
}

/* Go back to the yes screen or the split screen with a full cycle time and a new 2 minute count.
 * Runs with the display lock held. */
static void on_settings_close(const settings_t *s)
{
    s_in_settings = false;

    if (s->volume != s_settings.volume) {
        audio_set_volume(s->volume);
    }
    s_settings = *s;

    /* A failed save is tried again at the next close. */
    if (!settings_equal(&s_settings, &s_saved) && settings_store_save(&s_settings) == ESP_OK) {
        s_saved = s_settings;
    }

    start_screen();
    ui_set_icons(s_settings.icons == SETTINGS_ICONS_SHOW);
    ui_load();
    s_last_tick = lv_tick_get();
    apply_state();
}

static void on_tick(lv_timer_t *timer)
{
    (void)timer;
    uint32_t elapsed = lv_tick_elaps(s_last_tick);
    s_last_tick += elapsed;

    /* The switch and the 2 minute count stop while the settings screen shows. */
    if (s_in_settings) {
        if (lv_display_get_inactive_time(NULL) >= SETTINGS_IDLE_MS) {
            ui_settings_close();
        }
        return;
    }

    if (screen_cycle_tick(&s_cycle, elapsed)) {
        apply_state();
    }
}

static void on_tap(void)
{
    screen_t word = screen_cycle_current(&s_cycle);

    /* On the lit split screen, the point where the press started picks the word. A press on the
     * band between the halves does nothing. */
    if (screen_cycle_is_split(&s_cycle) && screen_cycle_is_on(&s_cycle) &&
        !screen_cycle_split_word(ui_press_start_y(), lv_display_get_vertical_resolution(NULL), &word)) {
        return;
    }

    if (screen_cycle_tap_word(&s_cycle, word) == SCREEN_TAP_SPEAK) {
        /* The colors swap before the sound starts. */
        apply_state();
        audio_play(word, s_settings.voice);
        return;
    }

    /* Time from before the wake must not count toward the first switch. */
    s_last_tick = lv_tick_get();
    apply_state();
}

/* A swipe on the dark screen only wakes it, so it acts as a tap. */
static bool on_swipe_down(void)
{
    if (!screen_cycle_is_on(&s_cycle)) {
        return false;
    }
    s_in_settings = true;
    ui_settings_open(&s_settings, on_settings_close);
    return true;
}

/* Runs in the playback task, so it takes the display lock. */
static void on_speak_done(void)
{
    bsp_display_lock(0);
    /* A tap can queue a new clip while this task waits for the lock. That clip keeps the speaking
     * state, and its own end ends it. The check is safe here because audio_play runs only with the
     * lock held. */
    if (!audio_pending()) {
        screen_cycle_speak_done(&s_cycle);
        /* While the settings screen shows, the colors go back when it closes. */
        if (!s_in_settings) {
            apply_state();
        }
    }
    bsp_display_unlock();
}

void app_main(void)
{
    settings_store_load(&s_settings);
    s_saved = s_settings;

    bsp_display_start();
    ESP_ERROR_CHECK(audio_init(s_settings.volume, on_speak_done));

    bsp_display_lock(0);

    /* The driver lights the panel before the first frame. Keep it dark until the first screen is drawn. */
    bsp_display_brightness_set(0);
    s_lit = false;

    start_screen();
    ui_init(on_tap, on_swipe_down);
    ui_set_icons(s_settings.icons == SETTINGS_ICONS_SHOW);
    s_view = VIEW_NONE;
    apply_state();

    s_last_tick = lv_tick_get();
    lv_timer_create(on_tick, TICK_PERIOD_MS, NULL);

    bsp_display_unlock();
}

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

static screen_cycle_t s_cycle;
static uint32_t s_last_tick;
static screen_t s_shown;
static bool s_lit;
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
            bsp_display_brightness_set(0);
            s_lit = false;
        }
        return;
    }

    screen_t screen = screen_cycle_current(&s_cycle);
    if (screen != s_shown) {
        ui_show(screen);
        s_shown = screen;
    }
    if (!s_lit) {
        /* Draw the new screen before the panel lights up, so the old screen does not flash. */
        lv_refr_now(NULL);
        bsp_display_brightness_set(100);
        s_lit = true;
    }
}

/* Go back to the yes screen with a full cycle time and a new 2 minute count. Runs with the
 * display lock held. */
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

    screen_cycle_init(&s_cycle, SCREEN_YES, settings_cycle_ms(&s_settings));
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
    if (screen_cycle_tap(&s_cycle) == SCREEN_TAP_SPEAK) {
        audio_play(screen_cycle_current(&s_cycle), s_settings.voice);
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
    screen_cycle_speak_done(&s_cycle);
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

    screen_cycle_init(&s_cycle, SCREEN_YES, settings_cycle_ms(&s_settings));
    ui_init(on_tap, on_swipe_down);
    s_shown = screen_cycle_current(&s_cycle);
    ui_show(s_shown);
    apply_state();

    s_last_tick = lv_tick_get();
    lv_timer_create(on_tick, TICK_PERIOD_MS, NULL);

    bsp_display_unlock();
}

#include "lvgl.h"
#include "bsp/esp-bsp.h"
#include "bsp/display.h"
#include "screen_cycle.h"
#include "ui.h"

#define TICK_PERIOD_MS 50

static screen_cycle_t s_cycle;
static uint32_t s_last_tick;
static screen_t s_shown;
static bool s_lit;

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

static void on_tick(lv_timer_t *timer)
{
    (void)timer;
    uint32_t elapsed = lv_tick_elaps(s_last_tick);
    s_last_tick += elapsed;
    if (screen_cycle_tick(&s_cycle, elapsed)) {
        apply_state();
    }
}

static void on_tap(void)
{
    if (screen_cycle_tap(&s_cycle)) {
        /* Time from before the wake must not count toward the first switch. */
        s_last_tick = lv_tick_get();
        apply_state();
    }
}

void app_main(void)
{
    bsp_display_start();

    bsp_display_lock(0);

    /* The driver lights the panel before the first frame. Keep it dark until the first screen is drawn. */
    bsp_display_brightness_set(0);
    s_lit = false;

    screen_cycle_init(&s_cycle, SCREEN_YES);
    ui_init(on_tap);
    s_shown = screen_cycle_current(&s_cycle);
    ui_show(s_shown);
    apply_state();

    s_last_tick = lv_tick_get();
    lv_timer_create(on_tick, TICK_PERIOD_MS, NULL);

    bsp_display_unlock();
}

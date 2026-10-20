#include "screen_cycle.h"

/* A period of 0 would divide by zero in screen_cycle_tick. */
static uint32_t valid_period(uint32_t period_ms)
{
    return (period_ms == 0) ? SCREEN_CYCLE_DEFAULT_PERIOD_MS : period_ms;
}

void screen_cycle_init(screen_cycle_t *sc, screen_t first, uint32_t period_ms)
{
    sc->screen = first;
    sc->on = true;
    sc->speaking = false;
    sc->period_ms = valid_period(period_ms);
    sc->cycle_ms = 0;
    sc->idle_ms = 0;
}

bool screen_cycle_tick(screen_cycle_t *sc, uint32_t elapsed_ms)
{
    if (!sc->on) {
        return false;
    }

    sc->idle_ms += elapsed_ms;

    if (sc->idle_ms >= SCREEN_CYCLE_OFF_MS) {
        sc->on = false;
        sc->speaking = false;
        return true;
    }

    /* The screen holds while the word plays. */
    if (sc->speaking) {
        return false;
    }

    sc->cycle_ms += elapsed_ms;
    uint32_t switches = sc->cycle_ms / sc->period_ms;
    sc->cycle_ms %= sc->period_ms;

    /* An even number of switches ends on the same screen. */
    if (switches % 2 == 1) {
        sc->screen = (sc->screen == SCREEN_YES) ? SCREEN_NO : SCREEN_YES;
        return true;
    }
    return false;
}

screen_tap_t screen_cycle_tap(screen_cycle_t *sc)
{
    if (!sc->on) {
        screen_cycle_init(sc, SCREEN_YES, sc->period_ms);
        return SCREEN_TAP_WAKE;
    }

    sc->idle_ms = 0;
    sc->speaking = true;
    return SCREEN_TAP_SPEAK;
}

void screen_cycle_speak_done(screen_cycle_t *sc)
{
    if (!sc->speaking) {
        return;
    }

    /* The screen gets a full period after the word ends. */
    sc->speaking = false;
    sc->cycle_ms = 0;
}

void screen_cycle_set_period(screen_cycle_t *sc, uint32_t period_ms)
{
    sc->period_ms = valid_period(period_ms);
    sc->cycle_ms = 0;
}

screen_t screen_cycle_current(const screen_cycle_t *sc)
{
    return sc->screen;
}

bool screen_cycle_is_on(const screen_cycle_t *sc)
{
    return sc->on;
}

bool screen_cycle_is_speaking(const screen_cycle_t *sc)
{
    return sc->speaking;
}

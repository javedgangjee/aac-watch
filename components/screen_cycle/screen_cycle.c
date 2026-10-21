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
    sc->spoken = first;
    sc->split = false;
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

    /* The screen holds while the word plays, and never switches in split mode. */
    if (sc->speaking || sc->split) {
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

screen_tap_t screen_cycle_tap_word(screen_cycle_t *sc, screen_t word)
{
    if (!sc->on) {
        bool split = sc->split;
        screen_cycle_init(sc, SCREEN_YES, sc->period_ms);
        sc->split = split;
        return SCREEN_TAP_WAKE;
    }

    sc->idle_ms = 0;
    sc->speaking = true;
    sc->spoken = word;
    return SCREEN_TAP_SPEAK;
}

screen_tap_t screen_cycle_tap(screen_cycle_t *sc)
{
    return screen_cycle_tap_word(sc, sc->screen);
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

void screen_cycle_set_split(screen_cycle_t *sc, bool split)
{
    sc->split = split;
    sc->cycle_ms = 0;
}

bool screen_cycle_is_split(const screen_cycle_t *sc)
{
    return sc->split;
}

bool screen_cycle_split_word(int32_t y, int32_t height, screen_t *word)
{
    int32_t band_top = (height - SCREEN_CYCLE_SPLIT_GAP) / 2;

    if (y < band_top) {
        *word = SCREEN_YES;
        return true;
    }
    if (y < band_top + SCREEN_CYCLE_SPLIT_GAP) {
        return false;
    }
    *word = SCREEN_NO;
    return true;
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

bool screen_cycle_spoken(const screen_cycle_t *sc, screen_t *word)
{
    if (!sc->speaking) {
        return false;
    }
    *word = sc->spoken;
    return true;
}

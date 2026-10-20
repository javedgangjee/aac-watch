#include "screen_cycle.h"

void screen_cycle_init(screen_cycle_t *sc, screen_t first)
{
    sc->screen = first;
    sc->on = true;
    sc->cycle_ms = 0;
    sc->idle_ms = 0;
}

bool screen_cycle_tick(screen_cycle_t *sc, uint32_t elapsed_ms)
{
    if (!sc->on) {
        return false;
    }

    sc->idle_ms += elapsed_ms;
    sc->cycle_ms += elapsed_ms;

    if (sc->idle_ms >= SCREEN_CYCLE_OFF_MS) {
        sc->on = false;
        return true;
    }

    uint32_t switches = sc->cycle_ms / SCREEN_CYCLE_PERIOD_MS;
    sc->cycle_ms %= SCREEN_CYCLE_PERIOD_MS;

    /* An even number of switches ends on the same screen. */
    if (switches % 2 == 1) {
        sc->screen = (sc->screen == SCREEN_YES) ? SCREEN_NO : SCREEN_YES;
        return true;
    }
    return false;
}

bool screen_cycle_tap(screen_cycle_t *sc)
{
    if (!sc->on) {
        screen_cycle_init(sc, SCREEN_YES);
        return true;
    }

    sc->idle_ms = 0;
    return false;
}

screen_t screen_cycle_current(const screen_cycle_t *sc)
{
    return sc->screen;
}

bool screen_cycle_is_on(const screen_cycle_t *sc)
{
    return sc->on;
}

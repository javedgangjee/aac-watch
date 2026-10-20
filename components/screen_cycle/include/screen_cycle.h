#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Time between two screen switches. */
#define SCREEN_CYCLE_PERIOD_MS 5000
/* Time without a tap before the screen turns off. */
#define SCREEN_CYCLE_OFF_MS 120000

typedef enum {
    SCREEN_YES,
    SCREEN_NO,
} screen_t;

typedef enum {
    /* The tap turned the dark screen on. */
    SCREEN_TAP_WAKE,
    /* The tap was on the lit screen. Speak the word on the screen. */
    SCREEN_TAP_SPEAK,
} screen_tap_t;

typedef struct {
    screen_t screen;
    bool on;
    bool speaking;
    uint32_t cycle_ms;
    uint32_t idle_ms;
} screen_cycle_t;

/* Set the first screen, turn the screen on, clear the speaking state, and set both counts to zero. */
void screen_cycle_init(screen_cycle_t *sc, screen_t first);

/* Add elapsed_ms to the off count, and to the switch count when the screen is not speaking.
 * Return true if the screen or the on state changed. */
bool screen_cycle_tick(screen_cycle_t *sc, uint32_t elapsed_ms);

/* Handle a tap. On a dark screen, wake it at the yes screen. On a lit screen, restart the
 * off count and start the speaking state. */
screen_tap_t screen_cycle_tap(screen_cycle_t *sc);

/* End the speaking state and set the switch count to zero. Do nothing when not speaking. */
void screen_cycle_speak_done(screen_cycle_t *sc);

screen_t screen_cycle_current(const screen_cycle_t *sc);

bool screen_cycle_is_on(const screen_cycle_t *sc);

bool screen_cycle_is_speaking(const screen_cycle_t *sc);

#ifdef __cplusplus
}
#endif

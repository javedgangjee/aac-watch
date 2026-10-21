#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Time between two screen switches when the caller gives a period of 0. */
#define SCREEN_CYCLE_DEFAULT_PERIOD_MS 5000
/* Time without a tap before the screen turns off. */
#define SCREEN_CYCLE_OFF_MS 120000
/* Height in pixels of the band between the two halves of the split screen. */
#define SCREEN_CYCLE_SPLIT_GAP 10

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
    /* The word that plays. It has meaning only while speaking is true. */
    screen_t spoken;
    /* In split mode the screen shows both words and does not switch. */
    bool split;
    uint32_t period_ms;
    uint32_t cycle_ms;
    uint32_t idle_ms;
} screen_cycle_t;

/* Set the first screen and the period, turn the screen on, clear the speaking state, turn split
 * mode off, and set both counts to zero. A period of 0 becomes SCREEN_CYCLE_DEFAULT_PERIOD_MS. */
void screen_cycle_init(screen_cycle_t *sc, screen_t first, uint32_t period_ms);

/* Add elapsed_ms to the off count, and to the switch count when the screen is not speaking and
 * not in split mode. Return true if the screen or the on state changed. */
bool screen_cycle_tick(screen_cycle_t *sc, uint32_t elapsed_ms);

/* Handle a tap on the given word. On a dark screen, wake it at the yes screen and keep the period
 * and the mode. On a lit screen, restart the off count, start the speaking state, and record word
 * as the spoken word. */
screen_tap_t screen_cycle_tap_word(screen_cycle_t *sc, screen_t word);

/* Handle a tap on the word of the current screen, as screen_cycle_tap_word does. */
screen_tap_t screen_cycle_tap(screen_cycle_t *sc);

/* End the speaking state and set the switch count to zero. Do nothing when not speaking. */
void screen_cycle_speak_done(screen_cycle_t *sc);

/* Set the time between two screen switches and set the switch count to zero. A period of 0
 * becomes SCREEN_CYCLE_DEFAULT_PERIOD_MS. */
void screen_cycle_set_period(screen_cycle_t *sc, uint32_t period_ms);

/* Turn split mode on or off and set the switch count to zero. */
void screen_cycle_set_split(screen_cycle_t *sc, bool split);

bool screen_cycle_is_split(const screen_cycle_t *sc);

/* Find the half of the split screen under a press at row y on a screen of the given height.
 * Return false when the press is on the band between the halves. Otherwise write SCREEN_YES for
 * the top half or SCREEN_NO for the bottom half to word and return true. */
bool screen_cycle_split_word(int32_t y, int32_t height, screen_t *word);

screen_t screen_cycle_current(const screen_cycle_t *sc);

bool screen_cycle_is_on(const screen_cycle_t *sc);

bool screen_cycle_is_speaking(const screen_cycle_t *sc);

/* Return true while the screen is speaking and write the spoken word to word. Return false and
 * leave word as it is when the screen is not speaking. */
bool screen_cycle_spoken(const screen_cycle_t *sc, screen_t *word);

#ifdef __cplusplus
}
#endif

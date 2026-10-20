#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SETTINGS_CYCLE_MIN_S     3
#define SETTINGS_CYCLE_MAX_S     15
#define SETTINGS_CYCLE_STEP_S    1
#define SETTINGS_CYCLE_DEFAULT_S 5

/* The volume is on the codec scale of 0 to 100. */
#define SETTINGS_VOLUME_MIN     10
#define SETTINGS_VOLUME_MAX     100
#define SETTINGS_VOLUME_STEP    10
#define SETTINGS_VOLUME_DEFAULT 90

typedef enum {
    SETTINGS_VOICE_SAMANTHA,
    SETTINGS_VOICE_REED,
} settings_voice_t;

#define SETTINGS_VOICE_DEFAULT SETTINGS_VOICE_SAMANTHA

typedef enum {
    SETTINGS_MODE_CYCLE,
    SETTINGS_MODE_SPLIT,
} settings_mode_t;

#define SETTINGS_MODE_DEFAULT SETTINGS_MODE_CYCLE

typedef enum {
    SETTINGS_ICONS_HIDE,
    SETTINGS_ICONS_SHOW,
} settings_icons_t;

#define SETTINGS_ICONS_DEFAULT SETTINGS_ICONS_SHOW

typedef struct {
    settings_mode_t mode;
    settings_icons_t icons;
    settings_voice_t voice;
    uint8_t cycle_s;
    uint8_t volume;
} settings_t;

/* Set the mode, the icons, the voice, the cycle time, and the volume to their defaults. */
void settings_defaults(settings_t *s);

/* Move the cycle time one step up for dir +1 or down for dir -1, and stay inside the limits.
 * Return true if the value changed. */
bool settings_cycle_step(settings_t *s, int dir);

/* Move the volume one step up for dir +1 or down for dir -1, and stay inside the limits.
 * Return true if the value changed. */
bool settings_volume_step(settings_t *s, int dir);

/* Set each field that is out of range to its default. Return true if a field changed. */
bool settings_sanitize(settings_t *s);

uint32_t settings_cycle_ms(const settings_t *s);

/* Return true if every field of a and b is the same. */
bool settings_equal(const settings_t *a, const settings_t *b);

#ifdef __cplusplus
}
#endif

#include "settings.h"

void settings_defaults(settings_t *s)
{
    s->mode = SETTINGS_MODE_DEFAULT;
    s->icons = SETTINGS_ICONS_DEFAULT;
    s->voice = SETTINGS_VOICE_DEFAULT;
    s->cycle_s = SETTINGS_CYCLE_DEFAULT_S;
    s->volume = SETTINGS_VOLUME_DEFAULT;
}

/* Move value one step in dir and stay inside min and max. Return true if the value changed. */
static bool step(uint8_t *value, int dir, int step_size, int min, int max)
{
    int next = *value + dir * step_size;
    if (next < min || next > max) {
        return false;
    }
    *value = (uint8_t)next;
    return true;
}

bool settings_cycle_step(settings_t *s, int dir)
{
    return step(&s->cycle_s, dir, SETTINGS_CYCLE_STEP_S, SETTINGS_CYCLE_MIN_S, SETTINGS_CYCLE_MAX_S);
}

bool settings_volume_step(settings_t *s, int dir)
{
    return step(&s->volume, dir, SETTINGS_VOLUME_STEP, SETTINGS_VOLUME_MIN, SETTINGS_VOLUME_MAX);
}

bool settings_sanitize(settings_t *s)
{
    bool changed = false;

    if (s->mode != SETTINGS_MODE_CYCLE && s->mode != SETTINGS_MODE_SPLIT) {
        s->mode = SETTINGS_MODE_DEFAULT;
        changed = true;
    }
    if (s->icons != SETTINGS_ICONS_HIDE && s->icons != SETTINGS_ICONS_SHOW) {
        s->icons = SETTINGS_ICONS_DEFAULT;
        changed = true;
    }
    if (s->voice != SETTINGS_VOICE_SAMANTHA && s->voice != SETTINGS_VOICE_REED) {
        s->voice = SETTINGS_VOICE_DEFAULT;
        changed = true;
    }
    if (s->cycle_s < SETTINGS_CYCLE_MIN_S || s->cycle_s > SETTINGS_CYCLE_MAX_S) {
        s->cycle_s = SETTINGS_CYCLE_DEFAULT_S;
        changed = true;
    }
    if (s->volume < SETTINGS_VOLUME_MIN || s->volume > SETTINGS_VOLUME_MAX ||
        s->volume % SETTINGS_VOLUME_STEP != 0) {
        s->volume = SETTINGS_VOLUME_DEFAULT;
        changed = true;
    }
    return changed;
}

uint32_t settings_cycle_ms(const settings_t *s)
{
    return (uint32_t)s->cycle_s * 1000;
}

bool settings_equal(const settings_t *a, const settings_t *b)
{
    return a->mode == b->mode && a->icons == b->icons && a->voice == b->voice && a->cycle_s == b->cycle_s &&
           a->volume == b->volume;
}

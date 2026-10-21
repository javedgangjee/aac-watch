#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "esp_err.h"
#include "screen_cycle.h"
#include "settings.h"

typedef void (*audio_done_cb_t)(void);

/* Open the speaker codec at the given volume and start the playback task. on_done runs in the
 * playback task when a clip ends and no new request is waiting. If this function fails, the other
 * functions still work, and audio_play calls on_done at once in the task that calls it. */
esp_err_t audio_init(uint8_t volume, audio_done_cb_t on_done);

/* Play the clip for the given screen in the given voice. A play in progress stops and the new
 * clip starts from the start. Does not block. */
void audio_play(screen_t screen, settings_voice_t voice);

/* Return true if a play request waits for the playback task. */
bool audio_pending(void);

/* Set the speaker volume on the codec scale of 0 to 100. */
esp_err_t audio_set_volume(uint8_t volume);

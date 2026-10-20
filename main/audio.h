#pragma once

#include <stdint.h>
#include "esp_err.h"
#include "screen_cycle.h"
#include "settings.h"

typedef void (*audio_done_cb_t)(void);

/* Open the speaker codec at the given volume and start the playback task. on_done runs in the
 * playback task when a clip ends and no new request is waiting. */
esp_err_t audio_init(uint8_t volume, audio_done_cb_t on_done);

/* Play the clip for the given screen in the given voice. A play in progress stops and the new
 * clip starts from the start. Does not block. */
void audio_play(screen_t screen, settings_voice_t voice);

/* Set the speaker volume on the codec scale of 0 to 100. */
esp_err_t audio_set_volume(uint8_t volume);

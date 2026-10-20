#pragma once

#include "esp_err.h"
#include "screen_cycle.h"

typedef void (*audio_done_cb_t)(void);

/* Open the speaker codec and start the playback task. on_done runs in the playback task
 * when a clip ends and no new request is waiting. */
esp_err_t audio_init(audio_done_cb_t on_done);

/* Play the clip for the given screen. A play in progress stops and the new clip starts from
 * the start. Does not block. */
void audio_play(screen_t screen);

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "esp_check.h"
#include "esp_codec_dev.h"
#include "bsp/esp-bsp.h"
#include "audio.h"

#define AUDIO_SAMPLE_RATE 22050
#define AUDIO_BITS        16
#define AUDIO_CHANNELS    1
#define AUDIO_CHUNK_BYTES 1024

static const char *TAG = "audio";

extern const uint8_t _binary_yes_samantha_pcm_start[];
extern const uint8_t _binary_yes_samantha_pcm_end[];
extern const uint8_t _binary_no_samantha_pcm_start[];
extern const uint8_t _binary_no_samantha_pcm_end[];
extern const uint8_t _binary_yes_reed_pcm_start[];
extern const uint8_t _binary_yes_reed_pcm_end[];
extern const uint8_t _binary_no_reed_pcm_start[];
extern const uint8_t _binary_no_reed_pcm_end[];

typedef struct {
    screen_t screen;
    settings_voice_t voice;
} audio_request_t;

static esp_codec_dev_handle_t s_codec;
static QueueHandle_t s_queue;
static audio_done_cb_t s_on_done;

/* Set start and end to the clip for the request. */
static void pick_clip(const audio_request_t *req, const uint8_t **start, const uint8_t **end)
{
    bool yes = (req->screen == SCREEN_YES);
    if (req->voice == SETTINGS_VOICE_REED) {
        *start = yes ? _binary_yes_reed_pcm_start : _binary_no_reed_pcm_start;
        *end = yes ? _binary_yes_reed_pcm_end : _binary_no_reed_pcm_end;
    } else {
        *start = yes ? _binary_yes_samantha_pcm_start : _binary_no_samantha_pcm_start;
        *end = yes ? _binary_yes_samantha_pcm_end : _binary_no_samantha_pcm_end;
    }
}

static void play_task(void *arg)
{
    (void)arg;
    audio_request_t req;

    for (;;) {
        xQueueReceive(s_queue, &req, portMAX_DELAY);

        bool restart;
        do {
            restart = false;
            const uint8_t *pos;
            const uint8_t *end;
            pick_clip(&req, &pos, &end);

            while (pos < end) {
                /* A new tap stops this clip and starts the new one from the start. */
                if (xQueueReceive(s_queue, &req, 0) == pdTRUE) {
                    restart = true;
                    break;
                }
                int len = (end - pos < AUDIO_CHUNK_BYTES) ? (int)(end - pos) : AUDIO_CHUNK_BYTES;
                esp_codec_dev_write(s_codec, (void *)pos, len);
                pos += len;
            }
        } while (restart);

        if (uxQueueMessagesWaiting(s_queue) == 0 && s_on_done) {
            s_on_done();
        }
    }
}

esp_err_t audio_init(uint8_t volume, audio_done_cb_t on_done)
{
    s_on_done = on_done;

    s_codec = bsp_audio_codec_speaker_init();
    ESP_RETURN_ON_FALSE(s_codec, ESP_FAIL, TAG, "Speaker codec init failed");

    ESP_RETURN_ON_ERROR(audio_set_volume(volume), TAG, "Set volume failed");

    esp_codec_dev_sample_info_t fs = {
        .sample_rate = AUDIO_SAMPLE_RATE,
        .bits_per_sample = AUDIO_BITS,
        .channel = AUDIO_CHANNELS,
    };
    ESP_RETURN_ON_FALSE(esp_codec_dev_open(s_codec, &fs) == ESP_CODEC_DEV_OK,
                        ESP_FAIL, TAG, "Codec open failed");

    s_queue = xQueueCreate(1, sizeof(audio_request_t));
    ESP_RETURN_ON_FALSE(s_queue, ESP_ERR_NO_MEM, TAG, "Queue create failed");

    ESP_RETURN_ON_FALSE(xTaskCreate(play_task, "audio", 4096, NULL, 5, NULL) == pdPASS,
                        ESP_ERR_NO_MEM, TAG, "Task create failed");
    return ESP_OK;
}

void audio_play(screen_t screen, settings_voice_t voice)
{
    audio_request_t req = { .screen = screen, .voice = voice };
    xQueueOverwrite(s_queue, &req);
}

esp_err_t audio_set_volume(uint8_t volume)
{
    ESP_RETURN_ON_FALSE(esp_codec_dev_set_out_vol(s_codec, volume) == ESP_CODEC_DEV_OK,
                        ESP_FAIL, TAG, "Set volume failed");
    return ESP_OK;
}

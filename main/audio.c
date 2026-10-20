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
#define AUDIO_VOLUME      90
#define AUDIO_CHUNK_BYTES 1024

static const char *TAG = "audio";

extern const uint8_t _binary_yes_pcm_start[];
extern const uint8_t _binary_yes_pcm_end[];
extern const uint8_t _binary_no_pcm_start[];
extern const uint8_t _binary_no_pcm_end[];

static esp_codec_dev_handle_t s_codec;
static QueueHandle_t s_queue;
static audio_done_cb_t s_on_done;

static void play_task(void *arg)
{
    (void)arg;
    screen_t screen;

    for (;;) {
        xQueueReceive(s_queue, &screen, portMAX_DELAY);

        bool restart;
        do {
            restart = false;
            const uint8_t *pos = (screen == SCREEN_YES) ? _binary_yes_pcm_start : _binary_no_pcm_start;
            const uint8_t *end = (screen == SCREEN_YES) ? _binary_yes_pcm_end : _binary_no_pcm_end;

            while (pos < end) {
                /* A new tap stops this clip and starts the new one from the start. */
                if (xQueueReceive(s_queue, &screen, 0) == pdTRUE) {
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

esp_err_t audio_init(audio_done_cb_t on_done)
{
    s_on_done = on_done;

    s_codec = bsp_audio_codec_speaker_init();
    ESP_RETURN_ON_FALSE(s_codec, ESP_FAIL, TAG, "Speaker codec init failed");

    ESP_RETURN_ON_FALSE(esp_codec_dev_set_out_vol(s_codec, AUDIO_VOLUME) == ESP_CODEC_DEV_OK,
                        ESP_FAIL, TAG, "Set volume failed");

    esp_codec_dev_sample_info_t fs = {
        .sample_rate = AUDIO_SAMPLE_RATE,
        .bits_per_sample = AUDIO_BITS,
        .channel = AUDIO_CHANNELS,
    };
    ESP_RETURN_ON_FALSE(esp_codec_dev_open(s_codec, &fs) == ESP_CODEC_DEV_OK,
                        ESP_FAIL, TAG, "Codec open failed");

    s_queue = xQueueCreate(1, sizeof(screen_t));
    ESP_RETURN_ON_FALSE(s_queue, ESP_ERR_NO_MEM, TAG, "Queue create failed");

    ESP_RETURN_ON_FALSE(xTaskCreate(play_task, "audio", 4096, NULL, 5, NULL) == pdPASS,
                        ESP_ERR_NO_MEM, TAG, "Task create failed");
    return ESP_OK;
}

void audio_play(screen_t screen)
{
    xQueueOverwrite(s_queue, &screen);
}

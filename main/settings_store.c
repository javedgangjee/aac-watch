#include "esp_check.h"
#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "settings_store.h"

#define STORE_NAMESPACE "yes_no"
#define KEY_VOICE       "voice"
#define KEY_CYCLE_S     "cycle_s"
#define KEY_VOLUME      "volume"

static const char *TAG = "settings_store";

static esp_err_t start_nvs(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_LOGW(TAG, "NVS needs an erase: %s", esp_err_to_name(err));
        ESP_RETURN_ON_ERROR(nvs_flash_erase(), TAG, "NVS erase failed");
        err = nvs_flash_init();
    }
    return err;
}

/* Read one u8 key into value. Keep value as it is when the key is missing. */
static void read_u8(nvs_handle_t nvs, const char *key, uint8_t *value)
{
    esp_err_t err = nvs_get_u8(nvs, key, value);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
        ESP_LOGW(TAG, "Read of %s failed: %s", key, esp_err_to_name(err));
    }
}

void settings_store_load(settings_t *s)
{
    settings_defaults(s);

    esp_err_t err = start_nvs();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "NVS start failed: %s", esp_err_to_name(err));
        return;
    }

    nvs_handle_t nvs;
    err = nvs_open(STORE_NAMESPACE, NVS_READONLY, &nvs);
    if (err != ESP_OK) {
        /* The namespace does not exist before the first save. */
        if (err != ESP_ERR_NVS_NOT_FOUND) {
            ESP_LOGW(TAG, "NVS open failed: %s", esp_err_to_name(err));
        }
        return;
    }

    uint8_t voice = (uint8_t)s->voice;
    read_u8(nvs, KEY_VOICE, &voice);
    s->voice = (settings_voice_t)voice;
    read_u8(nvs, KEY_CYCLE_S, &s->cycle_s);
    read_u8(nvs, KEY_VOLUME, &s->volume);
    nvs_close(nvs);

    if (settings_sanitize(s)) {
        ESP_LOGW(TAG, "A saved value was out of range and now has its default");
    }
}

esp_err_t settings_store_save(const settings_t *s)
{
    nvs_handle_t nvs;
    ESP_RETURN_ON_ERROR(nvs_open(STORE_NAMESPACE, NVS_READWRITE, &nvs), TAG, "NVS open failed");

    esp_err_t err = nvs_set_u8(nvs, KEY_VOICE, (uint8_t)s->voice);
    if (err == ESP_OK) {
        err = nvs_set_u8(nvs, KEY_CYCLE_S, s->cycle_s);
    }
    if (err == ESP_OK) {
        err = nvs_set_u8(nvs, KEY_VOLUME, s->volume);
    }
    if (err == ESP_OK) {
        err = nvs_commit(nvs);
    }
    nvs_close(nvs);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Save failed: %s", esp_err_to_name(err));
    }
    return err;
}

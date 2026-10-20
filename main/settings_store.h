#pragma once

#include "esp_err.h"
#include "settings.h"

/* Start NVS and read the saved settings into s. A missing or out of range value gets its
 * default. */
void settings_store_load(settings_t *s);

/* Write the settings to NVS and commit them. */
esp_err_t settings_store_save(const settings_t *s);

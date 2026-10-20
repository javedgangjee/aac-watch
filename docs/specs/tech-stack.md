# Tech stack

## Firmware

The firmware is written in C with ESP-IDF v6.1, which is installed on the development Mac. Waveshare tests its board code on ESP-IDF v5.5.5 and v6.0.2, so a board driver may need a small fix on v6.1.

The project uses these components from the ESP-IDF component registry.

- The `waveshare/esp32_s3_touch_amoled_2_06` board support package at version 2 drives the screen, touch, and audio.
- The `lvgl/lvgl` library at version 9.5.0 draws the screens.
- The `espressif/esp_codec_dev` component at version 1.5 plays audio through the ES8311 codec.

The Waveshare example `examples/esp-idf/02_lvgl_demo_v9` in the board repo is the starting point for the project setup.

## Font

The words use Montserrat Bold at font size 160, which makes the capital letters about 112 pixels high. The `lv_font_conv` tool makes the font file once through Node and `npx`, and the file `main/font_yes_no_160.c` is checked in. The font file must not be compressed. The project draws with two LVGL threads, and LVGL decodes compressed glyphs with one shared state, so compressed letters come out jumbled.

## Audio clips

The clips for yes and no are made on the Mac at build time with the macOS `say` command. A Python 3.13 script runs `say`, converts the output to raw 16-bit PCM, and writes the files that the build embeds in the firmware flash. The ESP32-S3 has no English text-to-speech engine, so the watch plays stored clips.

MicroPython was considered for the firmware and rejected. The board has no ready MicroPython drivers for its screen, touch, power, and audio chips.

## Tests

Unity tests run on the Mac with the ESP-IDF linux target. They cover the screen and timer logic, which is kept apart from the drivers so it builds on the host. The linux target is in preview on ESP-IDF v6.1, so `idf.py set-target linux` needs the `--preview` flag.

Manual checks on the watch cover the display, touch, and sound.

## Where it runs

The firmware runs on the watch. The build, the clip script, and the host tests run on macOS.

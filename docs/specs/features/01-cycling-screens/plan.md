# Feature 1 plan: cycling screens

1. Set up the ESP-IDF project from the Waveshare example `02_lvgl_demo_v9`. Remove the LVGL demos, the performance monitor, and the extra built-in fonts from the copied settings. Keep the flash, PSRAM, and FreeRTOS settings.
   - This step creates `CMakeLists.txt`, `sdkconfig.defaults`, `partitions.csv`, `main/CMakeLists.txt`, `main/idf_component.yml`, and `.gitignore`.
   - The `.gitignore` file ignores `build/`, `managed_components/`, `sdkconfig`, and `sdkconfig.old`. The file `dependencies.lock` is kept in git.
   - The partition table keeps the 8 MB factory app partition and drops the 7 MB storage partition, which this project does not use.

2. Write the screen and timer logic as a plain C component with no ESP-IDF or LVGL calls.
   - This step creates `components/screen_cycle/CMakeLists.txt`, `components/screen_cycle/include/screen_cycle.h`, and `components/screen_cycle/screen_cycle.c`.
   - The header defines the screen type with the values `SCREEN_YES` and `SCREEN_NO`. It defines the constant `SCREEN_CYCLE_PERIOD_MS` with the value 5000 and the constant `SCREEN_CYCLE_OFF_MS` with the value 120000.
   - The function `screen_cycle_init` sets the first screen, turns the screen on, and sets both counts to zero.
   - The function `screen_cycle_tick` adds the time since the last call to both counts. If the 2 minute count reaches `SCREEN_CYCLE_OFF_MS`, the screen turns off and does not switch in that call. Otherwise the screen switches once for each full period, and the time left over is kept. When the screen is off, the call changes nothing. The function returns true if the screen or the on state changed.
   - The function `screen_cycle_tap` handles a tap. On a dark screen it turns the screen on, sets the yes screen, sets both counts to zero, and returns true. On a lit screen it sets the 2 minute count to zero and returns false.
   - The function `screen_cycle_current` returns the screen to show, and the function `screen_cycle_is_on` returns whether the screen is on.

3. Write the host test project and the Unity tests from validation.md.
   - This step creates `test/host/CMakeLists.txt`, `test/host/sdkconfig.defaults`, `test/host/main/CMakeLists.txt`, and `test/host/main/test_screen_cycle.c`.
   - The test project adds `components/screen_cycle` through `EXTRA_COMPONENT_DIRS` and builds for the linux target.

4. Make the font file with `lv_font_conv` and check it in.
   - This step creates `main/font_yes_no_160.c`.
   - The step runs `idf.py set-target esp32s3` first, so that the component manager downloads LVGL into `managed_components/`.
   - The command is `npx lv_font_conv --font managed_components/lvgl__lvgl/tests/src/test_files/fonts/Montserrat-Bold.ttf --symbols YesNo --size 160 --bpp 4 --no-compress --format lvgl --lv-include lvgl.h --lv-font-name font_yes_no_160 -o main/font_yes_no_160.c`.
   - The font must not be compressed. The project draws with two LVGL threads, and LVGL decodes compressed glyphs with one shared state. Two threads that decode at the same time corrupt the letters.

5. Write the drawing code.
   - This step creates `main/ui.h` and `main/ui.c`.
   - The function `ui_init` makes one LVGL screen with one label in the center that uses `font_yes_no_160`. It takes a callback that runs when the user taps the screen.
   - The function `ui_show` takes a screen value. It sets the background to `#007A29` and the text to "Yes" for `SCREEN_YES`, and it sets the background to `#C62828` and the text to "No" for `SCREEN_NO`. The text is white for both.

6. Write the entry point.
   - This step creates `main/main.c`.
   - The function `app_main` starts the display with `bsp_display_start`, sets the brightness to 100 percent, and calls `ui_init` and `ui_show` for the first screen while it holds the display lock.
   - An LVGL timer runs every 50 ms. It calls `screen_cycle_tick` with the time since its last run.
   - The tap callback calls `screen_cycle_tap`.
   - When the tick or the tap returns true, the firmware applies the new state. It calls `ui_show` for the current screen, and it sets the brightness to 100 percent when the screen is on and to 0 percent when the screen is off.

7. Build the firmware for the `esp32s3` target with ESP-IDF v6.1. If a board driver fails to build on v6.1, stop and tell the user before any change to the driver.

8. Run the host tests and the firmware build, and fix any failures.

# Feature 1 requirements: cycling screens

## From the specs

1. The watch shows a yes screen with a green background and the word "Yes" in large white letters. The source is mission.md, "What it does".
2. The watch shows a no screen with a red background and the word "No" in large white letters. The source is mission.md, "What it does".
3. Each word is in title case and sits in the center of the screen, both horizontally and vertically. The source is mission.md, "What it does".
4. The screens switch every 5 seconds. The source is mission.md, "What it does", and roadmap.md, feature 1.
5. The screen and timer logic is kept apart from the drivers so that it builds and runs on the Mac with the ESP-IDF linux target. Unity tests cover it. The source is tech-stack.md, "Tests".
6. The project is set up from the Waveshare example `02_lvgl_demo_v9`. It uses ESP-IDF v6.1, the board support package at version 2, and LVGL 9.5.0. The source is tech-stack.md, "Firmware", and roadmap.md, feature 1.

## Not covered by the specs

The user confirmed each item below during planning.

1. **Confirmed.** The words use Montserrat Bold at 160 pixels high. The largest font built into LVGL is 48 pixels, which is small on a 410 by 502 screen. The font is made once with the `lv_font_conv` tool through Node and `npx`. The font file holds only the letters Y, e, s, N, and o. The output file `main/font_yes_no_160.c` is checked in, so the build does not need Node. The source font is `Montserrat-Bold.ttf` from the LVGL component, under the SIL Open Font License.
2. **Confirmed.** The green is `#007A29` and the red is `#C62828`. White text on each of these colors has a contrast ratio above 4.5 to 1.
3. **Confirmed.** The watch shows the yes screen first after power on.
4. **Confirmed.** The screen brightness is 100 percent.
5. **Confirmed.** The screen turns off after 2 minutes of cycling. A tap turns it back on at full brightness. The watch does not use a wrist raise. The first tap on the dark screen starts the cycle again and does not speak. The screen turns back on at the yes screen with a fresh 5 seconds.
6. **Confirmed.** The project uses these file names.
   - The screen and timer logic is the component `components/screen_cycle/`.
   - The drawing code is `main/ui.c` and `main/ui.h`.
   - The host test project is `test/host/`.
7. **Confirmed.** The project manifest `main/idf_component.yml` does not list the `espressif/esp_codec_dev` and `espressif/usb` components that the example uses. The board support package lists both as its own public dependencies, so the build still includes them. Feature 2 adds `esp_codec_dev` to the project manifest.
8. **Confirmed.** A tap on the lit screen restarts the 2 minute count. It does nothing else in this feature. Feature 2 makes it speak.

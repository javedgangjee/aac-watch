# Feature 6 plan: icons

1. Add the icons setting to the settings logic.
   - This step changes `components/settings/include/settings.h` and `components/settings/settings.c`.
   - The header defines `settings_icons_t` with the values `SETTINGS_ICONS_HIDE` and `SETTINGS_ICONS_SHOW`, and the constant `SETTINGS_ICONS_DEFAULT`, which is `SETTINGS_ICONS_SHOW`. The struct `settings_t` gets the field `icons`.
   - The function `settings_defaults` sets the icons to shown. The function `settings_sanitize` sets a value that is neither of the two to shown. The function `settings_equal` also compares the icons.

2. Update the host tests.
   - This step changes `test/host/main/test_settings.c`.
   - The settings tests add cases S17 to S20 from validation.md.

3. Save and load the icons setting.
   - This step changes `main/settings_store.c`.
   - The store reads and writes the key `icons` as a `u8`, in the same way as the key `mode`.

4. Make the icon font files.
   - This step creates `tools/fonts/FontAwesome5-Solid+Brands+Regular.woff`, `tools/fonts/FontAwesome5-LICENSE.txt`, `main/font_icons_150.c`, and `main/font_icons_80.c`, and changes `main/CMakeLists.txt`.
   - The woff file and the license come from `managed_components/lvgl__lvgl/scripts/built_in_font/`.
   - The first command is `npx lv_font_conv --font tools/fonts/FontAwesome5-Solid+Brands+Regular.woff --range 0xF00C,0xF00D --size 150 --bpp 4 --no-compress --format lvgl --lv-include lvgl.h --lv-font-name font_icons_150 -o main/font_icons_150.c`.
   - The second command is the same with size 80, the name `font_icons_80`, and the output `main/font_icons_80.c`.
   - The `SRCS` list in `main/CMakeLists.txt` adds the two font files.

5. Draw the icons on the yes and no screens.
   - This step changes `main/ui.c` and `main/ui.h`.
   - In cycle mode, a box with a column layout sits in the center of the screen. It holds an icon label in `font_icons_150` and the word label below it, with a 32 pixel gap. The function `ui_show` sets the icon label to the check on the yes screen and to the X on the no screen.
   - In split mode, each half holds a box with a row layout in its center. The box holds an icon label in `font_icons_80` and the word label to its right, with a 16 pixel gap, and the two labels are centered on the same line.
   - The new function `ui_set_icons` shows or hides the three icon labels. A hidden label takes no space, so the word moves to the center as in feature 5.
   - The boxes and the icon labels are not clickable and not scrollable, so presses reach the screen object.

6. Add the Icons row and the scroll to the settings screen.
   - This step changes `main/ui_settings.c`.
   - A group named "Icons" is the second row. It holds a "Show" button and a "Hide" button, laid out as the voice row is, and the chosen one is lit. A tap on a button sets the icons setting and redraws the values.
   - The screen scrolls up and down and does not bounce past its ends. The rows sit at the top in place of the even spread, and Done sits 20 pixels below the last row.
   - The new constant `UI_ROW_H` is 81. The function `make_group` sets each group to that fixed height.
   - Each group after Mode starts with a 1 pixel line in the color `UI_COLOR_BUTTON`, with 10 pixels of space above and below it. The line is part of its group, so when `draw_values` hides the cycle time group, its line hides too. The fixed height of a group does not count its line.
   - A press that starts in the bottom 60 pixels turns off the scroll of the screen, and the release turns it on again. This lets the swipe up from the bottom edge close the screen at any scroll position.
   - The function `ui_settings_open` scrolls the list to the top.

7. Connect the icons setting in the main loop.
   - This step changes `main/main.c`.
   - After `ui_init` at start, and in `on_settings_close` before `apply_state`, the code calls `ui_set_icons` with the saved setting.

8. Update the README.
   - This step changes `README.md`.
   - The use section says the settings screen has five settings and scrolls, and it describes the icons setting and where each icon sits. The license section credits Font Awesome and names its license. The `deslopificate` skill checks the changed text for AI tells and tropes.

9. Set the status of feature 6 to in progress in `docs/specs/roadmap.md`. This step is done during planning.

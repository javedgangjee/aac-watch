# Feature 6 requirements: icons

## From the specs

1. The yes screen gets a check mark icon, and the no screen gets an X icon. The source is mission.md, "Stretch goals", and roadmap.md, feature 6.
2. A setting on the settings screen shows or hides the icons. The source is mission.md, "Stretch goals", and roadmap.md, feature 6.
3. In cycle mode, the icon sits above the word. The source is mission.md, "Stretch goals", and roadmap.md, feature 6.
4. In split mode, the icon sits on the same line as the word. The source is mission.md, "Stretch goals", and roadmap.md, feature 6.
5. A swipe up from the bottom edge or a Done button goes back from the settings screen. The source is mission.md, "What it does".
6. The font files must not be compressed. The source is tech-stack.md, "Font".
7. The screen and timer logic and the settings logic stay apart from the drivers, and Unity tests on the Mac cover them. The source is tech-stack.md, "Tests".
8. The cycle mode, the split mode, the taps, the swipes, and the 2 minute count work as they do now. The source is mission.md, "What it does" and "Stretch goals".

## Not covered by the specs

The user confirmed each item below during planning.

1. **Confirmed.** The settings screen gets a row named "Icons". The rows do not all fit on the screen, so the settings screen scrolls up and down. Each row keeps its size, and Done stays at the end of the list.
2. **Confirmed.** A press that starts in the bottom 60 pixels of the settings screen does not scroll the list, so a swipe up from the bottom edge closes the screen at any scroll position. A drag that starts anywhere else scrolls the list. LVGL sends no swipe event while a list can scroll, so this rule keeps item 5 from the specs true.
3. **Confirmed.** The list does not bounce past its ends. Each time the settings screen opens, the list starts at the top.
4. **Confirmed.** The rows are Mode, Icons, Voice, Cycle time, and Volume in that order. The Icons row has a "Show" button and a "Hide" button, laid out as the voice row is, and the chosen one is lit.
5. **Confirmed.** Each settings row has a fixed height of 81 pixels, which is the name, a 6 pixel gap, and a 48 pixel button row. A row never grows or shrinks to fill the screen.
6. **Confirmed.** A 1 pixel horizontal line in the button color `0x333333` sits between each two settings rows, with 10 pixels of space above and below it. No line sits between Volume and Done, and Done sits 20 pixels below Volume.
7. **Confirmed.** In split mode, the cycle time row and the line above it are hidden. The rows below move up and keep their height, and no blank space or double line is left. This replaces item 2 of feature 5, where the other rows spread out.
8. **Confirmed.** The icons show by default. The watch saves the setting in NVS with the key `icons` in the namespace `aac_watch`. A missing or out of range value becomes shown, so a watch with saved settings from feature 5 shows the icons and keeps its other settings.
9. **Confirmed.** The icons are the check glyph U+F00C and the X glyph U+F00D from the Font Awesome 5 font that ships with LVGL. The `lv_font_conv` tool makes them into two font files, each with both glyphs and not compressed.
10. **Confirmed.** The source file `FontAwesome5-Solid+Brands+Regular.woff` and its license are copied from the LVGL component to `tools/fonts/`, so the font files can be made again from the repo. The font files are under the SIL Open Font License 1.1, and the README credits Font Awesome.
11. **Confirmed.** In cycle mode, the icon font has size 150. The check is 150 by 113 pixels, which matches the 112 pixel capitals, and the X is 104 by 104 pixels. The icon sits 32 pixels above the word, and the icon and the word sit in the center of the screen as one group.
12. **Confirmed.** In split mode, the word keeps size 160 and the icon is 80 pixels wide, with a 16 pixel gap. The icon font has size 80, so the check is 80 by 60 pixels and the X is 55 by 56 pixels. The line "check Yes" is about 381 pixels wide, with about 14 pixels to each edge of the half.
13. **Confirmed.** In split mode, the icon sits to the left of the word. The icon and the word sit in the center of their half as one line, and the icon is centered on the capitals.
14. **Confirmed.** The icons are white, as the words are. When the icons are hidden, each screen looks as it does in feature 5.
15. **Confirmed.** The icons do not take presses, so a tap on an icon acts as a tap on its screen or its half.
16. **Confirmed.** The project uses these names.
    - The settings type `settings_icons_t` has the values `SETTINGS_ICONS_HIDE` and `SETTINGS_ICONS_SHOW`, the constant `SETTINGS_ICONS_DEFAULT` is `SETTINGS_ICONS_SHOW`, and `settings_t` gets the field `icons`.
    - The icon font files are `main/font_icons_150.c` and `main/font_icons_80.c`.
    - The yes and no screen code gets the function `ui_set_icons`.
17. **Confirmed.** The README use section describes the icons setting and the scrolling settings screen, and the license section credits Font Awesome.
18. **Confirmed.** tech-stack.md does not list the icon font. The replan step after this feature adds it to the "Font" section.

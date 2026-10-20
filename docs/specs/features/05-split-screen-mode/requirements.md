# Feature 5 requirements: split screen mode

## From the specs

1. The watch has a split mode. In split mode, the top half of the screen shows yes on green and the bottom half shows no on red, and the screen does not switch. The source is mission.md, "Stretch goals", and roadmap.md, feature 5.
2. In split mode, a tap on the yes half speaks yes, and a tap on the no half speaks no. The source is mission.md, "Stretch goals", and roadmap.md, feature 5.
3. The mode described in "What it does" is the cycle mode, and it works as it does now. The source is mission.md, "Stretch goals".
4. A person picks the cycle mode or the split mode on the settings screen. The source is mission.md, "Stretch goals", and roadmap.md, feature 5.
5. Each word is in title case and in large white letters. The source is mission.md, "What it does".
6. The screen turns off after 2 minutes with no tap, a tap on the dark screen turns it on and does not speak, and a tap on the lit screen restarts the 2 minute count. The source is mission.md, "What it does".
7. The screen and timer logic stays apart from the drivers, and Unity tests on the Mac cover it. The source is tech-stack.md, "Tests".

## Not covered by the specs

The user confirmed each item below during planning.

1. **Confirmed.** The settings screen gets a row named "Mode" at the top, with two buttons named "Cycle" and "Split". The chosen one is lit. Every button on the settings screen goes from 56 to 48 pixels high, so the four rows and Done fit.
2. **Confirmed.** The cycle time row is hidden while "Split" is lit, and the other rows spread out. The row shows again when "Cycle" is lit.
3. **Confirmed.** In split mode, a tap on a half while a word plays stops the clip and plays the word of the tapped half from the start. This follows the feature 2 rule.
4. **Confirmed.** A black band separates the two halves. A tap on the band does not speak.
5. **Confirmed.** The band is 10 pixels high and covers rows 246 to 255. The yes half covers rows 0 to 245, and the no half covers rows 256 to 501. Each half is 410 by 246 pixels.
6. **Confirmed.** A tap on the band of the lit screen does nothing, and it does not restart the 2 minute count. A tap on the band of the dark screen wakes the screen, as any tap does.
7. **Confirmed.** The point where the press starts picks the half. This is the same point that the swipe check uses.
8. **Confirmed.** The default mode is cycle. The watch saves the mode in NVS with the key `mode` in the namespace `aac_watch`. A missing or out of range mode becomes cycle, so a watch with saved settings from feature 3 starts in cycle mode and keeps its other settings.
9. **Confirmed.** In split mode, a tap on the dark screen turns it on at the split screen and does not speak. The mode does not change when the screen turns off.
10. **Confirmed.** In split mode, the settings screen opens and closes as in feature 3. A swipe down that starts in the top 60 pixels opens it. A swipe that does not open it acts as a tap at the point where the press started. After the settings screen closes, the 2 minute count starts from zero.
11. **Confirmed.** In split mode, each word keeps Montserrat Bold at size 160 and sits in the center of its half. The word "Yes" is about 294 pixels wide and 113 pixels high, so it fits in a half.
12. **Confirmed.** The halves use the same green and red as the cycle mode screens.
13. **Confirmed.** The project uses these names, and the feature adds no new source files.
    - The settings type `settings_mode_t` has the values `SETTINGS_MODE_CYCLE` and `SETTINGS_MODE_SPLIT`, and `settings_t` gets the field `mode`.
    - The screen and timer logic gets the field `split`, the constant `SCREEN_CYCLE_SPLIT_GAP`, and the functions `screen_cycle_set_split`, `screen_cycle_is_split`, and `screen_cycle_split_word`.
    - The yes and no screen code gets the function `ui_show_split`.
14. **Confirmed.** The README use section describes the split mode and the mode setting.

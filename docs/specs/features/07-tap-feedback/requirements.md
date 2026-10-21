# Feature 7 requirements: tap feedback

## From the specs

1. When a tap speaks a word, the tapped screen or half swaps its background color and its text color while the word plays. The source is mission.md, "Stretch goals", and roadmap.md, feature 7.
2. The colors go back when the word ends or stops. The source is roadmap.md, feature 7.
3. The swap works in cycle mode and in split mode. The source is roadmap.md, feature 7.
4. A tap on the dark screen wakes it, does not speak, and does not swap the colors. The source is mission.md, "What it does", and roadmap.md, feature 7.
5. The screen and timer logic stays apart from the drivers, and Unity tests on the Mac cover it. The source is tech-stack.md, "Tests".
6. The cycle mode, the split mode, the icons, the taps, the swipes, and the 2 minute count work as they do now. The source is mission.md, "What it does" and "Stretch goals".

## Not covered by the specs

Each item below is open until the user confirms it.

1. **Open.** The swap makes the background white and makes the word and the icon green `0x007A29` on the yes screen or red `0xC62828` on the no screen. In split mode, only the tapped half swaps. The other half and the black band between the halves do not change.
2. **Open.** The colors swap at the tap, before the sound starts. They go back when the clip ends, when the screen turns off, or when the settings screen closes.
3. **Open.** A tap on the same screen or half while its word plays starts the word again, and the colors stay swapped. In split mode, a tap on the other half while a word plays puts the first half back at once and swaps the new half.
4. **Open.** A swipe down while a word plays opens the settings screen, and the word plays to its end as it does now. When the settings screen closes, the yes screen or the split screen shows with its normal colors.
5. **Open.** The settings screen gets no setting to turn the swap off.
6. **Open.** The project uses these names.
    - The screen and timer logic gets the field `spoken` in `screen_cycle_t`, the function `screen_cycle_tap_word`, and the function `screen_cycle_spoken`. The function `screen_cycle_tap` stays and taps the word on the current screen.
    - The yes and no screen code gets the function `ui_set_swap`.
7. **Open.** The README use section says that the tapped screen or half shows white with the word in green or red while the word plays.

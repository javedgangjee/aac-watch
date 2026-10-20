# Feature 3 requirements: settings screen

## From the specs

1. The watch has a settings screen where a person sets the voice, the cycle time, and the speaker volume. The source is mission.md, "What it does", and roadmap.md, feature 3.
2. The voice is a female voice or a male voice. The source is mission.md, "What it does".
3. The male voice clips are made with `say`. The source is roadmap.md, feature 3, and tech-stack.md, "Audio clips".
4. The cycle time is 5 seconds by default. The source is mission.md, "What it does", and roadmap.md, feature 3.
5. A person opens the settings screen with a gesture and goes back to the yes and no screens. The source is roadmap.md, feature 3. The user changed the side buttons to a swipe during planning, and mission.md and roadmap.md now say so.
6. The screen and timer logic stays apart from the drivers, and Unity tests on the Mac cover it. The source is tech-stack.md, "Tests".
7. Feature 2 behaviour stays the same with the default settings. The source is mission.md, "What it does".

## Not covered by the specs

The user confirmed each item below during planning.

1. **Confirmed.** A swipe down that starts near the top edge of the lit yes or no screen opens the settings screen. The swipe does not speak.
2. **Confirmed.** A swipe up that starts near the bottom edge of the settings screen goes back. A Done button on the settings screen also goes back.
3. **Confirmed.** After 30 seconds with no touch on the settings screen, the watch goes back by itself.
4. **Confirmed.** The cycle time goes from 3 to 15 seconds in steps of 1 second.
5. **Confirmed.** The volume goes from 10 to 100 in steps of 10 on the codec scale, and the default is 90.
6. **Confirmed.** The male voice is "Reed (English (US))". The female voice stays Samantha.
7. **Confirmed.** The settings stay after a restart. The watch saves them in the NVS flash partition.
8. **Confirmed.** The settings screen has a row for each setting. The voice row has two buttons named "Samantha" and "Reed", and the chosen one is lit. The cycle time row and the volume row each show the value between a minus button and a plus button. A Done button sits below the rows.
9. **Confirmed.** The settings screen makes no sound when a setting changes.
10. **Confirmed.** The swipe down must start in the top 60 pixels of the screen. The swipe up must start in the bottom 60 pixels.
11. **Confirmed.** A touch on the dark screen only wakes it at the yes screen, even if the touch is a swipe down from the top.
12. **Confirmed.** A swipe that does not open or close the settings screen acts as a tap, as it does now.
13. **Confirmed.** When a person goes back by any of the three ways, the watch saves the settings if they changed. It then shows the yes screen, and the first switch comes one full cycle time later. The 2 minute count starts from zero.
14. **Confirmed.** While the settings screen shows, the yes and no switch and the 2 minute count stop. A word that plays when the settings screen opens plays to the end.
15. **Confirmed.** At the lowest or highest value, the minus or plus button does nothing and shows dimmed.
16. **Confirmed.** If a saved value is missing or out of range, the watch uses the default for that value.
17. **Confirmed.** The settings screen uses Atkinson Hyperlegible, and the Yes and No words stay in Montserrat Bold. The font is under the SIL Open Font License and comes from the Google Fonts repo on GitHub.
18. **Confirmed.** The settings screen has a black background and white text. It uses Atkinson Hyperlegible Regular at size 28 for the names and the button text, and Atkinson Hyperlegible Bold at size 48 for the values and the minus and plus signs. Each font file holds the printable ASCII letters and is not compressed. The source TTF files and the license are checked in at `tools/fonts/`.
19. **Confirmed.** The project uses these names.
    - The settings logic is the component `components/settings/`, with `settings.c` and `include/settings.h`.
    - The flash storage code is `main/settings_store.c` and `main/settings_store.h`.
    - The settings screen code is `main/ui_settings.c` and `main/ui_settings.h`.
    - The clips are `yes_samantha.pcm`, `no_samantha.pcm`, `yes_reed.pcm`, and `no_reed.pcm`.
    - The NVS namespace is `yes_no`, and the keys are `voice`, `cycle_s`, and `volume`.
    - The settings host tests are in `test/host/main/test_settings.c`.
    - The settings font files are `main/font_settings_28.c` and `main/font_settings_48.c`.

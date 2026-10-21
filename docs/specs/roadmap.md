# Roadmap

1. **Cycling screens.** Status is done.
   The goal is a watch that switches between the yes screen and the no screen every 5 seconds.
   This feature covers the green yes screen, the red no screen, and the 5 second switch. It also sets up the ESP-IDF project from the Waveshare board drivers.

2. **Tap to speak.** Status is done.
   The goal is a watch that speaks the word on the screen when the user taps it.
   This feature covers the clips made with `say`, playback through the ES8311 codec and speaker, and the tap. It also covers the timer rule, which holds the screen while the word plays and then gives it a full 5 seconds. A tap on the dark screen still wakes the screen and does not speak.

3. **Settings screen.** Status is done.
   The goal is a settings screen where a person sets the voice, the cycle time, and the speaker volume.
   This feature covers the settings screen, the male voice clips made with `say`, and the three settings. The voice is female or male, and the cycle time is 5 seconds by default. It also covers the swipe down from the top that opens the settings screen and how a person goes back to the yes and no screens. The swipe replaced the side buttons during planning.

4. **Cleanup for distribution.** Status is done.
   The goal is a project that another person can get, build, and run from the repo.
   This feature covers a license file and a README that says what the project does and how to build and flash it. It also renames the project from yes-no to aac-watch, which the user added during planning.

## Stretch goals

5. **Split screen mode.** Status is done.
   The goal is a split screen view that shows the yes screen on the top half and the no screen on the bottom half at the same time.
   This feature covers the split screen view, the tap on each half that speaks its word, and a mode setting on the settings screen, where a person picks the cycle mode or the split mode.

6. **Icons.** Status is done.
   The goal is a check mark icon on the yes screen and an X icon on the no screen.
   This feature covers the two icons and a setting on the settings screen that shows or hides them. In cycle mode, the icon sits above the word, as in a typical AAC app. In split mode, the icon sits on the same line as the word.

7. **Tap feedback.** Status is planned.
   The goal is a screen that shows a tap was heard, with the colors of the tapped screen or half swapped while its word plays.
   This feature covers the swap of the background color and the text color on a tap that speaks, in cycle mode and in split mode. The colors go back when the word ends or stops. A tap on the dark screen wakes it and does not swap the colors.

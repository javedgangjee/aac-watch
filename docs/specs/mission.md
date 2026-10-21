# Mission

aac-watch is a simple AAC app on a wrist watch. It lets a person who cannot speak answer yes or no questions out loud.

## Who uses it

A non-speaking person wears the watch. People who talk with that person hear the answer from the watch speaker.

## What it does

In normal use, the watch shows one of two screens. The yes screen has a green background with the word "Yes" in large white letters. The no screen has a red background with the word "No" in large white letters. Each word is in title case and sits in the center of the screen, both horizontally and vertically.

The screens switch at a set cycle time, which is 5 seconds by default. When the user taps the screen, the watch speaks the word on that screen. The screen stays put while the word plays. After the word ends, the screen stays for a full cycle time before the next switch.

The screen turns off after 2 minutes with no tap. A tap on the dark screen turns it back on at the yes screen and does not speak. A tap on the lit screen restarts the 2 minute count.

The watch also has a settings screen, which a person opens with a swipe down from the top edge of the screen. On it, the person sets the voice, the cycle time, and the speaker volume. The voice is Samantha, a female voice, or Reed, a male voice. A swipe up from the bottom edge or a Done button goes back to the yes and no screens.

## Stretch goals

The watch gets a second mode, which is the split mode. The mode described above is the cycle mode. In split mode, the top half of the screen shows yes on green and the bottom half shows no on red, and the screen does not switch. A tap on the yes half speaks yes, and a tap on the no half speaks no. A person picks the mode on the settings screen.

The yes screen gets a check mark icon, and the no screen gets an X icon. A setting on the settings screen shows or hides the icons. In cycle mode, the icon sits above the word, as in a typical AAC app. In split mode, the icon sits on the same line as the word.

When a tap speaks a word, the tapped screen or half swaps its background color and its text color while the word plays. The colors go back when the word ends.

## Hardware

The app runs on the Waveshare ESP32-S3-Touch-AMOLED-2.06. This board has a 410 by 502 AMOLED touch screen, an ES8311 audio codec, and a speaker.

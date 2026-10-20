# Mission

Yes-no is a simple AAC app on a wrist watch. It lets a person who cannot speak answer yes or no questions out loud.

## Who uses it

A non-speaking person wears the watch. People who talk with that person hear the answer from the watch speaker.

## What it does

The watch shows one of two screens. The yes screen has a green background with the word "Yes" in large white letters. The no screen has a red background with the word "No" in large white letters. Each word is in title case and sits in the center of the screen, both horizontally and vertically.

The screens switch every 5 seconds. When the user taps the screen, the watch speaks the word on that screen. The screen stays put while the word plays. After the word ends, the screen stays for a full 5 seconds before the next switch.

The screen turns off after 2 minutes with no tap. A tap on the dark screen turns it back on at the yes screen and does not speak. A tap on the lit screen restarts the 2 minute count.

## Hardware

The app runs on the Waveshare ESP32-S3-Touch-AMOLED-2.06. This board has a 410 by 502 AMOLED touch screen, an ES8311 audio codec, and a speaker.

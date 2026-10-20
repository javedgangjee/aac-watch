# aac-watch

aac-watch is a simple AAC app for a wrist watch. 

The watch switches between a green screen that says "Yes" and a red screen that says "No". When the person taps the screen, the watch speaks the word on that screen.

## Hardware

The app runs on the Waveshare ESP32-S3-Touch-AMOLED-2.06 watch board.

## What the build machine needs

- macOS.
- firmware builds with ESP-IDF v6.1.
- Python 3.13, which it runs as `python3.13`.

The command `say -v '?'` lists the voices on your Mac. If a voice is missing, add it in System Settings under Accessibility, Spoken Content.

The build fails if Python 3.13 or `say` is missing.

## Build and flash

```sh
git clone https://github.com/javedgangjee/aac-watch.git
cd aac-watch
```

Turn on the ESP-IDF v6.1 environment. The path below is where the ESP-IDF installer puts the script by default.

```sh
source ~/.espressif/tools/activate_idf_v6.1.sh
```

Set the target chip and build the firmware. The first build downloads the components from the ESP-IDF component registry.

```sh
idf.py set-target esp32s3
idf.py build
```

Connect the watch to the Mac with a USB cable, and flash the firmware.

```sh
idf.py flash
```

If `idf.py` does not find the watch, give the port, for example `idf.py -p /dev/cu.usbmodem101 flash`.

## Use the watch

- The watch starts at the yes screen. It switches between the yes screen and the no screen every 5 seconds.
- A tap on the screen speaks the word on that screen. The screen stays while the word plays, and then it stays for one more full cycle.
- The screen turns off after 2 minutes with no tap. A tap on the dark screen turns it on at the yes screen and does not speak.
- A swipe down from the top edge of the screen opens the settings screen.

The settings screen has three settings.

- The voice is Samantha or Reed.
- The cycle time goes from 3 to 15 seconds in steps of 1 second. The default is 5 seconds.
- The volume goes from 10 to 100 in steps of 10. The default is 90.

The watch saves the settings and keeps them after a restart. Each of these actions takes you back to the yes and no screens.

- A swipe up from the bottom edge of the screen goes back.
- A tap on Done goes back.
- The watch goes back by itself after 30 seconds with no touch.

## License

Copyright (C) 2026 Javed Gangjee

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.

The full license text is in [LICENSE](LICENSE).

The Yes and No words use the Montserrat Bold font, which is under the [SIL Open Font License](https://github.com/JulietaUla/Montserrat/blob/master/OFL.txt). The settings screen uses the Atkinson Hyperlegible font. The font files and their license are in `tools/fonts/`.

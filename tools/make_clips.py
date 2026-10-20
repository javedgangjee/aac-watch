#!/usr/bin/env python3.13
"""Make the yes and no clips for each voice as raw 16-bit PCM with the macOS say command.

Usage: make_clips.py <output folder>
"""

import logging
import pathlib
import subprocess
import sys
import tempfile
import wave

# The voice key is in the clip file name. The value is the say voice name.
VOICES = {
    "samantha": "Samantha",
    "reed": "Reed (English (US))",
}
RATE = 22050
WORDS = ("yes", "no")

log = logging.getLogger("make_clips")


def fail(message: str) -> None:
    log.error(message)
    sys.exit(1)


def make_clip(word: str, voice: str, temp_dir: pathlib.Path, out_dir: pathlib.Path) -> None:
    wav_path = temp_dir / f"{word}_{voice}.wav"
    try:
        subprocess.run(
            [
                "say",
                "-v", VOICES[voice],
                "-o", str(wav_path),
                "--file-format=WAVE",
                f"--data-format=LEI16@{RATE}",
                word.title(),
            ],
            check=True,
        )
    except FileNotFoundError:
        fail("The say command is missing. The clips must be made on macOS.")
    except subprocess.CalledProcessError as e:
        fail(
            f"The say command failed for {word!r} in the {VOICES[voice]} voice "
            f"with exit code {e.returncode}."
        )

    with wave.open(str(wav_path), "rb") as wav:
        channels = wav.getnchannels()
        width = wav.getsampwidth()
        rate = wav.getframerate()
        if channels != 1 or width != 2 or rate != RATE:
            fail(
                f"{wav_path.name} is {channels} channels, {width * 8}-bit, {rate} Hz. "
                f"It must be mono, 16-bit, {RATE} Hz."
            )
        frames = wav.readframes(wav.getnframes())

    pcm_path = out_dir / f"{word}_{voice}.pcm"
    pcm_path.write_bytes(frames)
    log.info("Wrote %s with %d bytes.", pcm_path, len(frames))


def main() -> None:
    logging.basicConfig(level=logging.INFO, format="make_clips: %(message)s")
    if len(sys.argv) != 2:
        fail("The usage is make_clips.py <output folder>.")
    out_dir = pathlib.Path(sys.argv[1])
    out_dir.mkdir(parents=True, exist_ok=True)

    with tempfile.TemporaryDirectory() as temp:
        for voice in VOICES:
            for word in WORDS:
                make_clip(word, voice, pathlib.Path(temp), out_dir)


if __name__ == "__main__":
    main()

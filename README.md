# 🔴 Button Smasher

A 2-player reaction/speed game built with Arduino, a 16x2 LCD, two LEDs, and two push buttons. Whoever smashes their button the most times within 15 seconds wins!

## Gameplay

1. The LCD shows an idle screen alternating between **"PLYR 1 VS PLYR 2 / CLICK TO START"** and the game logo, with both LEDs blinking.
2. Either player presses their button to begin — a 3-second countdown ("3.. 2.. 1.. GO!!") starts the match.
3. For **15 seconds**, both players smash their buttons as fast as they can. Each press:
   - Increments that player's score
   - Flashes their LED
   - Updates the live score on the LCD
4. When time runs out, the game ends and the LCD shows **"GAME OVER"**.
5. The winner's LED flashes 3 times with a victory message (or "IT'S A TIE" if scores match), then the game resets to the idle screen.

## Features

- 🕹️ Simultaneous 2-player input handling
- ⏱️ 15-second countdown timer displayed live on the LCD
- 💡 LED feedback on every button press
- 🏆 Automatic win/lose/tie detection at the end of each round
- 🔁 Debounced button reads with a busy-wait release check to avoid double-counting a single press

## Hardware Requirements

| Component | Qty | Notes |
|---|---|---|
| Arduino board (Uno/Nano/etc.) | 1 | |
| 16x2 LCD (HD44780-compatible) | 1 | Connected in 4-bit mode |
| Push buttons | 2 | Wired active-LOW (pressed = LOW) |
| LEDs | 2 | One per player |
| Resistors for LEDs | 2 | ~220Ω recommended |

### Pin Mapping

| Function | Pin |
|---|---|
| LCD RS | 6 |
| LCD E | 7 |
| LCD D4 | 8 |
| LCD D5 | 9 |
| LCD D6 | 10 |
| LCD D7 | 11 |
| Button — Player 1 | 2 |
| Button — Player 2 | 13 |
| LED — Player 1 | 3 |
| LED — Player 2 | 12 |

> ⚠️ Buttons are read as **active-LOW** (`digitalRead(...) == LOW` triggers a press), so wire them with a pull-up (internal or external) so the pin reads HIGH when idle.

## Software Requirements

- [Arduino IDE](https://www.arduino.cc/en/software) or PlatformIO
- [`LiquidCrystal`](https://www.arduino.cc/reference/en/libraries/liquidcrystal/) library (bundled with the Arduino IDE)

## Getting Started

1. Wire up the LCD, buttons, and LEDs according to the pin mapping above.
2. Open the sketch in the Arduino IDE.
3. Select your board and port, then upload.
4. Power on, wait for the idle screen, and press either button to start a round!

## Customization

- `Button_Delay` — debounce window between accepted presses (default `100ms`)
- `GAME_TIME` — round duration in milliseconds (default `15000` = 15s)
- `Print_Delay` — how often the idle screen toggles/blinks (default `2000ms`)

## Possible Improvements

- Add sound feedback (buzzer) on win/press
- Persist high scores across rounds (EEPROM)
- Support best-of-N rounds or adjustable game duration via a menu
- Replace the busy-wait release check in `player_play` with a fully interrupt/non-blocking debounce

## License

This project is open source — feel free to use, modify, and share it.

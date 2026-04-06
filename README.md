# STM32 2-Player Electronic Dice Game 🎲

A competitive, turn-based 2-dice game built with the STM32 Nucleo microcontroller. Features a 16x2 LCD, 14 discrete LEDs structured in a 7-pip pattern, and an 8-bit passive buzzer for retro audio effects.

## Hardware Components
- 1x STM32 Nucleo Board (e.g., F401RE)
- 1x 16x2 Character LCD 
- 1x Passive Buzzer (5V)
- 14x Single LEDs (Red, 5mm) & 220-ohm resistors
- 2x Push buttons (Roll, Reset) & 10k-ohm pull-up resistors

## Rules of the Game

### 1. General Setup
- This is a competitive game designed for exactly 2 players.
- Player 1 always takes the first turn.
- Player 2 always takes the second turn.
- A single match lasts for exactly 2 turns.

### 2. How to Play (Rolling the Dice)
- Wait for the LCD screen to show that it is your turn.
- Press and hold the **"ROLL"** button to spin the dice.
- Release the **"ROLL"** button to lock your result.
- Your base score is the total sum of the two dice.

### 3. The "Doubles" Rule (Special Status)
- If you roll a **1-1** or a **6-6**, you get an **"Auto-Loss"** status.
- If you roll a **2-2, 3-3, 4-4,** or **5-5**, you get an **"Auto-Win"** status.
- **Important:** Player 2 *always* gets to play their turn, no matter what Player 1 rolls.

### 4. Resolving the Match (Win, Lose, or Draw)
After Player 2 finishes their turn, the system decides the winner based on these rules:
- **The Auto-Loss Scenario:** If Player 1 gets an Auto-Loss, Player 2 wins. *Exception:* If Player 2 *also* rolls an Auto-Loss, the match is a **DRAW**.
- **The Auto-Win Scenario:** If Player 1 gets an Auto-Win, Player 1 wins. *Exception:* If Player 2 *also* rolls an Auto-Win, the match is a **DRAW**.
- **Player 2's Special Status:** If Player 1 rolls normally, but Player 2 gets an Auto-Win, Player 2 wins. If Player 2 gets an Auto-Loss, Player 1 wins.
- **Normal Resolution:** If neither player rolls a double, the system compares their total sums. The player with the higher score wins. If both have the same score, the match is a **DRAW**.

### 5. Game Over and New Match
- When the match ends, the system plays the corresponding audio (Win, Lose, Draw) and locks the game.
- The "ROLL" button is completely disabled.
- To play again, you must press the **"RESET"** button to clear the LCD and LEDs, starting a new game.

# Checkers Game

## Description

This Checkers Game project was developed by Mohammad Reza Reihani, an Electrical Engineering student at IUST, as the final project for the Programming Basics course under the guidance of Dr. Ashtiani.

### Project Details

- **Date**: 1/18/2023
- **Season**: Winter
- **Day**: Wednesday

### Project Overview

Before diving into the details, note that the project is compatible with both Clion and DevC++ for ease of access. However, please be aware that the Clion console might not work as the default shell on Windows.

### Instructions

1. **Setting up the Game:**
   - Choose the ground size at the start of the game (default is 8x8).
   - The program allows you to pick a custom ground size (e.g., 5x5 for testing purposes).
   - The application handles invalid or missing input gracefully.

2. **Choosing the Starter Side:**
   - Select whether the starting side is BLACK (represented by 'o' and 'O') or WHITE (represented by 'x' and 'X').
   - Use 'w', 'white', or '0' for the white side and 'b', 'black', or '1' for the black side.

3. **Gameplay:**
   - Commands are not case-sensitive.
   - Make moves via the command line in the format: `%Coordinates% to %Coordinates%` (e.g., 3a to 4b, 2c to 1b).
   - Pieces can move diagonally and straight forward; kings can also move backward.
  
4. **Special Commands:**
   - **Redo:** Enter 'redo' to allow a move to be chosen again.
   - **Reset:** Completely restart the game.
   - **Report:** Display a report of the current game, including removed pieces, total pieces, and moves made.
   - **Exit:** Exit the game.

5. **Additional Features:**
   - Each round displays available pieces and possible moves for the current player.
   - Robust error handling notifies the player of incorrect inputs or moves.

### Specific Scenarios File

For specific scenarios, refer to the file named `specificScenarios.txt`.

## Source Code Customization

If you have access to the source code, you can customize the game appearance by modifying values in the `consts.h` file.

### Customize Piece Appearance

- Modify these values to change the appearance of pieces for each side:
  ```cpp
  const char WHITE_C = 'x';       // White character when printing play ground
  const char WHITE_KING_C = 'X';  // White king character when printing play ground
  const char BLACK_C = 'o';       // Black character when printing play ground
  const char BLACK_KING_C = 'O';  // Black king character when printing play ground
  ```

### Customize Console Colors

- Modify these values to customize console colors:
  ```cpp
  const int DEFAULT_COLOR = 7;
  const int WHITE_SIDE_COLOR = 10;
  const int BLACK_SIDE_COLOR = 4;
  const int DIVIDER_COLOR = 1;
  const int SHOW_INFO_COLOR = 6;
  const int REQ_INFO_COLOR = 3;
  const int GROUND_COLOR = 5;
  const int NOTICE_COLOR = 9;
  ```
  - Note: White = 7, Blue = 1, Red = 4, Yellow = 6, Aqua = 3, Purple = 5, Light Blue = 9.

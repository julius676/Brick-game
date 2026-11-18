In the context of the POO course in EPFL, @Maurits33 and me made Brick Game in C++. 
The game works by controlling a paddle with your mouse. The goal of the game is to break all of the bricks.
The game implements ball collisions and bricks use polymorphism by breaking differently by the type of the brick.

<img width="1920" height="1205" alt="image" src="https://github.com/user-attachments/assets/849920e0-06c5-45f5-9661-d19a9379a2ec" />

## Build and run

Requirements: g++ with C++17, pkg-config, gtkmm 4.

```bash
# Ubuntu / Debian
sudo apt install build-essential pkg-config libgtkmm-4.0-dev

# macOS (Homebrew)
brew install gtkmm4 pkg-config
```

```bash
cd Brick_Game
make
./project
```

Pick a level in the menu on the left (`test16.txt` is a good one), press **S** to start,
and **right-click** to spawn a new ball when you run out.

`make tests` opens the game on every level file in `tests/`, one after another.
`make clean` removes the build files.

# AetherTrace: A 2D Light-Based Exploration Puzzle Game

## Overview
AetherTrace is a unique 2D exploration-puzzle game built in C++ using the SFML library. Players navigate a procedurally generated maze by placing and redirecting light beams to reveal hidden paths and unlock portals. The game combines exploration with light-based puzzle mechanics, offering a fresh take on maze navigation and environmental interaction.

This open-source project is designed for developers, gamers, and educators interested in C++ game development, procedural generation, and innovative puzzle mechanics. By sponsoring AetherTrace via GitHub Sponsors, you support new features, maze designs, and educational resources for the community.

## Features
- **Light Manipulation**: Place and rotate mirrors to redirect light beams, revealing hidden paths.
- **Procedural Mazes**: Each level generates a unique maze for endless replayability.
- **Minimalist Aesthetics**: Clean 2D visuals with glowing light effects.
- **Cross-Platform**: Runs on Windows, macOS, and Linux via SFML.
- **Modular Codebase**: Well-structured C++ code for easy extension and learning.

## Getting Started

### Prerequisites
- **C++ Compiler**: GCC, Clang, or MSVC with C++17 support.
- **SFML Library**: Version 2.5.1 or later (install via [SFML's official site](https://www.sfml-dev.org/) or package manager).
- **CMake**: Version 3.10 or later for building.

### Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/aethertrace.git
   cd aethertrace
   ```
2. Create a build directory:
   ```bash
   mkdir build && cd build
   ```
3. Configure with CMake:
   ```bash
   cmake ..
   ```
4. Build the project:
   ```bash
   cmake --build .
   ```
5. Run the game:
   ```bash
   ./AetherTrace
   ```

## Gameplay
- **Objective**: Navigate the player to the portal by using light beams to reveal hidden paths.
- **Controls**:
  - **WASD**: Move the player.
  - **Arrow Keys**: Move the cursor to place mirrors.
  - **Space**: Place or rotate a mirror.
  - **R**: Reset the level.
  - **Esc**: Pause or exit.
- **Mechanics**: Light beams reveal hidden walls and activate portals. Place mirrors strategically to direct beams and clear the maze.

## Project Structure
- `src/main.cpp`: Game entry point and main loop.
- `src/Entity.hpp`: Base class for game entities (player, mirrors, portals).
- `src/Game.hpp`: Core game logic, maze generation, and rendering.
- `assets/`: Placeholder for textures/fonts (add your own or use SFML defaults).

## Contributing
Contributions are welcome! Fork the repo, create a feature branch, and submit a pull request. For major changes, open an issue to discuss your ideas.

## Sponsorship
Support AetherTrace’s development through [GitHub Sponsors](https://github.com/sponsors/yourusername)! Your contributions fund:
- New maze generation algorithms and puzzle elements.
- Enhanced light physics and visual effects.
- Tutorials for C++ and procedural generation.
- Optimizations for broader platform support.

## License
Licensed under the MIT License. See [LICENSE](LICENSE) for details.

## Acknowledgments
- Built with [SFML](https://www.sfml-dev.org/) for graphics and input.
- Inspired by light-based puzzles and exploration games.
- Thanks to the open-source community for feedback and inspiration.

---

**Sponsor AetherTrace to illuminate the future of puzzle gaming!**
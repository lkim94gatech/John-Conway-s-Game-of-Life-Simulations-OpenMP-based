# John Conway's Game of Life Simulation

This project implements **John Conway's Game of Life** using **C++**, featuring sequential, multithreaded, and OpenMP-based parallel processing. The program visually simulates the evolution of cells in a 2D grid, applying Conway's rules in each generation.

---

## Objective

To understand and apply the principles of **multithreading** using `std::thread` and **OpenMP** for parallel computations in a computationally intensive problem.

---

## Features

1. **Game of Life Rules**:
   - **Alive Cells**: Stay alive with 2 or 3 live neighbors.
   - **Dead Cells**: Spring to life with exactly 3 live neighbors.
   - **Infinite 2D Grid**: Simulated using edge wrapping.

2. **Processing Modes**:
   - **Sequential Mode**: Processes the grid in a single thread.
   - **Threaded Mode**: Splits the grid among multiple `std::thread` instances.
   - **OpenMP Mode**: Parallelizes grid processing using OpenMP.

3. **Randomized Initialization**:
   - Grid cells are initialized to either "alive" or "dead" using a random number generator.

4. **Console Output**:
   - Reports processing time every 100 generations for each mode.

5. **Visual Output**:
   - **Alive Cells**: Displayed as white squares.
   - **Dead Cells**: Left undrawn on a black background.
   - Continuous updates of generations in a window.

---

## File Structure

### Source File

- **lab2.cpp**: Contains the full implementation of the Game of Life simulation, including command-line argument parsing and processing modes.

---

## How to Run

### Prerequisites

- **SFML Library**: Install SFML for 2D graphics support.
- **C++ Compiler**: Requires a compiler with C++11 or later support (e.g., GCC, Clang).

### Command-Line Usage

The program accepts the following command-line arguments:

```bash
./Lab2 -n <num_threads> -c <cell_size> -x <window_width> -y <window_height> -t <processing_type>
```
- -n: Number of threads (ignored in SEQ mode, defaults to 8).
- -c: Cell size in pixels (defaults to 5).
- -x: Window width (defaults to 800).
- -y: Window height (defaults to 600).
- -t: Processing type (SEQ, THRD, OMP; defaults to THRD).

##Compilation
1. Clone the respository and navigate to the project directory.
2. Compile the code with the SFML library
3. Run the program
   ```bash
   ./Lab2 -n <num_threads> -c <cell_size> -x <window_width> -y <window_height> -t <processing_type>
   ```
## Gameplay Instructions

- The simulation runs continuously until the **Escape** key is pressed or the window is closed.
- **Alive cells** are displayed as white, while **dead cells** are left undrawn (black background).
- Processing times are printed to the console every 100 generations.

---

## Testing and Debugging

### Key Features to Test

1. **Command-Line Arguments**:
   - Validate that default values are used when arguments are missing.
   - Test all valid combinations of command-line arguments.

2. **Processing Modes**:
   - Verify the accuracy of output and measure processing times for `SEQ`, `THRD`, and `OMP` modes.

3. **Grid Initialization**:
   - Ensure proper randomization of cell states at the start of the simulation.

4. **Generational Updates**:
   - Test the implementation of Conway's rules for accuracy in all processing modes.

5. **Visual Output**:
   - Check the display for correct representation of **alive** and **dead** cells.

6. **Performance**:
   - Compare processing times between sequential (`SEQ`), multithreaded (`THRD`), and OpenMP (`OMP`) modes.

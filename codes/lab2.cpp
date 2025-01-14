/**
 * Leandro Alan Kim
 * Class: ECE 4122 A
 * Last Modified: 7 Oct 2024
 * 
 * Description: The purpose of this assignment is to apply the principles of multithreading using thread and OpenMP for parallel calculations
 * by playing the John Conway's Game of Life.
 */

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>
#include <random>
#include <chrono>
#include <omp.h>

using namespace sf;
using namespace std;


int WINDOW_WIDTH = 800; // default window dimension
int WINDOW_HEIGHT = 600; // default window dimension
int PIXEL_SIZE = 5; // default cell size
int numThreads = 8; // default number of threads
string PROCESS_TYPE = "THRD"; // default processing type

// grid width will be based on window width and pixel size
const int GRID_WIDTH()
{
    return WINDOW_WIDTH / PIXEL_SIZE;
}

// grid height will be based on window height and pixel size
const int GRID_HEIGHT()
{
    return WINDOW_HEIGHT / PIXEL_SIZE;
}

/**
 * command line arguments
 * @param argc number of command line args
 * @param argv string array that represents the command line arguments
 */
void arg(int argc, char* argv[]) 
{
    for (int i = 1; i < argc; ++i) // also checking to see if next arg is there
    {
        string arg = argv[i];
        if (arg == "-n") 
        {
            numThreads = stoi(argv[++i]);
            if (numThreads < 2)
            {
                numThreads = 8; // if number of threads is less than 2 then default
            }
        } 
        else if (arg == "-c") 
        {
            PIXEL_SIZE = stoi(argv[++i]);
            if (PIXEL_SIZE < 1)
            {
                PIXEL_SIZE = 5; // if pixel size is less than 1 than defualt
            }
        } 
        else if (arg == "-x") 
        {
            WINDOW_WIDTH = stoi(argv[++i]);
        } 
        else if (arg == "-y" && i + 1 < argc) // also checking to see if next arg is there
        {
            WINDOW_HEIGHT = stoi(argv[++i]);
        } 
        else if (arg == "-t" && i + 1 < argc) // also checking to see if next arg is there
        {
            PROCESS_TYPE = argv[++i];
            if (PROCESS_TYPE != "SEQ" && PROCESS_TYPE != "THRD" && PROCESS_TYPE != "OMP") 
            {
                PROCESS_TYPE = "THRD"; // if no arg for process type then default
            }
        }
    }
}

/**
 * function to randomly give bool values to the grid.
 * each cell will be randomly selected to be true = alive or false = dead
 * @param grid vector that represents if the cells are alive or dead
 */
void randGrid(vector<vector<bool>>& grid) 
{
    random_device rd; // random number generator
    mt19937 gen(rd()); // mersenne twister random num generator
    uniform_int_distribution<> dis(0, 1); // random num set to 0 or 1

    for (int x = 0; x < GRID_WIDTH(); ++x) // looping through the width
    {
        for (int y = 0; y < GRID_HEIGHT(); ++y) // looping throught the height
        {
            grid[x][y] = static_cast<bool>(dis(gen)); // set alive or dead
        }
    }
}

/**
 * function to count how many neighboring cells are alive at the cell position.
 * neighbors are counted wiht mod to wrap around the edges
 * @param grid vector that represents if the cells are alive or dead
 * @param x neighbor's alive cell at x position
 * @param y neighbor's alive cell at y position
 * @return count the total number of cells that are neighboring that is alive
 */
int countNeighbors(const vector<vector<bool>>& grid, int x, int y) 
{
    int count = 0; // counter for alive neighbors
    for (int i = -1; i <= 1; ++i) // this will only see neighbors over 3x3 grid
    {
        for (int j = -1; j <= 1; ++j)
        {
            if (i == 0 && j == 0) // condition to not count the actual cell
            {
                continue;
            }
            int nx = (x + i + GRID_WIDTH()) % GRID_WIDTH(); // calculating the neightbor's x position
            int ny = (y + j + GRID_HEIGHT()) % GRID_HEIGHT(); // calculating the neighrbor's y position
            count += grid[nx][ny]; // inc counter if neighbor is alive
        }
    }
    return count;
}

/**
 * function to update the next generation. cell's state is based on how many alive neighbors there are
 * @param grid vector that represents if the cells are alive or dead
 * @param nextGrid stores the next state of the grid
 */
void seq(vector<vector<bool>>& currGrid, vector<vector<bool>>& nextGrid) 
{
    for (int x = 0; x < GRID_WIDTH(); ++x) // looping through the x coord
    {
        for (int y = 0; y < GRID_HEIGHT(); ++y) // looping through the y coord
        {
            int neighbors = countNeighbors(currGrid, x, y); // num of neighbors that are alive

            if (currGrid[x][y]) 
            {
                if (neighbors < 2 || neighbors > 3)
                {
                    nextGrid[x][y] = false; // dies if there are less than 2 neighbors or more than 3
                }
                else
                {
                    nextGrid[x][y] = true; // survives if there are 2 or 3 neighbors
                }
            }
            else 
            {
                if (neighbors == 3)
                {
                    nextGrid[x][y] = true; // becomes alive if it has 3 neighbors
                }
                else
                {
                    nextGrid[x][y] = false; // stays dead
                }
            }
        }
    }
}

/**
 * function to process portions of the grid in parallel. each thread will be seperated to a subset for next generation.
 * @param start starting x coord
 * @param end ending x coord
 * @param currGrid current grid to be looked at
 * @param nextGrid next state that will be stored
 */
void thrd(int start, int end, const vector<vector<bool>>& currGrid, vector<vector<bool>>& nextGrid)
{
    for (int x = start; x < end; ++x) // loop through the start and end
    {
        for (int y = 0; y < GRID_HEIGHT(); ++y) // loop through the y coord
        {
            int neighbors = countNeighbors(currGrid, x, y); // number of alive neighbors

            if (currGrid[x][y]) 
            {
                if (neighbors < 2 || neighbors > 3)
                {
                    nextGrid[x][y] = false; // dies if there are less than 2 neighbors or more than 3
                }
                else
                {
                    nextGrid[x][y] = true; // survives if there are 2 or 3 neighbors
                }
            }
            else 
            {
                if (neighbors == 3)
                {
                    nextGrid[x][y] = true; // becomes alive if it has 3 neighbors
                }
                else
                {
                    nextGrid[x][y] = false; // stays dead
                }
            }
        }
    }
}

/**
 * function to use multiple threads in parallel. breaks down the grid and process each portions
 * @param currGrid current grid to be looked at
 * @param nextGrid next state that will be stored
 * @param numThreads number of threads to use in parallel
 */
void multiThread(vector<vector<bool>>& currGrid, vector<vector<bool>>& nextGrid, int numThreads)
{
    vector<thread> threads; // holds threads
    int blockSize = GRID_WIDTH() / numThreads; // num of rows on each thread

    for (int i = 0; i < numThreads; ++i) 
    {
        int start = i * blockSize; // starting row
        int end = (i == numThreads - 1) ? GRID_WIDTH() : (i + 1) * blockSize; // ending row
        threads.push_back(thread(thrd, start, end, ref(currGrid), ref(nextGrid))); // create thread and add to vector
    }
    for (auto& thread : threads)
    {
        thread.join(); // joins all threads
    }
}

/**
 * function to use OpenMP in parallel. OpenMP diveds the grid and we process each thread
 * @param currGrid current grid to be looked at
 * @param nextGrid next state that will be stored
 */
void openMP(vector<vector<bool>>& currGrid, vector<vector<bool>>& nextGrid)
{
    #pragma omp parallel for // make outer loop parallel
    for (int x = 0; x < GRID_WIDTH(); ++x) // loop through the x coord
    {
        for (int y = 0; y < GRID_HEIGHT(); ++y) // loop through the y coord
        {
            int neighbors = countNeighbors(currGrid, x, y); // num of neighbors that are alive

            if (currGrid[x][y]) 
            {
                if (neighbors < 2 || neighbors > 3)
                {
                    nextGrid[x][y] = false; // dies if there are less than 2 neighbors or more than 3
                }
                else
                {
                    nextGrid[x][y] = true; // survives if there are 2 or 3 neighbors
                }
            }
            else 
            {
                if (neighbors == 3)
                {
                    nextGrid[x][y] = true; // becomes alive if it has 3 neighbors
                }
                else
                {
                    nextGrid[x][y] = false; // stays dead
                }
            }
        }
    }
}

/**
 * main function to initialize the game. handles input and makes the game window
 * @param argc number of command line args
 * @param argv string array that represents the command line arguments
 */
int main(int argc, char* argv[]) 
{
    arg(argc, argv); // get the command line args

    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Game of Life"); // game window
    window.setFramerateLimit(60); // set frame rate to control speed

    vector<vector<bool>> currGrid(GRID_WIDTH(), vector<bool>(GRID_HEIGHT(), false)); // create vector for current grid
    vector<vector<bool>> nextGrid(GRID_WIDTH(), vector<bool>(GRID_HEIGHT(), false)); // create vector for next generation

    vector<vector<bool>>& refCurrent = currGrid, &refNext = nextGrid; // reference to curr and next grid

    randGrid(currGrid); // calling randGrid to initialize the grid with rand values

    using clock = chrono::high_resolution_clock;
    auto timeStart = clock::now(); // keep track of the time
    unsigned long numGenerations = 0; // counter for number of generations
    long long totalTime = 0; // records total time

    // main loop for the game
    while (window.isOpen()) 
    {
        Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == Event::Closed || 
               (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape))
            {
                window.close(); // closes window when escape key is pressed
            }
        }

        auto genStartTime = clock::now(); // start tracking time

        // process the grids according to the args passed in
        if (PROCESS_TYPE == "SEQ")
        {
            seq(refCurrent, refNext);
        }
        else if (PROCESS_TYPE == "THRD")
        {
            multiThread(refCurrent, refNext, numThreads);
        }
        else if (PROCESS_TYPE == "OMP")
        {
            openMP(refCurrent, refNext);
        }

        auto genEndTime = clock::now(); // record end time of generation
        long long genDuration = chrono::duration_cast<chrono::microseconds>(genEndTime - genStartTime).count();

        totalTime += genDuration; // add total time
        numGenerations++; // counter for number of generations passed

        refCurrent.swap(refNext); // to next generation

        // print every 100 generation
        if (numGenerations % 100 == 0)
        {
            cout << "100 generations took " << totalTime << " microseconds with ";

            if (PROCESS_TYPE == "SEQ") 
            {
                cout << "single thread." << endl;
            } 
            else if (PROCESS_TYPE == "THRD") 
            {
                cout << numThreads << " std::threads." << endl;
            }
            else if (PROCESS_TYPE == "OMP")
            {
	      cout << "OMP." << endl;
            }
            totalTime = 0; // reset
        }

        window.clear();

        // looping through the grid to draw the cells that are alive
        for (int x = 0; x < GRID_WIDTH(); ++x) 
        {
            for (int y = 0; y < GRID_HEIGHT(); ++y) 
            {
                if (refCurrent[x][y])
                {
                    RectangleShape cell(Vector2f(PIXEL_SIZE, PIXEL_SIZE));
                    cell.setPosition(x * PIXEL_SIZE, y * PIXEL_SIZE);
                    cell.setFillColor(Color::White);
                    window.draw(cell);
                }
            }
        }

        window.display();
    }

    return 0;
}

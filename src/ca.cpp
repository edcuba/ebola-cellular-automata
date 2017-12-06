#include "ca.h"
#include "bitmap_image.hpp"
#include <cstdlib>

/**
 * Create matrix for a single generation
 */
Cell **
CA::allocateMatrix ()
{
    Cell **m = new Cell *[rows];
    for (int i = 0; i < rows; ++i) {
        m[i] = new Cell[columns];
    }
    return m;
}

CA::~CA ()
{
    for (auto g : generations) {
        for (int r = 0; r < rows; ++r) {
            delete[] g[r];
        }
        delete[] g;
    }
}

/**
 * Initialize cellular automaton object and allocate matrix for the first generation
 */
CA::CA (int rows, int columns, double longProb, double deadProb, double terminalState)
    : columns (columns)
    , rows (rows)
    , longProb (longProb)
    , deadProb (deadProb)
    , terminalState (terminalState)
{
    generation = allocateMatrix ();
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            generation[i][j] = HEALTHY;
        }
    }
    generations.push_back (generation);
    numHealthy = 0;
    numDead = 0;
    numInfected = 0;
}

/**
 * Get count of infected neighbours in 3x3 grid with center at [row, column]
 */
int
CA::infectedNeighbours (int row, int column)
{
    int positive = 0;
    for (int r = row - 1; r <= row + 1; ++r) {
        int x = (r < 0) ? rows + r : r % rows;
        for (int c = column - 1; c <= column + 1; ++c) {
            int y = (c < 0) ? columns + c : c % columns;
            Cell val = generation[x][y];
            if (val == INFECTED || val == SPOILED) {
                positive++;
            }
        }
    }
    return positive;
}

/**
 * Get value of particular cell in the next generation
 */
Cell
CA::nextState (int row, int column, bool regenerate, bool delay)
{
    Cell val = generation[row][column];

    switch (val) {
        case DEAD:
            numDead++;
            val = regenerate ? HEALTHY : DEAD;
            break;
        case SPOILED:
            numInfected++;
            val = DEAD;
            break;
        case INFECTED:
            numInfected++;
            val = delay ? SPOILED : DEAD;
            break;
        case HEALTHY:
            numHealthy++;
            val = infectedNeighbours (row, column) > 0 ? INFECTED : HEALTHY;
            break;
    }

    return val;
}

/**
 * Perform a step creating a new generation according to input parameters
 */
void
CA::step (bool regenerate, bool delay)
{
    counter++;

    Cell **m = allocateMatrix ();
    numDead = 0;
    numHealthy = 0;
    numInfected = 0;

    // get state in next generation for every cell in the matrix
    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            m[row][column] = nextState (row, column, regenerate, delay);
        }
    }

    generations.push_back (m);
    generation = m;
}

/**
 * Perform a random step due to step probabilities
 */
void
CA::randomStep ()
{
    int choice = rand () % 100;
    if (choice < longProb * 100) {
        // long step
        step (true, true);
    } else if (choice < (longProb + deadProb) * 100) {
        // dead stays dead;
        step (false, false);
    } else {
        // healthy
        step (true, false);
    }
}

/**
 * Randomly seed @amount of infected cells into matrix
 */
void
CA::seed (int amount)
{
    for (int i = 0; i < amount; ++i) {
        size_t x = rand () % rows;
        size_t y = rand () % columns;
        // mark random cell as infected
        generation[x][y] = INFECTED;
    }
}

/**
 * Get the status of current generation
 * Result can be DEAD, INFECTED or HEALTHY
 *  - DEAD: terminating condition has been met
 *  - INFECTED: subject is alive, but there are still some infected cells
 *  - HEALTHY: subject is alive and there are no infected cells
 */
Cell
CA::status ()
{
    if (counter > 31) {
        return DEAD;
    }

    long infected = 0;
    long alive = 0;
    for (int x = 0; x < rows; ++x) {
        for (int y = 0; y < columns; ++y) {
            Cell c = generation[x][y];
            if (c == INFECTED || c == SPOILED)
                infected++;
            else if (c == HEALTHY)
                alive++;
        }
    }

    if (alive < rows * columns * terminalState) {
        return DEAD;
    }

    return infected > 0 ? INFECTED : HEALTHY;
}

/**
 * Generate BMP image of current generation
 */
void
CA::saveToFile (std::string filename)
{
    bitmap_image image (columns, rows);

    // set background to white
    image.set_all_channels (255, 255, 255);

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < columns; ++x) {
            switch (generation[y][x]) {
                case HEALTHY:
                    image.set_pixel (x, y, 39, 117, 84);
                    break;
                case INFECTED:
                case SPOILED:
                    image.set_pixel (x, y, 170, 57, 57);
                    break;
                case DEAD:
                    image.set_pixel (x, y, 0, 0, 0);
                    break;
            }
        }
    }

    image.save_image (filename);
}

int
CA::getDays () const
{
    return counter;
}

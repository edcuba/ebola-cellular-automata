#include "ca.h"
#include "bitmap_image.hpp"
#include <cstdlib>

Cell **
CA::allocateMatrix ()
{
    Cell **m = new Cell *[rows];
    for (size_t i = 0; i < rows; ++i) {
        m[i] = new Cell[columns];
        for (size_t j = 0; j < columns; ++j) {
            m[i][j] = HEALTHY;
        }
    }
    return m;
}

CA::~CA ()
{
    for (auto g : generations) {
        for (size_t r = 0; r < rows; ++r) {
            delete[] g[r];
        }
        delete[] g;
    }
}

CA::CA (size_t rows, size_t columns, double longProb, double deadProb, double terminalState)
    : columns (columns)
    , rows (rows)
    , longProb (longProb)
    , deadProb (deadProb)
    , terminalState (terminalState)
{
    generation = allocateMatrix ();
    numHealthy = 0;
    numDead = 0;
    numInfected = 0;
}

int
CA::infectedNeighbours (int row, int column)
{
    int positive = 0;
    int rw = rows;
    int cl = columns;
    for (int r = row - 1; r <= row + 1; ++r) {
        int x = r;
        if (x == -1) {
            x = rows - 1;
        } else if (x == rw) {
            x = 0;
        }
        for (int c = column - 1; c <= column + 1; ++c) {
            int y = c;
            if (y == -1) {
                y = columns - 1;
            } else if (y == cl) {
                y = 0;
            }
            Cell val = generation[x][y];
            if (val == INFECTED || val == SPOILED) {
                positive++;
            }
        }
    }
    return positive;
}

Cell
CA::nextState (size_t row, size_t column, bool regenerate, bool delay)
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

void
CA::step (bool regenerate, bool delay)
{
    counter++;

    Cell **m = allocateMatrix ();
    numDead = 0;
    numHealthy = 0;
    numInfected = 0;

    // get state in next generation for every cell in the matrix
    for (size_t row = 0; row < rows; ++row) {
        for (size_t column = 0; column < columns; ++column) {
            m[row][column] = nextState (row, column, regenerate, delay);
        }
    }

    generations.push_back (m);
    generation = m;
}

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

Cell
CA::status ()
{
    if (counter > 31) {
        return DEAD;
    }

    long infected = 0;
    long alive = 0;
    for (size_t x = 0; x < rows; ++x) {
        for (size_t y = 0; y < columns; ++y) {
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

void
CA::saveToFile (std::string filename)
{
    bitmap_image image (columns, rows);

    // set background to white
    image.set_all_channels (255, 255, 255);

    for (std::size_t y = 0; y < rows; ++y) {
        for (std::size_t x = 0; x < columns; ++x) {
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

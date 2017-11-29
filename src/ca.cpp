#include "ca.h"
#include <cstdlib>
#include <ctime>

CA::CA (size_t rows, size_t columns, double longProb, double deadProb)
    : columns (columns)
    , rows (rows)
    , longProb (longProb)
    , deadProb (deadProb)
    , generation (rows, columns)
{
    // initialize the random generator
    srand (time (NULL));
}

int
CA::infectedNeighbours (int row, int column)
{
    int positive = 0;
    for (int r = row - 1; r <= row + 1; ++r) {
        for (int c = column - 1; c <= column + 1; ++c) {
            Cell val = generation[r][c];
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
            val = regenerate ? HEALTHY : DEAD;
            break;
        case SPOILED:
            val = DEAD;
            break;
        case INFECTED:
            val = delay ? SPOILED : DEAD;
            break;
        case HEALTHY:
            val = infectedNeighbours (row, column) > 0 ? INFECTED : HEALTHY;
            break;
    }
    return val;
}

void
CA::step (bool regenerate, bool delay)
{
    counter++;
    Matrix m (rows, columns);

    // get state in next generation for every cell in the matrix
    for (size_t row = 0; row < rows; ++row) {
        for (size_t column = 0; column < columns; ++column) {
            m[row][column] = nextState (row, column, regenerate, delay);
        }
    }

    generations.push_back (generation);
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

std::string
CA::dump ()
{
    std::string s;
    s += "-- Generation ";
    s += std::to_string (counter);
    s += " --\n";
    return s + generation.dump ();
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

bool
CA::healthy ()
{
    for (auto &r : generation) {
        for (auto c : r) {
            if (c != HEALTHY)
                return false;
        }
    }
    return true;
}

bool
CA::dead ()
{
    for (auto &r : generation) {
        for (auto c : r) {
            if (c != DEAD)
                return false;
        }
    }
    return true;
}

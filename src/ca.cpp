#include "ca.h"

CA::CA (size_t rows, size_t columns)
    : generation (rows, columns)
    , columns (columns)
    , rows (rows)
{
}

void
CA::step (bool regenerate, bool delay)
{
    counter++;
    Matrix m (columns, rows);

    // TODO perform a step

    generations.push_back (generation);
    generation = m;
}

void
CA::randomStep ()
{
    // TODO randomly choose configuration of a step
    step (false, false);
}

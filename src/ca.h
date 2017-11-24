#ifndef CA_H
#define CA_H

#include "cell.h"
#include "cstdlib"
#include "matrix.h"
#include <vector>

class CA
{
  protected:
    std::vector<Matrix> generations;
    Matrix generation;
    int counter = 0;

  public:
    const size_t columns;
    const size_t rows;
    CA (size_t rows, size_t columns);

    void step (bool regenerate, bool delay);
    void randomStep ();
};

#endif

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

  public:
    const size_t columns;
    const size_t rows;
    CA (size_t rows, size_t columns);
};

#endif

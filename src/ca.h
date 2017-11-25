#ifndef CA_H
#define CA_H

#include "cell.h"
#include "cstdlib"
#include "matrix.h"
#include <string>
#include <vector>

class CA
{
  protected:
    std::vector<Matrix> generations;
    Matrix generation;
    int counter = 0;
    Cell nextState (size_t row, size_t column, bool regenerate, bool delay);
    int infectedNeighbours (int row, int column);

  public:
    const size_t columns;
    const size_t rows;
    const int regenerationFactor;
    const int delayFactor;
    CA (size_t rows, size_t columns, int regenerationFactor, int delayFactor);

    void step (bool regenerate, bool delay);
    void randomStep ();
    std::string dump ();
    void seed (int amount);
    bool healthy ();
    bool dead ();
};

#endif

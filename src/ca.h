#ifndef CA_H
#define CA_H

#include "cell.h"
#include "cstdlib"
#include "matrix.h"
#include <string>
#include <vector>

class CA
{
  public:
    const size_t columns;
    const size_t rows;
    const double longProb;
    const double deadProb;
    const double terminalState;
    CA (size_t rows, size_t columns, double longProb, double deadProb, double terminalState);

    void step (bool regenerate, bool delay);
    void randomStep ();
    std::string dump ();
    void seed (int amount);
    bool healthy ();
    bool dead ();

  protected:
    std::vector<Matrix> generations;
    Matrix generation;
    int counter = 0;
    Cell nextState (size_t row, size_t column, bool regenerate, bool delay);
    int infectedNeighbours (int row, int column);
};

#endif

#ifndef CSV_H
#define CSV_H

#include <sstream>
#include <string>
#include <vector>

using namespace std;

class CSV
{
  public:
    CSV (string delimiter);
    void writeHeader (vector<string> columns);
    bool writeRow (vector<string> columns);
    bool writeToFile (string filename);

  private:
    size_t columnsPerRow;
    string delimiter;
    stringstream output;
};

#endif

#include "csv.h"
#include <fstream>

CSV::CSV (string delimiter)
    : delimiter (delimiter)
{
}

/**
 * Write the CSV header
 **/
void
CSV::writeHeader (vector<string> columns)
{
    columnsPerRow = columns.size ();

    for (size_t i = 0; i < columns.size (); i++) {
        output << delimiter;
        output << columns[i];
    }

    output << endl;
}

/**
 * Write a single CSV row to output buffer
 **/
bool
CSV::writeRow (vector<string> columns)
{
    if (columns.size () != columnsPerRow) {
        return false;
    }

    for (size_t i = 0; i < columns.size (); i++) {
        output << delimiter;
        output << columns[i];
    }

    output << endl;

    return true;
}

/**
 * Write content of CSV buffer to a file
 **/
bool
CSV::writeToFile (string filename)
{
    ofstream f;
    f.open (filename);

    if (!f.is_open ()) {
        return false;
    }

    f << output.rdbuf ();

    f.close ();

    return true;
}

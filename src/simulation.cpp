#include "simulation.h"
#include "csv.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <unistd.h>

using namespace std;

int
main (int argc, char **argv)
{
    // defaults
    double longProb = 0;
    double deadProb = 0;
    double seedProb = 0.0055;
    int rows = 700;
    int columns = 700;
    srand (time (NULL));

    // having less than of cells 29% healthy means death
    double terminalState = 0.29;

    // parse command line configuration
    int c;
    while ((c = getopt (argc, argv, "l:d:r:c:s:t:")) != -1) {
        switch (c) {
            case 'l':
                longProb = strtod (optarg, NULL);
                break;
            case 'd':
                deadProb = strtod (optarg, NULL);
                break;
            case 't':
                terminalState = strtod (optarg, NULL);
                break;
            case 's':
                seedProb = strtod (optarg, NULL);
                break;
            case 'r':
                rows = strtol (optarg, NULL, 0);
                break;
            case 'c':
                columns = strtol (optarg, NULL, 0);
                break;
        }
    }
    if (1 - longProb - deadProb < 0) {
        cerr << "Error: l + d > 1" << endl;
        return 1;
    }

    // instantiate celluar automaton
    // CSV csv (",");
    // vector<string> header;
    // header.push_back ("time");
    // header.push_back ("healthy");
    // header.push_back ("infected");
    // header.push_back ("dead");
    // csv.writeHeader (header);

    for (int i = 0; i < 100; ++i) {
        CA ca (rows, columns, longProb, deadProb, terminalState);
        // place infected cells
        int seedCount = rows * columns * seedProb;
        ca.seed (seedCount == 0 ? 1 : seedCount);

        // run simulation while CA is not completely dead or empty
        while (ca.status () == INFECTED) {

            // print generation
            // cout << ca.dump () << endl;

            // perform step
            ca.randomStep ();

            // vector<string> row;
            // row.push_back (std::to_string (ca.getDays ()));
            // row.push_back (std::to_string (ca.numHealthy));
            // row.push_back (std::to_string (ca.numInfected));
            // row.push_back (std::to_string (ca.numDead));

            // csv.writeRow (row);

            // usleep (75000);
        }

        // print last generation (dead/alive)
        // cout << ca.dump () << endl;
        // ca.saveToFile ("last_gen.bmp");
        // csv.writeToFile ("simulation.csv");

        if (ca.status () == DEAD) {
            cout << "DEAD: " << ca.getDays () << endl;
        } else {
            cout << "ALIVE: " << ca.getDays () << endl;
        }
    }

    return 0;
}

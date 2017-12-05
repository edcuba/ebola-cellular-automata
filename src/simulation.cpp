#include "simulation.h"
#include "csv.h"
#include <cstdlib>
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

    bool exportCSV = false;
    bool calcFatRate = false;

    // having less than of cells 29% healthy means death
    double terminalState = 0.29;

    // parse command line configuration
    int c;
    while ((c = getopt (argc, argv, "l:d:r:c:s:t:of")) != -1) {
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
            case 'o':
                exportCSV = true;
                break;
            case 'f':
                calcFatRate = true;
                break;
        }
    }
    if (1 - longProb - deadProb < 0) {
        cerr << "Error: l + d > 1" << endl;
        return 1;
    }

    CSV csv (",");
    vector<string> header;
    header.push_back ("time");
    header.push_back ("healthy");
    header.push_back ("infected");
    header.push_back ("dead");
    csv.writeHeader (header);


    int runCount = 1;
    if (calcFatRate) {
        runCount = 100;
    }

    int deadCount = 0;
    int totalCount = 0;

    for (int i = 0; i < runCount; i++) {

        // instantiate celluar automaton
        CA ca (rows, columns, longProb, deadProb, terminalState);

        // place infected cells
        int seedCount = rows * columns * seedProb;
        ca.seed (seedCount == 0 ? 1 : seedCount);

        int time = 0;

        // run simulation while CA is not completely dead or empty
        while (!ca.healthy () && !ca.dead ()) {

            // print generation
            // cout << ca.dump () << endl;

            // perform step
            ca.randomStep ();

            if (exportCSV) {
                vector<string> row;
                row.push_back (std::to_string (time++));
                row.push_back (std::to_string (ca.numHealthy));
                row.push_back (std::to_string (ca.numInfected));
                row.push_back (std::to_string (ca.numDead));
            }

            // csv.writeRow (row);
            // usleep (75000);
        }

        if (ca.dead ()) {
            cout << "DEAD" << endl;
            deadCount++;
        } else {
            cout << "ALIVE" << endl;
        }

        totalCount++;
    }
    
    if (calcFatRate) {
        cout << "Fatality rate is " << (float)deadCount*(float)100/(float)totalCount << " %\n";
    }
    // print last generation (dead/alive)
    // cout << ca.dump () << endl;

    if (exportCSV) {
        csv.writeToFile ("simulation.csv");
    }

    return 0;
}

#include "simulation.h"
#include <iostream>

using namespace std;

int
main (int argc, char const *argv[])
{
    // CA configuration
    int rows = 10;
    int columns = 20;
    int regenerationFactor = 5;
    int delayFactor = 3;

    CA ca (rows, columns, regenerationFactor, delayFactor);
    ca.seed (1);
    while (!ca.healthy () && !ca.dead ()) {
        cout << ca.dump () << endl;
        ca.randomStep (); // stochastic
        // ca.step (true, false);
    }
    cout << ca.dump () << endl;
    return 0;
}

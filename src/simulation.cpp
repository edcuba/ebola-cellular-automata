#include "simulation.h"
#include <iostream>

using namespace std;

int
main (int argc, char const *argv[])
{
    // CA configuration
    int rows = 9;
    int columns = 18;
    int regenerationFactor = 4;
    int delayFactor = 3;

    CA ca (rows, columns, regenerationFactor, delayFactor);
    ca.seed (20);
    while (!ca.healthy () || ca.dead ()) {
        cout << ca.dump () << endl;
        ca.randomStep ();
    }
    cout << ca.dump () << endl;
    return 0;
}

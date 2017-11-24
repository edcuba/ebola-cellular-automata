#include "simulation.h"
#include <iostream>

using namespace std;

int
main (int argc, char const *argv[])
{
    CA ca (10, 10);
    for (int i = 0; i < 5; ++i) {
        ca.randomStep ();
    }
    return 0;
}

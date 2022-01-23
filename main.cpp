#include <iostream>

#include "PrimeMusic.hpp"

#include "NoteOperations.hpp"



int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    PrimeMusic p(3);
    p.findPerfectCycle();

    p.setup(5);
    p.findPerfectCycle();

    p.setup(7);
    p.findPerfectCycle();

    p.setup(11);
    p.findPerfectCycle();

    p.setup(13);
    p.findPerfectCycle();

    p.setup(17);
    p.findPerfectCycle();

    p.setup(19);
    p.findPerfectCycle();

    p.setup(23);
    p.findPerfectCycle();

    p.setup(29);
    p.findPerfectCycle();

    return 0;
}

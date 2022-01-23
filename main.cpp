#include <iostream>

#include "PrimeMusic.hpp"

#include "NoteOperations.hpp"


int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    PrimeMusic p(12, 3);
    p.findPerfectCycle();

    p.setup(12, 5);
    p.findPerfectCycle();

    p.setup(12, 7);
    p.findPerfectCycle();

    return 0;
}

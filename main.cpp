#include <iostream>
#include <set>

#include "PrimeMusic.hpp"
#include "NoteOperations.hpp"


int primeList [] = {3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53,
                    59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109 };


void exploreCyclesAndIntervals()
{
    PrimeMusic p;
    std::set<int> intervalsSet;

    for (size_t i = 0; i < sizeof(primeList); ++i) {
        p.setup(primeList[i]);
        p.findPerfectCycle();

        const auto [interval, _] = p.musicInterval();
        intervalsSet.insert(interval % 12);

        if (intervalsSet.size() == 11) {
            std::cout << "We got full set!" << std::endl;

            for (auto i: intervalsSet)
                std::cout << i << " ";
            std::cout << std::endl;

            break;
        }
    }
}


int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    exploreCyclesAndIntervals();

    return 0;
}

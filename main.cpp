#include <iostream>
#include <set>

#include "PrimeMusic.hpp"
#include "NoteOperations.hpp"

const size_t primesCount = 28;
int primeList [primesCount] = {3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53,
                    59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109 };


void exploreCyclesAndIntervals()
{
    PrimeMusic p;
    std::set<int> intervalsSet;

    //Just a reminded 2 generated all perfect harmonics
    //3 and 5 generated 1-3-5 major intervals

    for (size_t i = 0; i < primesCount; ++i) {
        p.setup(primeList[i]);
        p.findPerfectCycle(true);

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


void exploreWhenCycleBreaks()
{
    PrimeMusic p;

    for (size_t i = 0; i < primesCount; ++i) {
        std::cout << std::endl;
        p.setup(primeList[i]);
        const auto [steps, cycles] = p.whenCycleBreaks(true);

        if (steps == 0)
                break;
    }
}


void exploreWhenCycleConverges()
{
    PrimeMusic p;

    for (size_t i = 0; i < primesCount; ++i) {
        std::cout << std::endl;
        p.setup(primeList[i]);
        const auto [steps, cycles] = p.whenCycleConverges(true);

        if (steps == 0)
                break;
    }
}

//TODO new class PrimeMusicSequence - через init list - так же находить циклы итд



int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    //exploreCyclesAndIntervals();
    //exploreWhenCycleBreaks();
    exploreWhenCycleConverges();

    return 0;
}

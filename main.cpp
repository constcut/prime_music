#include <iostream>
#include <set>

#include "PrimeMusic.hpp"
#include "NoteOperations.hpp"

const size_t primesCount = 28;
int primeList [primesCount] = {3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53,
                    59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109 };

const size_t doubledCount = 10;
int doubledList [doubledCount] = {6, 10, 14, 22, 26, 34, 38, 46, 58, 62};


void exploreCyclesAndIntervals(int* sequence, size_t len)
{
    PrimeMusic p;
    std::set<int> intervalsSet;

    //Just a reminded 2 generated all perfect harmonics
    //3 and 5 generated 1-3-5 major intervals

    for (size_t i = 0; i < len; ++i) {
        p.setup(sequence[i]);
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


void exploreWhenCycleBreaks(int* sequence, size_t len)
{
    PrimeMusic p;

    for (size_t i = 0; i < len; ++i) {
        std::cout << std::endl;
        p.setup(sequence[i]);
        const auto [steps, cycles] = p.whenCycleBreaks(true);

        if (steps == 0)
                break;
    }
}


void exploreWhenCycleConverges(int* sequence, size_t len)
{
    PrimeMusic p;

    for (size_t i = 0; i < len; ++i) {
        std::cout << std::endl;
        p.setup(sequence[i]);
        const auto [steps, cycles] = p.whenCycleConverges(1., true);

        if (steps == 0)
                break;
    }
}

//TODO new class PrimeMusicSequence - через init list - так же находить циклы итд



int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    //exploreCyclesAndIntervals(primeList, primesCount);
    //exploreWhenCycleBreaks(primeList, primesCount);
    //exploreWhenCycleConverges(primeList, primesCount);

    exploreCyclesAndIntervals(doubledList, doubledCount);
    exploreWhenCycleBreaks(primeList, doubledCount);
    exploreWhenCycleConverges(primeList, doubledCount);

    return 0;
}

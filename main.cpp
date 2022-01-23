#include <iostream>
#include <unordered_map>

#include "PrimeMusic.hpp"
#include "NoteOperations.hpp"

const size_t primesCount = 28;
int primeList [primesCount] = {3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109 };



std::vector<int> exploreCyclesAndIntervals(int* sequence, size_t len, int mult = 1, bool log = false)
{
    PrimeMusic p;
    std::vector<int> intervals;
    for (size_t i = 0; i < len; ++i) {
        p.setup(sequence[i] * mult);
        p.findPerfectCycle(log);
        const auto [interval, _] = p.musicInterval();
        intervals.push_back(interval);
    }
    return intervals;
}


void exploreWhenCycleBreaks(int* sequence, size_t len, int mult = 1)
{
    PrimeMusic p;

    for (size_t i = 0; i < len; ++i) {
        std::cout << std::endl;
        p.setup(sequence[i] * mult);
        const auto [steps, cycles] = p.whenCycleBreaks(true);

        if (steps == 0)
                break;
    }
}


void exploreWhenCycleConverges(int* sequence, size_t len, int mult = 1)
{
    PrimeMusic p;

    for (size_t i = 0; i < len; ++i) {
        std::cout << std::endl;
        p.setup(sequence[i] * mult);
        const auto [steps, cycles] = p.whenCycleConverges(1., true);

        if (steps == 0)
                break;
    }
}

//TODO new class PrimeMusicSequence - через init list - так же находить циклы итд


std::vector<int> noteDiffVector(const std::vector<int>& lhs,
                const std::vector<int>& rhs, bool noteNorm = true)
{
    if (lhs.size() != rhs.size())
        return {};

    std::vector<int> diffs;

    for (size_t i = 0; i < lhs.size(); ++i) {
        int diff = (lhs[i] % 12) - (rhs[i] % 12);
        if (noteNorm && diff < 0)
            diff += 12;
         diffs.push_back(diff);
    }

    return diffs;
}


std::vector<int> mostFrequentValues(const std::vector<int>& v) {
    return {};
}



int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    //exploreCyclesAndIntervals(primeList, primesCount);
    //exploreWhenCycleBreaks(primeList, primesCount);
    //exploreWhenCycleConverges(primeList, primesCount);

    //exploreCyclesAndIntervals(primeList, primesCount, 5);
    //exploreWhenCycleBreaks(primeList, primesCount, 2);
    //exploreWhenCycleConverges(primeList, primesCount, 2);

    auto basicIntervals = exploreCyclesAndIntervals(primeList, primesCount);

    for (size_t i = 0; i < primesCount; ++i) {

        const int prime = primeList[i];

        auto nextIntervals = exploreCyclesAndIntervals(primeList, primesCount, prime);
        auto diffVector = noteDiffVector(nextIntervals, basicIntervals);

        std::cout << "For " << prime << " diff vector: " << std::endl;
        for (auto d: diffVector)
            std::cout << d << " ";
        std::cout << std::endl <<  std::endl;
    }

    return 0;
}

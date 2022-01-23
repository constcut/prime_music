#include <iostream>
#include <unordered_map>
#include <algorithm>

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
    std::unordered_map<int, size_t> entries;

    for (auto element: v) {
        if (entries.count(element))
            entries[element] += 1;
        else
            entries[element] = 1;
    }

    std::vector<std::pair<int,size_t>> sorted(entries.begin(), entries.end());
    std::sort(sorted.begin(), sorted.end(), [](auto& lhs, auto& rhs){ return lhs.second > rhs.second;});

    std::vector<int> mostFreq;
    for (auto s: sorted)
        mostFreq.push_back(s.first);

    return mostFreq;
}


void explorePrimeProducts() {
    auto basicIntervals = exploreCyclesAndIntervals(primeList, primesCount);

    for (size_t i = 0; i < primesCount; ++i)
    {
        const int prime = primeList[i];
        auto nextIntervals = exploreCyclesAndIntervals(primeList, primesCount, prime);
        auto diffVector = noteDiffVector(nextIntervals, basicIntervals);
        auto mostFreq = mostFrequentValues(diffVector);

        int mostDiff = mostFreq[1] - mostFreq[0];
        if (mostDiff == -11)
            mostDiff = 1;

        std::string expectationString;

        if ((basicIntervals[i] % 12) == mostFreq[0])
            expectationString = " Most frequent equal";
        else if ((basicIntervals[i] % 12) == mostFreq[1])
            expectationString = " Second frequent equal";
        else
            expectationString = " Something unexpected";

        std::cout << prime << ") " << mostFreq[0] << " Total " << mostFreq.size() <<
                  " Diff " << mostDiff << expectationString << std::endl;
    }
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

    const size_t multSize = 6;
    int multProducts[multSize] = {15, 21, 33, 35, 39, 55};

    for (size_t i = 0; i < multSize; ++i)
    {
        const int number = multProducts[i];
        auto nextIntervals = exploreCyclesAndIntervals(primeList, primesCount, number);
        auto diffVector = noteDiffVector(nextIntervals, basicIntervals);
        auto mostFreq = mostFrequentValues(diffVector);

        int mostDiff = mostFreq[1] - mostFreq[0];
        if (mostDiff == -11)
            mostDiff = 1;

        std::string expectationString;

        if ((basicIntervals[i] % 12) == mostFreq[0])
            expectationString = " Most frequent equal";
        else if ((basicIntervals[i] % 12) == mostFreq[1])
            expectationString = " Second frequent equal";
        else
            expectationString = " Something unexpected";

        std::cout << number << ") " << mostFreq[0] << " Total " << mostFreq.size() <<
                  " Diff " << mostDiff << expectationString << std::endl;
    }

    return 0;
}

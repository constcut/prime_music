#ifndef PRIMEMUSIC_H
#define PRIMEMUSIC_H

#include <vector>


class PrimeMusic
{
public:
    PrimeMusic() = default;
    PrimeMusic(const int midiNote, double primeNumber); //TODO midi note as second param and default value = 12

    void setup(const int midiNote, double primeNumber);

    void streightIterations(size_t count);

    std::pair<int, double> musicInterval();

    const std::vector<int>& findPerfectCycle();

private:

    int _midiNote;
    double _primeNumber; //rename TODO just number

    std::vector<int> _noteSequence;
    std::vector<double> _centsSequece;

    std::vector<int> _perfectCycle;

    bool gotCycle(const std::vector<int>& sequence);

};

#endif // PRIMEMUSIC_H

#ifndef PRIMEMUSIC_H
#define PRIMEMUSIC_H

#include <vector>


class PrimeMusic
{
public:
    PrimeMusic() = default;
    PrimeMusic(const double primeNumber, const int midiNote = 12); //TODO midi note as second param and default value = 12

    void setup(const double primeNumber, const int midiNote = 12);

    void streightIterations(size_t count);

    std::pair<int, double> musicInterval();

    const std::vector<int>& findPerfectCycle();

private:

    int _midiNote = 12;
    double _primeNumber = 2.0; //rename TODO just number

    std::vector<int> _noteSequence;
    std::vector<double> _centsSequece;

    std::vector<int> _perfectCycle;

    bool gotCycle(const std::vector<int>& sequence);

};

#endif // PRIMEMUSIC_H

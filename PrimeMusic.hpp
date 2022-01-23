#ifndef PRIMEMUSIC_H
#define PRIMEMUSIC_H

#include <vector>


class PrimeMusic
{
public:
    PrimeMusic() = default;
    PrimeMusic(const int midiNote, double primeNumber);

    void setup(const int midiNote, double primeNumber);

    void runIterations(size_t count);

private:

    int _midiNote;
    double _primeNumber;

    std::vector<int> _noteSequence;
    std::vector<double> _centsSequece;

};

#endif // PRIMEMUSIC_H

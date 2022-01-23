#include "PrimeMusic.hpp"

#include <iostream>

#include "NoteOperations.hpp"


PrimeMusic::PrimeMusic(const int midiNote, double primeNumber)
    : _midiNote(midiNote),
      _primeNumber(primeNumber)
{
}


void PrimeMusic::setup(const int midiNote, double primeNumber) {
    _midiNote = midiNote;
    _primeNumber = primeNumber;
}


void PrimeMusic::streightIterations(size_t count) {

    double _currentFreq = freqFromMidiNote(_midiNote);

    for (size_t i = 0; i < count; ++i)
    {
        if (i)
            _currentFreq *= _primeNumber;

        auto [midiNote, cents] = findClosestNote(_currentFreq);

        //TODO if midiNote > 128

        const auto noteName = nameMidiNote(midiNote);

        std::cout << i << ") freq= " << _currentFreq << " name= "
                  << noteName << " cents d= " << cents << std::endl;
    }
}


std::pair<int, double> PrimeMusic::musicInterval() {
    const double basicFreq = freqFromMidiNote(_midiNote); //TOOD into noteopearions
    const double nextMidiNote = midiCents(basicFreq * _primeNumber) / 100.0;
    const double interval = nextMidiNote - _midiNote;

    const double closestInterval = std::round(interval);
    const double centsDeviation = (interval - closestInterval) * 100;
    return {closestInterval, centsDeviation};
}

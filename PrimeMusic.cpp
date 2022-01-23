#include "PrimeMusic.hpp"


PrimeMusic::PrimeMusic(const int midiNote, double primeNumber)
    : _midiNote(midiNote),
      _primeNumber(primeNumber)
{
}


void PrimeMusic::setup(const int midiNote, double primeNumber) {
    _midiNote = midiNote;
    _primeNumber = primeNumber;
}

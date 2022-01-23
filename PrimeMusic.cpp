#include "PrimeMusic.hpp"

#include <iostream>

#include "NoteOperations.hpp"


PrimeMusic::PrimeMusic(const double primeNumber, const int midiNote)
    : _midiNote(midiNote),
      _primeNumber(primeNumber)
{
}


void PrimeMusic::setup(const double primeNumber, const int midiNote) {
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


const std::vector<int>& PrimeMusic::findPerfectCycle() {

    std::vector<int> keyNotes {_midiNote % 12 };

    const auto [interval, _] = musicInterval();

    int currentNote = _midiNote;
    for (size_t i = 1; i < 100000; ++i) {
        currentNote += interval;
        keyNotes.push_back(currentNote % 12);
        if (gotCycle(keyNotes))
            break;
    }
    //TODO insure cycle found

    _perfectCycle = std::vector<int>(keyNotes.begin(),
                                    keyNotes.begin() + keyNotes.size()/2);

    std::cout << "Perfect cycle for " << _primeNumber << " interval " << interval % 12 << std::endl;

    for (int midiNote: _perfectCycle)
        std::cout << nameMidiNote(midiNote) << " ";

    std::cout << std::endl << "Cycle length: " << _perfectCycle.size() << std::endl;

    return _perfectCycle;
}


bool PrimeMusic::gotCycle(const std::vector<int>& sequence) {
   if (sequence.size() % 2)
       return false;

   const size_t halfSize = sequence.size() / 2;
   for (size_t i = 0; i < halfSize; ++i)
       if (sequence[i] != sequence[i + halfSize])
           return false;

   return true;
}

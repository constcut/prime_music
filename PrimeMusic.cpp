#include "PrimeMusic.hpp"

#include <iostream>

#include "NoteOperations.hpp"


PrimeMusic::PrimeMusic(const long double primeNumber, const int midiNote)
    : _midiNote(midiNote),
      _primeNumber(primeNumber)
{
}


void PrimeMusic::setup(const long double primeNumber, const int midiNote) {
    _midiNote = midiNote;
    _primeNumber = primeNumber;
}


void PrimeMusic::streightIterations(size_t count) {

    long double _currentFreq = freqFromMidiNote(_midiNote);

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


std::pair<int, long double> PrimeMusic::musicInterval() {
    const long double basicFreq = freqFromMidiNote(_midiNote); //TOOD into noteopearions
    const long double nextMidiNote = midiCents(basicFreq * _primeNumber) / 100.0;
    const long double interval = nextMidiNote - _midiNote;

    const long double closestInterval = std::round(interval);
    const long double centsDeviation = (interval - closestInterval) * 100;
    return {closestInterval, centsDeviation};
}


const std::vector<int>& PrimeMusic::findPerfectCycle(bool log) {

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

    if (log) {
        std::cout << "Perfect cycle for " << _primeNumber << " interval "
                  << interval % 12 << " " << nameMusicInterval(interval % 12) << std::endl;

        for (int midiNote: _perfectCycle)
            std::cout << nameMidiNote(midiNote) << " ";

        std::cout << std::endl << "Cycle length: " << _perfectCycle.size() << std::endl;
    }

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


std::pair<int, long double> PrimeMusic::whenCycleBreaks(bool log) {
    const auto [_, centsDeviation] = musicInterval();

    if (log)
        std::cout << "Cents deviation: " << centsDeviation << std::endl;

    const auto& cycle = findPerfectCycle();

    long double cummulateError = 0.0;
    size_t steps;

    for (steps = 1; steps < 100; ++steps) {
        cummulateError += std::abs(centsDeviation);
        if (cummulateError > 50.0)
            break;
    }

    long double fullCycles = static_cast<long double>(steps) / static_cast<long double>(cycle.size());

    if (log) {
        std::cout << "For " << _primeNumber << " cycle breaks on " << steps << " steps" << std::endl;
        std::cout << "Full cycles " << fullCycles << std::endl;
    }

    return {steps, fullCycles};
}



std::pair<int, long double> PrimeMusic::whenCycleConverges(bool log) {

    const auto [interval, centsDeviation] = musicInterval();

    const int firstNote = _midiNote % 12;
    long double currentFreq = freqFromMidiNote(firstNote);
    const auto& cycle = findPerfectCycle();

    std::cout << std::endl << "WCC: " << centsDeviation << std::endl;

    bool converged = false;

    size_t steps = 0;
    for (steps = 1; steps < 5000; ++steps) { //TODO steps limit in class

        currentFreq *= _primeNumber;
        const auto [newNote, cents] = findClosestNote(currentFreq);
        const int currentNote = newNote % 12;
        const int cyclePos = steps % cycle.size();

        //std::cout << steps << ") " << nameMidiNote(currentNote) << " cycle pos " << cyclePos << " and cents " << cents << std::endl;

        if (std::abs(cents) < 1. && currentNote == firstNote && cyclePos == 0) {

            if (log)
                std::cout << "Converged! on step " << steps << std::endl;
            converged = true;

            break;
        }
    }

    if (converged == false) {
        std::cout << "Failed to converge" << std::endl;
    }

    long double fullCycles = static_cast<long double>(steps) / static_cast<long double>(cycle.size());

    if (log)
        std::cout << "Full cycles " << fullCycles << std::endl;

    return {steps, fullCycles};
}



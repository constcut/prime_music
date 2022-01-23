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


std::pair<int, double> PrimeMusic::whenCycleBreaks(bool log) {
    const auto [_, centsDeviation] = musicInterval();

    if (log)
        std::cout << "Cents deviation: " << centsDeviation << std::endl;

    const auto& cycle = findPerfectCycle();

    double cummulateError = 0.0;
    size_t steps;

    for (steps = 1; steps < 100; ++steps) {
        cummulateError += std::abs(centsDeviation);
        if (cummulateError > 50.0)
            break;
    }

    double fullCycles = static_cast<double>(steps) / static_cast<double>(cycle.size());

    if (log) {
        std::cout << "For " << _primeNumber << " cycle breaks on " << steps << " steps" << std::endl;
        std::cout << "Full cycles " << fullCycles << std::endl;
    }

    return {steps, fullCycles};
}



std::pair<int, double> PrimeMusic::whenCycleConverges(bool log) {

    const auto [interval, centsDeviation] = musicInterval();

    double cummulateError = 0.0;

    const int firstNote = _midiNote % 12;
    int currentNote = firstNote;

    size_t steps = 0;
    for (steps = 1; steps < 10000; ++steps) { //TODO steps limit in class

        currentNote += (interval % 12);
        currentNote %= 12;

        cummulateError += std::abs(centsDeviation);

        if (cummulateError >= 50.)
            cummulateError -= 50.;

        if (cummulateError < 1. && currentNote == firstNote) {

            if (log)
                std::cout << "Converged! on step " << steps << std::endl;

            break;
        }
    }

    const auto& cycle = findPerfectCycle();
    double fullCycles = static_cast<double>(steps) / static_cast<double>(cycle.size());

    if (log)
        std::cout << "Full cycles " << fullCycles << std::endl;

    return {steps, fullCycles};
}



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


unsigned long prevPowOfTwo(unsigned long v)
{
    v--; //Black bit magic
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v / 2;
}


std::pair<int, long double> PrimeMusic::whenCycleConverges(long double epsilaCents, bool log) {

    const int firstNote = _midiNote % 12;
    long double currentFreq = freqFromMidiNote(firstNote);
    const auto& cycle = findPerfectCycle();

    bool converged = false;
    long double prevPow = prevPowOfTwo(_primeNumber);

    size_t step = 0;
    for (step = 1; step < interationsLimit; ++step) {

        currentFreq *= _primeNumber;
        currentFreq /= prevPow; //Trick to decrease growth

        const auto [newNote, cents] = findClosestNote(currentFreq);
        const int currentNote = newNote % 12;
        const int cyclePos = step % cycle.size();

        if (std::abs(cents) < epsilaCents && currentNote == firstNote && cyclePos == 0) {
            if (log)
                std::cout << "Converged! " << _primeNumber <<  " on step " << step << std::endl;
            converged = true;
            break;
        }
    }


    if (converged == false) {
        const auto [newNote, cents] = findClosestNote(currentFreq);
        std::cout << "Failed to converge " << _primeNumber << " last cents " << cents <<
                std::endl;
    }

    long double fullCycles = static_cast<long double>(step) / static_cast<long double>(cycle.size());
    if (log)
        std::cout << "Full cycles " << fullCycles << std::endl;

    return {step, fullCycles};
}



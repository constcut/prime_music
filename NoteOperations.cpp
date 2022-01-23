#include "NoteOperations.hpp"

#include <string>
#include <cmath>


double midiOctaves[] = { 8.175, 16.35, 32.7, 65.4, 130.8, 261.6, 523.2,
                         1046.4, 2092.8, 4185.6, 8371,2, 16742.4};

double subContrOctave[] = { 16.35, 17.32, 18.35, 19.44, 20.61, 21.82,
                            23.12, 24.50, 25.95, 27.50, 29.13, 30.87 };

std::string noteNames[] = { "C", "C#", "D", "D#", "E", "F",
                            "F#", "G", "G#", "A", "A#", "B" };


double midiCents(const double f0, const double tuneFreq,
                                      const int midiTune)
{
    double note = 12.0 * std::log2(f0 / tuneFreq) + midiTune;
    return note * 100.0;
}


std::pair<double, double> findClosestNote(const double f0) {
    const double cents = midiCents(f0);
    const double midiNote = cents / 100.0;
    const double closestNote = std::round(midiNote);
    const double centsDeviation = (midiNote - closestNote) * 100.0;
    return {closestNote, centsDeviation};
}


std::string nameMidiNote(const int midiNote) {
    const std::string key = noteNames[midiNote % 12]; //TODO subfunction note + octave as pair
    const int octaveNumber = (midiNote / 12) - 1;
    return key + std::to_string(octaveNumber);
}


double freqFromMidiNote(const int midiNote) {
    const int noteIdx = midiNote % 12;
    const int octave = (midiNote / 12) - 1;
    double octavePow = std::pow(2.0, octave);
    return subContrOctave[noteIdx] * octavePow;
}


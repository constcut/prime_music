#include "NoteOperations.hpp"

#include <string>
#include <cmath>


long double midiOctaves[] = { 8.175, 16.35160, 32.7, 65.4, 130.8, 261.6, 523.2,
                         1046.4, 2092.8, 4185.6, 8371,2, 16742.4};

long double subContrOctave[] = { 16.35160, 17.32391, 18.35405, 19.44544, 20.60172, 21.82676,
                            23.12465, 24.49971, 25.95654, 27.50000, 29.13524, 30.86771 };

std::string noteNames[] = { "C", "C#", "D", "D#", "E", "F",
                            "F#", "G", "G#", "A", "A#", "B" };


std::string musicIntervals[] = {"Perfect unison", "Minor second", "Major second", "Minor third", "Major third", "Perfect fourth",
                                "Diminished fifth/Augmented fourth", "Perfect fifth", "Minor sixth", "Major sixth",
                                "Minor seventh", "Major seventh", "Perfect octave" };



std::string nameMusicInterval(const int semitones) {

    if (semitones == 0)
        return musicIntervals[0];
    if (semitones % 12 == 0)
        return musicIntervals[12];

    return musicIntervals[semitones % 12];
}


long double midiCents(const long double f0, const long double tuneFreq,
                                      const int midiTune)
{
    long double note = 12.0 * std::log2(f0 / tuneFreq) + midiTune;
    return note * 100.0;
}


std::pair<int, long double> findClosestNote(const long double f0) {
    const long double cents = midiCents(f0);
    const long double midiNote = cents / 100.0;
    const long double closestNote = std::round(midiNote);
    const long double centsDeviation = (midiNote - closestNote) * 100.0;
    return {closestNote, centsDeviation};
}


std::string fullNameMidiNote(const int midiNote) {
    const std::string key = noteNames[midiNote % 12]; //TODO subfunction note + octave as pair
    const int octaveNumber = (midiNote / 12) - 1;
    return key + std::to_string(octaveNumber);
}


std::string nameMidiNote(const int midiNote) {
    const std::string key = noteNames[midiNote % 12];
    return key;
}


long double freqFromMidiNote(const int midiNote) {
    const int noteIdx = midiNote % 12;
    const int octave = (midiNote / 12) - 1;
    long double octavePow = std::pow(2.0, octave);
    return subContrOctave[noteIdx] * octavePow;
}


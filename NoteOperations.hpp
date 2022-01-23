#ifndef NOTEOPERATIONS_HPP
#define NOTEOPERATIONS_HPP


#include <string>
#include <cmath>


//TODO enum for all MidiNote to avoid magic constants


long double midiCents(const long double f0, const long double tuneFreq = 440.0,
                                      const int midiTune = 69);

//Returns pair: closes midi note + deviation in cents
std::pair<int, long double> findClosestNote(const long double f0);


std::string fullNameMidiNote(const int midiNote);
std::string nameMidiNote(const int midiNote);


long double freqFromMidiNote(const int midiNote); //TODO tuneFreq for A4 or else


std::string nameMusicInterval(const int semitones);


#endif // NOTEOPERATIONS_HPP

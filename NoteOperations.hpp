#ifndef NOTEOPERATIONS_HPP
#define NOTEOPERATIONS_HPP


#include <string>
#include <cmath>


double midiCents(const double f0, const double tuneFreq = 440.0,
                                      const int midiTune = 69);

//Returns pair: closes midi note + deviation in cents
std::pair<double, double> findClosestNote(const double f0);


std::string nameMidiNote(const int midiNote);


double freqFromMidiNote(const int midiNote);



#endif // NOTEOPERATIONS_HPP

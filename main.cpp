#include <iostream>
#include <unordered_map>
#include <algorithm>

#include "PrimeMusic.hpp"
#include "NoteOperations.hpp"
#include "Experiments.hpp"

#include "midi/MidiFile.hpp"

using namespace aural_sight;


std::vector<int> primeList = { 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
                               73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157,
                               163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263,
                               269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809,
                               811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941,
                               947, 953, 967, 971, 977, 983, 991, 997, 1009, 1013, 1019, 1021, 1031, 1033, 1039, 1049, 1051, 1061, 1063, 1069,
                               1087, 1091, 1093, 1097, 1103, 1109, 1117, 1123, 1129, 1151, 1153, 1163, 1171, 1181, 1187, 1193, 1201, 1213, 1217, 1223,
                               1229, 1231, 1237, 1249, 1259, 1277, 1279, 1283, 1289, 1291, 1297, 1301, 1303, 1307, 1319, 1321, 1327, 1361, 1367, 1373,
                               1381, 1399, 1409, 1423, 1427, 1429, 1433, 1439, 1447, 1451, 1453, 1459, 1471, 1481, 1483, 1487, 1489, 1493, 1499, 1511,
                               1523, 1531, 1543, 1549, 1553, 1559, 1567, 1571, 1579, 1583, 1597, 1601, 1607, 1609, 1613, 1619, 1621, 1627, 1637, 1657,
                               1663, 1667, 1669, 1693, 1697, 1699, 1709, 1721, 1723, 1733, 1741, 1747, 1753, 1759, 1777, 1783, 1787, 1789, 1801, 1811,
                               1823, 1831, 1847, 1861, 1867, 1871, 1873, 1877, 1879, 1889, 1901, 1907, 1913, 1931, 1933, 1949, 1951, 1973, 1979, 1987,
                               1993, 1997, 1999, 2003, 2011, 2017, 2027, 2029, 2039, 2053, 2063, 2069, 2081, 2083, 2087, 2089, 2099, 2111, 2113, 2129,
                               2131, 2137, 2141, 2143, 2153, 2161, 2179, 2203, 2207, 2213, 2221, 2237, 2239, 2243, 2251, 2267, 2269, 2273, 2281, 2287,
                               2293, 2297, 2309, 2311, 2333, 2339, 2341, 2347, 2351, 2357, 2371, 2377, 2381, 2383, 2389, 2393, 2399, 2411, 2417, 2423,
                               2437, 2441, 2447, 2459, 2467, 2473, 2477, 2503, 2521, 2531, 2539, 2543, 2549, 2551, 2557, 2579, 2591, 2593, 2609, 2617,
                               2621, 2633, 2647, 2657, 2659, 2663, 2671, 2677, 2683, 2687, 2689, 2693, 2699, 2707, 2711, 2713, 2719, 2729, 2731, 2741,
                               2749, 2753, 2767, 2777, 2789, 2791, 2797, 2801, 2803, 2819, 2833, 2837, 2843, 2851, 2857, 2861, 2879, 2887, 2897, 2903,
                               2909, 2917, 2927, 2939, 2953, 2957, 2963, 2969, 2971, 2999, 3001, 3011, 3019, 3023, 3037, 3041, 3049, 3061, 3067, 3079,
                               3083, 3089, 3109, 3119, 3121, 3137, 3163, 3167, 3169, 3181, 3187, 3191, 3203, 3209, 3217, 3221, 3229, 3251, 3253, 3257,
                               3259, 3271, 3299, 3301, 3307, 3313, 3319, 3323, 3329, 3331, 3343, 3347, 3359, 3361, 3371, 3373, 3389, 3391, 3407, 3413,
                               3433, 3449, 3457, 3461, 3463, 3467, 3469, 3491, 3499, 3511, 3517, 3527, 3529, 3533, 3539, 3541, 3547, 3557, 3559, 3571 };


void printPrimeIntervals(const std::vector<int>& sequence) {
    for (size_t i = 0; i < sequence.size(); ++i) {

        const int edo = 19;

        const long double semitones = edo * std::log2(
                    static_cast<long double>(sequence[i]));
        const uint32_t closestSemitones = std::round(semitones);
        const long double cents = (semitones - closestSemitones) * 100.0;

        std::cout << sequence[i] << ", "
                  << closestSemitones << ", "
                  << closestSemitones % edo << ", "
                  << cents
                  << std::endl;
    }
}


//Посчитать интервалы найти приближение в виде простое делённое на 2 в степени, где разница будет меньше цента
//Меньше десятой цента

//Но наверное всё это не очень интересно

//Интересна сходимость, написать разные варианты, когда нота содержится всегда в 1 октаве или больше

std::pair<int, long double> musicInterval(int midiNote, double number) {
    const long double basicFreq = freqFromMidiNote(midiNote); //TOOD into noteopearions
    const long double nextMidiNote = midiCents(basicFreq * number) / 100.0;
    const long double interval = nextMidiNote - midiNote;

    const long double closestInterval = std::round(interval);
    const long double centsDeviation = (interval - closestInterval) * 100;
    return {closestInterval, centsDeviation};
}


bool gotCycle(const std::vector<int>& sequence) {
   if (sequence.size() % 2)
       return false;

   const size_t halfSize = sequence.size() / 2;
   for (size_t i = 0; i < halfSize; ++i)
       if (sequence[i] != sequence[i + halfSize])
           return false;

   return true;
}


std::vector<int> findPerfectCycle2(int midiNote, double first, double second) { //TODO init list

    std::vector<int> keyNotes {midiNote % 12 };

    const auto [interval1, _] = musicInterval(midiNote, first);
    const auto [interval2, __] = musicInterval(midiNote, second);

    int currentNote = midiNote;
    for (size_t i = 1; i < 100000; ++i) {

        if (i % 2)
            currentNote += interval1;
        if (i % 2 == 0)
            currentNote += interval2;

        keyNotes.push_back(currentNote % 12);
        if (gotCycle(keyNotes))
            break;
    }

    std::vector<int> perfectCycle = std::vector<int>(keyNotes.begin(),
                                    keyNotes.begin() + keyNotes.size()/2);

    return perfectCycle;
}



std::vector<int> findPerfectCycleIntervals(int midiNote, int first, int second, int third) { //TODO init list

    std::vector<int> keyNotes {midiNote % 12 };

    int currentNote = midiNote;
    for (size_t i = 1; i < 100000; ++i) {

        if (i % 3 == 1)
            currentNote += first;
        if (i % 3 == 2)
            currentNote += second;
        if (i % 3 == 0)
            currentNote += third;

        keyNotes.push_back(currentNote % 12);
        if (gotCycle(keyNotes))
            break;
    }

    std::vector<int> perfectCycle = std::vector<int>(keyNotes.begin(),
                                    keyNotes.begin() + keyNotes.size()/2);

    return perfectCycle;
}



void tipleGeneration() {


    size_t count = 0;

    size_t kCount = 0;

    for (size_t i = 1; i < 12; ++i) {

        bool hadOnce = false;

        for (size_t j = 1; j < 12; ++j) {



            for (size_t k = 1; k < 12; ++k) {

                MidiTrack track;
                track.pushChangeBPM(240, 0); //somehow 240 is almost! realtime


                ++count;

                hadOnce = true;
                /*std::cout << std::endl << " " << i << " + " << j << " + " << k
                          << " sum  " << i + j + k << std::endl;*/

                auto cycle = findPerfectCycleIntervals(12, i, j, k);

                std::cout << "{\"circles\": [{\"digits\":[";
                for (size_t idx = 0; idx < cycle.size() ; idx++) {

                    std::cout << cycle[idx];
                    if (idx != cycle.size() - 1)
                        std::cout << ", ";

                    track.pushNoteOn(48 + cycle[idx], 100, 0);
                    track.accumulate(500);
                    track.pushNoteOff(48 + cycle[idx], 100, 0);
                }

                for (size_t idx = 0; idx < cycle.size() ; idx++) {
                    track.pushNoteOn(36 + cycle[idx], 100, 0);
                    track.accumulate(500);
                    track.pushNoteOff(36 + cycle[idx], 100, 0);
                }

                for (size_t idx = 0; idx < cycle.size() ; idx++) {
                    track.pushNoteOn(36 + cycle[idx], 100, 0);
                    track.pushNoteOn(48 + cycle[idx], 100, 0);
                    track.accumulate(500);
                    track.pushNoteOff(36 + cycle[idx], 100, 0);
                    track.pushNoteOff(48 + cycle[idx], 100, 0);
                }
                std::cout << "],\"scale\":13}]},";
                std::cout << std::endl;
                //std::cout << cycle.size() << std::endl;

                track.pushEvent47();

                MidiFile midi;
                midi.push_back(track);
                std::string filename = std::to_string(i) + "+" + std::to_string(j) + "+" +
                            std::to_string(k) + ".mid";

                if (cycle.size() == 36)
                midi.writeToFile(filename);

                //TODO render sound
            }


        }

        if (hadOnce) {
            std::cout << std::endl << "J BREAK i = " << i << std::endl;
            ++kCount;
        }
    }

    std::cout << std::endl << "TOTAL " << count <<
               " and " << kCount << std::endl;
}




void findPerfectCycle(int midiNote) { //TODO init list


    for (size_t i = 1; i < 12; ++i) {

        std::vector<int> keyNotes {midiNote % 12 };
        int currentNote = midiNote;

        for (size_t j = 0; j < 10000; ++j) {

            currentNote += i;
            keyNotes.push_back(currentNote % 12);

            if (gotCycle(keyNotes))
                break;
        }

        std::vector<int> perfectCycle = std::vector<int>(keyNotes.begin(),
                                        keyNotes.begin() + keyNotes.size()/2);

        MidiTrack track;
        track.pushChangeBPM(240, 0); //somehow 240 is almost! realtime

        for (auto& interval: perfectCycle) {
            track.pushNoteOn(48 + interval, 100, 0);
            track.accumulate(500);
            track.pushNoteOff(48 + interval, 100, 0);
        }
        for (auto& interval: perfectCycle) {
            track.pushNoteOn(48 + interval, 100, 0);
            track.accumulate(500);
            track.pushNoteOff(48 + interval, 100, 0);
        }

        std::cout << "Perfect cycle " << i << " " << perfectCycle.size() << std::endl;
        track.pushEvent47();

        MidiFile midi;
        midi.push_back(track);
        std::string filename = std::to_string(i) + "_.mid";
        midi.writeToFile(filename);
    }

}




void findPerfectCycle2(int midiNote) { //TODO init list


    for (size_t i = 1; i < 12; ++i) {



        for (size_t i2 = i + 1; i2 < 12; i2++) {

            std::vector<int> keyNotes {midiNote % 12 };
            int currentNote = midiNote;

            for (size_t j = 0; j < 10000; ++j) {

                if (j % 2 == 0)
                    currentNote += i;
                if (j%2 == 1)
                    currentNote += i2;

                keyNotes.push_back(currentNote % 12);

                if (gotCycle(keyNotes))
                    break;
            }

            std::vector<int> perfectCycle = std::vector<int>(keyNotes.begin(),
                                            keyNotes.begin() + keyNotes.size()/2);

            MidiTrack track;
            track.pushChangeBPM(240, 0); //somehow 240 is almost! realtime

            for (auto& interval: perfectCycle) {
                track.pushNoteOn(48 + interval, 100, 0);
                track.accumulate(500);
                track.pushNoteOff(48 + interval, 100, 0);
            }
            for (auto& interval: perfectCycle) {
                track.pushNoteOn(48 + interval, 100, 0);
                track.accumulate(500);
                track.pushNoteOff(48 + interval, 100, 0);
            }

            std::cout << "Perfect cycle " << i << " " << perfectCycle.size() << std::endl;
            track.pushEvent47();

            MidiFile midi;
            midi.push_back(track);
            std::string filename = std::to_string(i) + "_" + std::to_string(i2) + "_.mid";
            midi.writeToFile(filename);

        }

    }

}




int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) ///TODO эта функия будет просто как пример, офрмить в виде библиотеки
{


    //findPerfectCycle(36);
    findPerfectCycle2(36);


    return 0;
}

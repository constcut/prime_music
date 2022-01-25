#include "MidiMessage.hpp"

#include "log.hpp"


using namespace aural_sight;


MidiMessage::MidiMessage() : _typeAndChannel(0), _param1(0), _param2(0){}


MidiMessage::MidiMessage(const uint8_t b0, const uint8_t b1, const uint8_t b2, const uint32_t timeShift)
: _typeAndChannel(b0), _param1(b1), _param2(b2), _timeStamp(timeShift) {}


double MidiMessage::getSecondsLength(const double bpm) const {
    double seconds = static_cast<double>(_timeStamp.getValue()) * (120.0 / bpm) / 960.0;
    return seconds;
}


uint8_t MidiMessage::getEventType() const {
    uint8_t eventType = (_typeAndChannel & (MidiMasks::EventTypeMask)) >> 4; //name with enumeration byte blocks
    return eventType;
}


uint8_t MidiMessage::getChannel() const {
    uint8_t midiChannel = _typeAndChannel & MidiMasks::ChannelMask; //name with enumeration byte blocks
    return midiChannel;
}


bool MidiMessage::isMetaEvent() const {
    return _typeAndChannel == MidiEvent::MetaEvent;
}

bool MidiMessage::isNotSingleParamEvent(const uint8_t eventType) const {
    return (eventType != MidiEvent::PatchChange) && (eventType != MidiEvent::ChannelPressure) &&
            (eventType != MidiEvent::SystemCommonMessage2Bytes) && (eventType != MidiEvent::SystemCommonMessage3Bytes) &&
            (eventType != MidiEvent::SysExStartOrContinue) && (eventType != MidiEvent::SystemCommonMessage1Byte) &&
            (eventType != MidiEvent::SysExEnds2Bytes) && (eventType != MidiEvent::ReservedExtentions);
}


uint32_t MidiMessage::calculateSize(const bool skipSomeMessages) const {
    uint32_t messageSize = 0;

    if (skipSomeMessages == true)
        if (canSkipThat())
            return 0;

    messageSize += _timeStamp.size();
    ++messageSize; //byte 0

    if (isMetaEvent() == false) {
        ++messageSize; //parameter 1
        uint8_t eventType = getEventType();

        if (isNotSingleParamEvent(eventType))
            ++messageSize;                                                                                                                                                                //parameter 2
    }
    else {
        ++messageSize; //parameter 1 actually
        messageSize += _metaLen.size();
        messageSize += _metaBufer.size();
    }
    return messageSize;
}


bool MidiMessage::canSkipThat() const {
    if (isMetaEvent()) {
        if (_param1 == MidiMetaTypes::KindOfFinish)
            return false;
        if (_param1 == MidiMetaTypes::ChangeTempo)
            return false;
        if (_param1 == MidiMetaTypes::ChangeTimeSignature)
            return false;
        if (_param1 == MidiMetaTypes::TrackName)
            return false;
        return true;
    }
    else {
        uint8_t eventType = getEventType();
        if ((eventType == MidiEvent::NoteOff) || (eventType == MidiEvent::NoteOn))
            return false;
        return true;
    }
    return true;
}


uint32_t MidiMessage::readFromFile(std::ifstream& f) {

    uint32_t totalBytesRead = 0;
    totalBytesRead += _timeStamp.readFromFile(f);
    f.read(reinterpret_cast<char*>(&_typeAndChannel), 1);
    f.read(reinterpret_cast<char*>(&_param1), 1);
    totalBytesRead += 2;

    if (isMetaEvent()) {
        totalBytesRead += _metaLen.readFromFile(f);
        uint32_t bytesInMetaBufer = _metaLen.getValue();
        _metaBufer.clear();

        for (uint32_t i = 0; i < bytesInMetaBufer; ++i) {
            uint8_t byteBufer;
            f.read(reinterpret_cast<char*>(&byteBufer), 1);
            _metaBufer.push_back(byteBufer);
        }
        totalBytesRead += bytesInMetaBufer;
        if (enableMidiLog)
            qDebug() << "Midi meta mes read " << _typeAndChannel << _param1 << _metaLen.getValue() << _timeStamp.getValue() << " total bytes " << totalBytesRead << " " << f.tellg();
    }
    else {
        uint8_t eventType = getEventType();
        if (isNotSingleParamEvent(eventType)) {
            f.read(reinterpret_cast<char*>(&_param2), 1);
            ++totalBytesRead;
        }
        if (enableMidiLog)
            qDebug() << "Midi message read " << nameEvent(eventType) << " ( " << eventType << getChannel() << " ) "
                   << static_cast<int>(_param1)  << " " << static_cast<int>(_param2) << " t: " << _timeStamp.getValue() << " total bytes " << totalBytesRead << " " << f.tellg();
        if (eventType == 0xB) {
            if (enableMidiLog)
                qDebug() << "Controller name: " << nameController(_param1);
        }
    }

    if (totalBytesRead > calculateSize())
        qDebug() << "Error! overread " << f.tellg();
    return totalBytesRead;
}


std::string MidiMessage::nameEvent(const uint8_t eventNumber) const {
    switch (eventNumber) {
        case MidiEvent::NoteOff:
            return "Note off";
        case MidiEvent::NoteOn:
            return "Note on";
        case MidiEvent::Aftertouch:
            return "Aftertouch";
        case MidiEvent::ControlChange:
            return "Control change";
        case MidiEvent::PatchChange:
            return "Program (patch) change";
        case MidiEvent::ChannelPressure:
            return "Channel pressure";
        case MidiEvent::PitchWheel:
            return "Pitch Wheel";
    }
    return "Unknown_EventType";
}


std::string MidiMessage::nameController(const uint8_t controllerNumber) const
{
    struct controllesNames {
        uint8_t index;
        std::string name;
    };

    controllesNames names[] = {{0, "Bank Select"},
                               {1, "Modulation Wheel (coarse)"},
                               {2, "Breath controller (coarse)"},
                               {4, "Foot Pedal (coarse)"},
                               {5, "Portamento Time (coarse)"},
                               {6, "Data Entry (coarse)"},
                               {7, "Volume (coarse)"},
                               {8, "Balance (coarse)"},
                               {10, "Pan position (coarse)"},
                               {11, "Expression (coarse)"},
                               {12, "Effect Control 1 (coarse)"},
                               {13, "Effect Control 2 (coarse)"},
                               {16, "General Purpose Slider 1"},
                               {17, "General Purpose Slider 2"},
                               {18, "General Purpose Slider 3"},
                               {19, "General Purpose Slider 4"},
                               {32, "Bank Select (fine)"},
                               {33, "Modulation Wheel (fine)"},
                               {34, "Breath controller (fine)"},
                               {36, "Foot Pedal (fine)"},
                               {37, "Portamento Time (fine)"},
                               {38, "Data Entry (fine)"},
                               {39, "Volume (fine)"},
                               {40, "Balance (fine)"},
                               {42, "Pan position (fine)"},
                               {43, "Expression (fine)"},
                               {44, "Effect Control 1 (fine)"},
                               {45, "Effect Control 2 (fine)"},
                               {64, "Hold Pedal (on/off)"},
                               {65, "Portamento (on/off)"},
                               {66, "Sustenuto Pedal (on/off)"},
                               {67, "Soft Pedal (on/off)"},
                               {68, "Legato Pedal (on/off)"},
                               {69, "Hold 2 Pedal (on/off)"},
                               {70, "Sound Variation"},
                               {71, "Sound Timbre"},
                               {72, "Sound Release Time"},
                               {73, "Sound Attack Time"},
                               {74, "Sound Brightness"},
                               {75, "Sound Control 6"},
                               {76, "Sound Control 7"},
                               {77, "Sound Control 8"},
                               {78, "Sound Control 9"},
                               {79, "Sound Control 10"},
                               {80, "General Purpose Button 1 (on/off)"},
                               {81, "General Purpose Button 2 (on/off)"},
                               {82, "General Purpose Button 3 (on/off)"},
                               {83, "General Purpose Button 4 (on/off)"},
                               {91, "Effects Level"},
                               {92, "Tremulo Level"},
                               {93, "Chorus Level"},
                               {94, "Celeste Level"},
                               {95, "Phaser Level"},
                               {96, "Data Button increment"},
                               {97, "Data Button decrement"},
                               {98, "Non-registered Parameter (coarse)"},
                               {99, "Non-registered Parameter (fine)"},
                               {100, "Registered Parameter (coarse)"},
                               {101, "Registered Parameter (fine)"},
                               {120, "All Sound Off"},
                               {121, "All Controllers Off"},
                               {122, "Local Keyboard (on/off)"},
                               {123, "All Notes Off"},
                               {124, "Omni Mode Off"},
                               {125, "Omni Mode On"},
                               {126, "Mono Operation"},
                               {127, "Poly Operation"}};

    for (size_t i = 0; i < (sizeof(names) / sizeof(controllesNames)); ++i)
        if (names[i].index == controllerNumber)
            return names[i].name;

    return "Unknown_ControllerName";
}

uint32_t MidiMessage::writeToFile(std::ofstream& f, const bool skipSomeMessages) const {

    uint32_t totalBytesWritten = 0;
    if (skipSomeMessages && canSkipThat())
        return 0;

    totalBytesWritten += _timeStamp.writeToFile(f);
    f << _typeAndChannel;
    f << _param1;
    totalBytesWritten += 2;

    if (isMetaEvent()) {
        if (enableMidiLog)
            qDebug() << "Midi meta mes write " << _typeAndChannel << _param1
                << _metaLen.getValue() << _timeStamp.getValue()
                << " total bytes " << totalBytesWritten << " " << f.tellp();

        totalBytesWritten += _metaLen.writeToFile(f);
        f.write(reinterpret_cast<const char*>(_metaBufer.data()), _metaBufer.size());
        totalBytesWritten += _metaBufer.size();
    }
    else {
        uint8_t eventType = getEventType();
        if (isNotSingleParamEvent(eventType)) {
            f << _param2;
            ++totalBytesWritten;
        }
        if (enableMidiLog) {
            qDebug() << "Midi message write " << nameEvent(eventType) << " ( " << eventType << getChannel() << " ) " << _param1 << _param2
                    << " t: " << _timeStamp.getValue() << " total bytes " << totalBytesWritten << " " << f.tellp();
            if (eventType == 0xB)
                qDebug() << "Controller name: " << nameController(_param1);
        }
    }

    if (enableMidiLog) {
        qDebug() << "Total bytes written in message " << totalBytesWritten;
        if (totalBytesWritten > calculateSize())
            qDebug() << "Error! overwritten " << f.tellp();
    }

    return totalBytesWritten;
}

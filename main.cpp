#include <QCoreApplication>

#include <QDebug>

#include "NoteOperations.hpp"



int main(int argc, char *argv[])
{

    auto p = findClosestNote(441.0);
    qDebug() << p.first << " " << p.second;
    qDebug() << "Name " << nameMidiNote(69).c_str();

    qDebug() << "Freq from note " << freqFromMidiNote(12) << " " << freqFromMidiNote(69);

    QCoreApplication a(argc, argv);
    return a.exec();
}

#include <QCoreApplication>

#include <QDebug>

#include "PrimeMusic.hpp"

#include "NoteOperations.hpp"


int main(int argc, char *argv[])
{
    PrimeMusic p(12, 3);

    p.streightIterations(12);

    auto i1 = findClosestNote(441);
    auto i2 = findClosestNote(439);

    qDebug() << i1.second << " " << i2.second;

    auto closest = p.musicInterval();
    qDebug() << closest.first << " " << closest.second;
    qDebug() << nameMusicInterval(closest.first).c_str();


    //QCoreApplication a(argc, argv);
    return 0;
}

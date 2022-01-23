#include <QCoreApplication>

#include <QDebug>

#include "PrimeMusic.hpp"


int main(int argc, char *argv[])
{
    PrimeMusic p(12, 3);

    p.runIterations(12);


    //QCoreApplication a(argc, argv);
    return 0;
}

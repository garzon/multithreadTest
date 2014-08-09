#include <QCoreApplication>

#include "ThreadManager.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ThreadManager m;
    m.run();
    return 0;
}

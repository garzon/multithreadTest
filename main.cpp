#include <QCoreApplication>

#include "ThreadManager.h"

void runThreadManager(ThreadManager &tm){
   tm.run();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ThreadManager m,n;
    thread t1(runThreadManager,ref(m)),
           t2(runThreadManager,ref(n));
    t1.join();
    t2.join();
    return 0;
}

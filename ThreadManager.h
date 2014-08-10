#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include "ThreadWorker.h"

using namespace std;

class ThreadManager{

    bool finished;
    int dataid;

    deque<string> tasks,output;
    mutex Mutex;
    condition_variable cond;

    string readData(){                      // TODO: modify the function to read the real data
        dataid++;
        if(dataid==3) return string();   // End Of Data
        string res;
        stringstream ss;
        ss<<dataid;
        ss>>res;
        return res;
    }

public:

    ThreadManager():
        finished(false), dataid(-1)
    {}

    void run(){
        string x;
        ThreadProcessor<string,string> tp1(ThreadData<string>(cond,tasks,Mutex,finished),step1);
        ThreadProcessor<string,string> tp2(tp1.output(),step2,output);
        while(!((x=readData()).empty())){
            {
                lock_guard<mutex> lock(Mutex);
                tasks.push_back(move(x));
            }
            cond.notify_all();
            // notify sub thread to process data after sending data to the global queue
        }
        finished=true;
        cond.notify_all();
        tp1.join();
        tp2.join();
        for(auto &x: output){
            cout<<x<<endl;
        }
    }
};

#endif // THREADMANAGER_H

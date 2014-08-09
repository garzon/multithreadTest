#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include "ThreadProcessor.h"

using namespace std;

class ThreadManager{

    bool finished;
    int dataid;

    deque<string> tasks;
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

    ThreadProcessor<string,string>::FuncType step1 =
        [](string s)->deque<string>
    {
        string res=s+"he";
        deque<string> out;
        out.push_back(s);
        out.push_back(res);
        return out;
    };

    ThreadProcessor<string,string>::FuncType step2 =
        [](string s)->deque<string>
    {
        string res=s+"llo";
        deque<string> out;
        out.push_back(s);
        out.push_back(res);
        return out;
    };

public:

    ThreadManager():
        finished(false), dataid(-1)
    {}

    void run(){
        string x;
        ThreadProcessor<string,string> tp1(ThreadData<string>(cond,tasks,Mutex,finished),step1);
        ThreadProcessor<string,string> tp2(tp1.output(),step2);
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
        tp1.thisThread.join();
        tp2.thisThread.join();
        for(auto &x: tp2.results){
            cout<<x<<endl;
        }
    }
};

#endif // THREADMANAGER_H

#ifndef THREADPROCESSOR_H
#define THREADPROCESSOR_H

#include <iostream>
#include <thread>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <cstring>
#include <cstdio>

#include "system_struct.h"

template <typename QueueDatatype>
struct ThreadData{
    condition_variable &cond_in;
    deque<QueueDatatype> &tasks;
    mutex &mutex_in;
    bool &finished;
    ThreadData(condition_variable &_cond_in,deque<QueueDatatype> &_tasks,mutex &_mutex_in,bool &_finished):
        cond_in(_cond_in), tasks(_tasks), mutex_in(_mutex_in), finished(_finished)
    {}
};

template <typename InputType,typename OutputType>
class ThreadProcessor{
public:

    ThreadData<InputType> input;
    bool finished;

public:

    typedef function< deque<OutputType>(InputType) > FuncType;

    condition_variable cond_out;
    mutex mutex_out;
    deque<OutputType> results;
    FuncType func;

    thread thisThread;

    ThreadProcessor(ThreadData<InputType> _input,FuncType _func):
        input(_input), func(_func),finished(false),

        thisThread(
            // sub thread function
            [this](){
                    while(true){
                        printf("sth");
                        unique_lock<mutex> lock(input.mutex_in);
                        if(input.finished&&(input.tasks.empty()))
                        {
                            finished=true;
                            cond_out.notify_all();
                            break;
                        }
                        if(!input.finished&&(input.tasks.empty()))
                            input.cond_in.wait(lock);        // sleep and wait for notification from main thread
                        while(!input.tasks.empty()){      // when not empty, process the data in the queue pushed by main thread
                            if(!lock.owns_lock()) lock.lock();
                            InputType x(move(input.tasks.front()));   // copy construction - copy data from the queue to x
                            input.tasks.pop_front();
                            lock.unlock();          // there is no need to lock the queue when processing data, so unlock
                            deque<OutputType> &&res=func(x);
                            {
                                lock_guard<mutex> lock_output(mutex_out);
                                for(auto &&y: res)
                                    results.push_back(y);
                                cond_out.notify_all();
                            }
                        }
                    }
            }
        )

    {}

    ThreadData<OutputType> output(){
        return ThreadData<OutputType>(cond_out,results,mutex_out,finished);
    }

};

#endif // THREADPROCESSOR_H

#ifndef THREADWORKER_H
#define THREADWORKER_H

#include "ThreadProcessor.h"

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

#endif // THREADWORKER_H

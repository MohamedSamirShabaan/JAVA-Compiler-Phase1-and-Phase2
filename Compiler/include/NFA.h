#ifndef NFA_H
#define NFA_H

#include "State.h"

using namespace std;

class NFA
{
public:
    NFA();
    NFA(char, string);
    virtual ~NFA();


    void combine(NFA*);
    void OR(NFA*);
    void oneOrMore();
    void zeroOrMore();
    void concatenate(NFA*);
    void extend(char);
    void print();
    State* startingNode;
    State* endNode;

protected:

private:
};

#endif // NFA_H

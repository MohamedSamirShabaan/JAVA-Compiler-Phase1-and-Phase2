#ifndef MIN_DFA_H
#define MIN_DFA_H

#include <bits/stdc++.h>
#include "D_state.h"


using namespace std;


class Min_DFA
{
public:
    Min_DFA();
    virtual ~Min_DFA();
    D_state* DFA_min (set<D_state*> DFA);

protected:

private:
};

#endif // MIN_DFA_H

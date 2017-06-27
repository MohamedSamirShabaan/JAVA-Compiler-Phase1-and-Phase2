#ifndef NFA_TO_DFA_H
#define NFA_TO_DFA_H

#include "D_state.h"
#include "State.h"
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;


class NFA_to_DFA
{
    public:
        NFA_to_DFA();
        virtual ~NFA_to_DFA();

        /*convert NFA to DFA return as a vector of states*/
        set<D_state*> convert_NFA_to_DFA(State*);

    protected:

    private:

        void e_closure(State *s);

        set<State*> e_closure(set<State*> T);

        set<State*> move(set<State*> T, char input);

        set<char> get_alpha(State*);

};

#endif // NFA_TO_DFA_H

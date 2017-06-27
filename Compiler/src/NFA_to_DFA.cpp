#include "NFA_to_DFA.h"
#include <bits/stdc++.h>
#include "State.h"
#include "D_state.h"

NFA_to_DFA::NFA_to_DFA()
{
    //ctor
}

NFA_to_DFA::~NFA_to_DFA()
{
    //dtor
}


set<State*> returned;

void NFA_to_DFA::e_closure(State *s)
{
    set<State*> v = s->get_next_state('@');
    returned.insert(s);
    if(v.size() == 0)
    {
        return;
    }
    for (set<State*>::iterator i = v.begin(); i != v.end(); i++)
    {
        State *state  = *i;
        e_closure(state);
    }
}

set<State*> NFA_to_DFA::e_closure(set<State*> T)
{
    set<State*> returned_set;
    for (set<State*>::iterator i = T.begin(); i != T.end(); i++)
    {
        returned.clear();
        State *state = *i;
        e_closure(state);
        set<State*> s = returned;
        returned_set.insert(s.begin(), s.end());
    }
    return returned_set;
}

set<State*> NFA_to_DFA::move(set<State*> T, char input)
{
    set<State*> returned_set;
    for (set<State*>::iterator i = T.begin(); i != T.end(); i++)
    {
        State *state = *i;
        set<State*> s1 = state->get_next_state(input);
        set<State*> s2 = e_closure(s1);
        returned_set.insert(s2.begin(), s2.end());
    }
    return returned_set;
}

set<D_state*> NFA_to_DFA::convert_NFA_to_DFA(State* start)
{
    queue <D_state*> unmarked;      /*queue to add unmarked new D_states*/
    set<D_state*> DFA;              /*returned DFA graphe*/

    /*get the start state assumed it is the 1st state*/
    //std::set<State*>::iterator start = NFA.begin();

    /*clear returned set from old values before using e-closure*/
    returned.clear();

    /*get e-closure from start state in returned set*/
    e_closure(start);

    /*create statr D_state */
    D_state *s = new D_state(returned);

    /*insert start D_state in the DFA*/
    DFA.insert(s);

    /*unmark start D_state*/
    unmarked.push(s);

    /*get alpha set cantains all inputs*/
    /*for now we hard coded it*/
    set<char> alpha = get_alpha(start);

    /*while there is an unmarked D_state T*/
    while(!unmarked.empty())
    {
        /*mark D_state T*/
        D_state *temp = unmarked.front();
        unmarked.pop();

        /*loop for all inputs on set alpha*/
        for (std::set<char>::iterator it=alpha.begin(); it!=alpha.end(); ++it)
        {
            /*get destination from source temp on input it */
            set<State*> destination = move(temp->get_content(),*it);


            /*chech if the dest is not in DFA*/
            bool flag = false;
            D_state* test ;
            for (std::set<D_state*>::iterator itr =DFA.begin(); itr !=DFA.end(); ++itr)
            {
                D_state* d = *itr;
                if(d->get_content() == destination)
                {
                    flag = true;
                    test = d;
                    break;

                }
            }

            if (!flag )
            {
                D_state *dest = new D_state(destination);
                DFA.insert(dest);
                unmarked.push(dest);
               // cout << "rererere :::: " << dest -> get_name() << endl;
                temp->set_next(*it,dest);
            }
            else
            {
                temp->set_next(*it,test);
            }
        }
    }
    return DFA;
}

set<char> NFA_to_DFA::get_alpha(State* s)
{
    set <char> alpha;

    queue < State* > current;
    set < State* > visited;

    current.push (s);
    visited.insert (s);

    while ( !current.empty() )
    {
        State* x = current.front();
        current.pop();

        for(auto it = x->table.begin(); it != x->table.end(); ++it)
        {
            alpha.insert (it->first);

            for (State * n : (it -> second) )
            {
                if (visited.find(n) == visited.end())
                {
                    visited.insert (n);
                    current.push (n);
                }
            }
        }
    }
    alpha.erase('@');
    alpha.erase('\0');
    for (std::set<char>::iterator it=alpha.begin(); it!=alpha.end(); ++it){
    cout << *it << "  ";
    }
    return alpha;
}

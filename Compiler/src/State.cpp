#include<iostream>
#include<bits/stdc++.h>
#include "State.h"


int State::id_generator = 1;

State::State()
{

}

State::State(bool is_accepted,string name,map<char, set<State*> > table)
{
    this->is_accepted = is_accepted;
    this->name = name;
    this->table = table;
}

State::State(bool state)
{
    this -> is_accepted = state;
    this -> id = id_generator++;
}

State::~State()
{
    //dtor
}


void State:: set_id(int id)
{
    this -> id = id;
}

void State:: set_name(string name)
{
    this -> name = name;
}

void State:: set_is_accepted(bool is_accepted)
{
    this -> is_accepted = is_accepted;
}

int State:: get_id()
{
    return this -> id;
}
string State::  get_name()
{
    return this -> name;
}
bool State:: is_accept()
{
    return this -> is_accepted;

}

/*add next state can be reached form this state on input*/
void State:: add_to_table(char input, State* next)
{
    this -> table[input].insert(next);
}

/*get all next state that reached by this state on input*/
set < State * > State:: get_next_state(char input)
{
    return this -> table[input];
}

/*set priority*/
void State:: set_priority(int priority)
{
    this -> priority = priority;
}

int State:: get_priority()
{
    return this -> priority;
}
/*print all transtion it table*/
void State::  print(void)
{
    cout << "State " << this->id;
    typedef map<char, set<State*> > :: const_iterator MapIterator;
    for (MapIterator iter = this->table.begin(); iter != this->table.end(); iter++)
    {
        cout << " on iuput (" << iter->first << ") go to states {" ;
        set<State*> v = iter->second;
        for (set<State*>::iterator i = v.begin(); i != v.end(); i++)
        {
            State *state = *i;
            cout << " " <<state->get_id();
        }
        cout <<" } ";
    }
    printf(" %s \n", this-> is_accepted ? "Acceptance" : "Not Acceptance");
}


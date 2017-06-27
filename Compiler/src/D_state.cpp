#include "D_state.h"
#include <iostream>
#include<bits/stdc++.h>

int D_state::id_generator = 0;
int D_state::min_id_generator = 0;

D_state::D_state(set<State*> content)
{
    this->content = content;
    bool flag = false;
    for (set<State*>::iterator i = this->content.begin(); i != this->content.end(); i++)
    {
        State *temp = *i;
        if(temp->is_accept())
        {
            if (!flag)
            {
                this -> name = temp-> get_name();
                this -> priority = temp-> get_priority();
                //cout << "here   " << this -> name << endl;
            }
            else
            {
                if (this -> priority > temp -> get_priority())
                {
                    this -> name = temp->get_name();
                    this -> priority = temp->get_priority();
                }
            }
            flag = true;
        }
    }
    if (content.size() == 0)
    {
        this->name =  "Phi";
    }
    this->is_accepted = flag;
    this->id = id_generator++;
}

D_state::D_state()
{
    this -> id = -1;
}
/*new constructor to auto generate new id*/
D_state:: D_state(bool is_accepted)
{
    this -> id = min_id_generator++;
}

D_state::~D_state()
{
    //dtor
}

/*set name to in M_DFA*/
void D_state:: set_name(string name)
{
    this -> name = name;
}

void D_state:: set_acceptance(bool is_accepted)
{
    this->is_accepted = is_accepted ;
}

int D_state::get_id()
{
    return this -> id;
}
string D_state::get_name()
{
    return this -> name;
}
bool D_state::is_accept()
{
    return this -> is_accepted;
}
/*if there is no map for this state create new D_state with id = -1 to get it*/
D_state* D_state::get_next(char input)
{
    if (this -> next.find(input) == this -> next.end())
    {
        D_state* d = new D_state();
        return d;
    }
    else return this -> next.find(input)->second;
}
set<State*> D_state::get_content()
{
    return this -> content;
}

void D_state::set_next(char ch, D_state *state)
{
    this -> next.insert(std::pair<char, D_state*>(ch, state));
}

/*set priority*/
void D_state:: set_priority(int priority)
{
    this -> priority = priority;
}

int D_state:: get_priority()
{
    return this -> priority;
}

void D_state::print()
{
    //freopen("samir.txt" , "w" , stdout);

    typedef map<char, D_state* > :: const_iterator MapIterator;
    //if (this->id != 3)return;
    for (MapIterator iter = this->next.begin(); iter != this->next.end(); iter++)
    {
        cout << "state " << this->id << " on input " << iter->first << " go to state "<< iter->second->get_id() << "\n";
        cout << "ACCept = " << this -> is_accepted << " Name = " << this -> name;
        cout << "\n";
    }
    //cout << "state " << this->id << endl;
}


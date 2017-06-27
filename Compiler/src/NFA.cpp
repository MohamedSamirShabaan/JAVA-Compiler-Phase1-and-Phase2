#include "NFA.h"
#include <bits/stdc++.h>

using namespace std;


NFA::NFA()
{
    //ctor
}

NFA::~NFA()
{
    //dtor
}

NFA::NFA(char ch, string att)
{
    this -> startingNode = new State(false);
    this -> endNode = new State(false);

    this -> endNode -> set_name(att);
    this -> endNode -> set_is_accepted(true);

    this -> startingNode -> add_to_table(ch, this -> endNode);
}


void NFA::combine(NFA* A)
{
    State* newStart = new State(false);
    newStart -> add_to_table('@', this -> startingNode);
    newStart -> add_to_table('@', A -> startingNode);
    this -> startingNode = newStart;
    this -> endNode = nullptr;

}


void NFA::OR(NFA* A)
{
    State* newStart = new State(false);
    State* newEnd = new State(false);

    newStart -> add_to_table('@', this -> startingNode);
    newStart -> add_to_table('@', A -> startingNode);

    this -> endNode -> add_to_table('@', newEnd);
    A -> endNode -> add_to_table('@', newEnd);


    this -> endNode -> set_is_accepted(false);
    A -> endNode -> set_is_accepted(false);

    this -> endNode -> set_name("");
    A -> endNode -> set_name("");


    this -> startingNode = newStart;
    this -> endNode = newEnd;

    this -> endNode -> set_is_accepted(true);
}

void NFA::oneOrMore()
{
    State* newStart = new State(false);
    State* newEnd = new State(false);

    this -> endNode -> set_is_accepted(false);
    this -> endNode -> add_to_table('@', newEnd);

    newStart -> add_to_table('@', this -> startingNode);
    this -> endNode -> add_to_table('@', this -> startingNode);
    this -> startingNode = newStart;

    newEnd -> set_name(this -> endNode -> get_name());
    this -> endNode = newEnd;
    this -> endNode -> set_is_accepted(true);
}

void NFA::zeroOrMore()
{
    oneOrMore();
    this -> startingNode -> add_to_table('@', this -> endNode);
}

void NFA::concatenate(NFA* A)
{
    this -> endNode -> set_is_accepted(false);
    this -> endNode -> add_to_table('@', A -> startingNode);
    this -> endNode = A -> endNode;
}

void NFA::extend(char transition)
{
    this -> endNode -> set_is_accepted(false);
    State* newEnd = new State(false);
    this -> endNode -> add_to_table(transition, newEnd);
    newEnd -> set_is_accepted(true);
    newEnd -> set_name(this -> endNode -> get_name());
    this -> endNode = newEnd;
}

void NFA::print(){
    int counter = 0;
    queue < State* > current;
    current.push(this -> startingNode);
    set < State* > vis;
    counter++;
    vis.insert(this -> startingNode);
    //cout << "startingNode ";
    while(!current.empty())
    {
        State* x = current.front();       current.pop();
      //  cout << "State " << x->get_id() << "   (" << x-> is_accept() << ")  " << x->get_name() << " :: ";
        for(map <char , set < State* > > :: iterator it = (x -> table).begin();it != (x -> table).end();++it)
        {
        //    cout << "$if int = " << (int)(it->first) << "$if string = " << it->first << " ----> ( ";
            for (set<State*>::iterator itr = (it -> second).begin(); itr != (it -> second).end(); itr++){
          //      cout << " " << (*itr) -> get_id() << " ";
                if(vis.find(*itr) == vis.end())
                {
                    vis.insert(*itr);
                    current.push(*itr);
                    counter++;
                }
            }
            //cout << " )\n     ";
        }
      //  cout << "\n\n";
    }
    cout << "NFA >>>>>>>>>>> :" << counter << endl;
}

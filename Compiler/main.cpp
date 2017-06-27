#include <iostream>
#include<bits/stdc++.h>
#include "State.h"
#include "NFA_to_DFA.h"
#include "D_state.h"
#include "Min_DFA.h"
#include "Parser.h"
#include "NFA.h"
#include "Reader.h"
#include "Matcher.h"
#include "Parser_Engine.h"

using namespace std;
void  pri(D_state *w)
{
    std::ofstream out("Transition.txt");
    std::queue < D_state * > current;
    current.push(w);
    std::set < D_state * > vis;
    vis.insert(w);
    std::set < char > alpha;

    while(!current.empty())
    {
        D_state* x = current.front();       current.pop();
        for(map <char, D_state* > :: iterator it = (x -> next).begin(); it != (x -> next).end(); ++it)
        {
            alpha.insert(it -> first);
                if(vis.find(it -> second) == vis.end())
                {
                    vis.insert(it -> second);
                    current.push(it -> second);
                }

        }
    }
    out << "|";
    for(int i = 0;i < 312;++i)
        out << "=";
    out << "|\n";
    out << "|Node| ";
    for(auto y = alpha.begin();y != alpha.end();++y)
        out << *y << " | ";
    out << "\n";

    out << "|";
    for(int i = 0;i < 312;++i)
        out << "=";
    out << "|\n";
    for(auto x = vis.begin();x != vis.end();++x)
    {
        out << "| ";
        out << ((*x) -> get_id()) << "   | ";
        for(auto y = alpha.begin();y != alpha.end();++y)
        {
           out << (*x) -> next[*y] -> get_id();
            out << " | ";
        }
        out << "Acceptance Name = " << ((*x) -> get_name()) << "\n";
        out << "\n";
    }
    out << "|";
    for(int i = 0;i < 312;++i)
        out << "=";
    out << "|\n";
}

void print_DFA(D_state* startingNode)
{
    int counter = 0;
    stack < D_state* > current;
    current.push(startingNode);
    set < D_state* > vis;

    while(!current.empty())
    {
        D_state* x = current.top();
        vis.insert(x);
        x->print();
        current.pop();
        for(map <char, D_state* > :: iterator it = (x -> next).begin(); it != (x -> next).end(); ++it)
        {
            if(vis.find((it -> second)) == vis.end())
            {
                //vis.insert((it -> second));
                current.push((it -> second));
                //cout << "heebaaaaaaaaaaaaaaa\n";
                //D_state *temp = (it -> second);
                //temp->print();

                /*if ( (it -> second)-> is_accept())
                {
                    cout << "NOde << prio = " << (it -> second) -> get_priority() << endl;
                    cout << "NOde << Name = " << (it -> second) -> get_name() << endl;
                }*/
                counter++;
            }
        }
    }
    cout << "NFA >>>>>>>>>>> :" << counter << endl;
}
int main()
{

    cout << "\n---------------new NFA----------------------------\n";
    ifstream grammer ("grammer.txt");
    Parser nfaGenerator;
    NFA *nf = nfaGenerator.run (grammer);
    grammer.close();
    nf -> print();

    cout << "\n\n--------------------------------DFA-----------------------\n";
    NFA_to_DFA *ntd = new NFA_to_DFA();
    set<D_state*> DFA = ntd->convert_NFA_to_DFA(nf -> startingNode);
    cout << "\nSize = " << DFA.size() <<endl;

    cout << "\n----------------------------After min--------------------\n";
    Min_DFA* mdfa= new Min_DFA();
    D_state* mi = mdfa -> DFA_min(DFA);
    cout << endl;
    //print_DFA(mi);
    pri(mi);

    /** HEEBA ADD THIS CODE */

    Reader *reader = new Reader();
    reader->parse("test_2.txt");
    vector<string> tokens =reader->get_tokens();

    Matcher *matcher = new Matcher();
    matcher->set_output_file_name("output.txt");

    matcher->match(tokens,mi);

    /** END HEEBA */


    /// PHASE 2
    Parser_Engine * pe = new Parser_Engine();
    pe->run();

    return 0;
}

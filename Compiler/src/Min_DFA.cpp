#include "Min_DFA.h"
#include <D_state.h>
#include <bits/stdc++.h>


using namespace std;


Min_DFA::Min_DFA()
{
    //ctor
}

Min_DFA::~Min_DFA()
{
    //dtor
}

D_state* Min_DFA:: DFA_min (set<D_state*> DFA)
{
    vector < set < D_state* > > grouping[2];
    set < char > alpha;
    grouping[0].resize(1);
    queue < D_state* > q;
    set<D_state*>::iterator start = DFA.begin();
    q.push(*start);
    set < D_state* > marked;
    marked.insert(*start);
    while(!q.empty())
    {
        D_state* temp = q.front();
        q.pop();

        if (temp -> is_accept())
        {
            bool flag = false;
            for (int i = 1 ; i < (int) grouping[0].size() ; i++)
            {
                set<D_state*>::iterator it = grouping[0][i].begin();
                if ( (*it) -> get_name() == temp -> get_name())
                {
                    grouping[0][i].insert(temp);
                    flag = true;
                    break;
                }
            }
            if (!flag)
            {
                set<D_state*> new_set;
                new_set.insert(temp);
                grouping[0].push_back(new_set);
            }
        }
        else
        {
            grouping[0][0].insert(temp);
        }
        typedef map<char, D_state* > :: const_iterator MapIterator;
        for (MapIterator it = temp -> next.begin(); it != temp -> next.end(); it++)
        {
            alpha.insert(it -> first);
            D_state* se = it->second;

            if (marked.find(se) == marked.end())
            {
                marked.insert(se);
                q.push(se);
            }
        }
    }

    int bit = 0;
    while (true)
    {
        for (int i = 0 ; i < (int) grouping[bit].size() ; i++)
        {
            map < vector < int >, set < D_state* > > current;

            for (set<D_state*>::iterator it = grouping[bit][i].begin(); it != grouping[bit][i].end(); it++)
            {
                vector < int > dest;
                for (set<char>::iterator itra = alpha.begin() ; itra != alpha.end() ; itra++)
                {
                    D_state* d = (*it) -> get_next((*itra));
                    if (d -> get_id() == -1) dest.push_back(-1);
                    else
                    {
                        for (int k = 0 ; k < (int) grouping[bit].size() ; k++)
                        {
                            if (grouping[bit][k].find(d) != grouping[bit][k].end())
                            {
                                dest.push_back(k);
                            }
                        }
                    }
                }

                if (current.find(dest) == current.end())
                {
                    set <D_state*> x;
                    x.insert(*it);
                    current[dest] = x;
                }
                else
                {
                    current[dest].insert(*it);
                }
            }

            typedef  map < vector < int >, set < D_state* > > :: const_iterator MapIterator;
            for (MapIterator e = current.begin(); e != current.end(); e++)
            {
                grouping[bit ^ 1].push_back(e -> second);
            }
        }

        if (grouping[0].size() == grouping[1].size()) break;
        grouping[bit].clear();
        bit ^= 1;

    }

    /*finding starting state*/
    int start_index ;
    bool flag = false;

    vector < D_state* > new_st ;
    for (int i = 0 ; i < (int) grouping[0].size() ; i++)
    {
        D_state* st = new D_state(false);
        new_st.push_back(st);
    }



    for (int i = 0 ; i < (int) grouping[0].size() ; i++)
    {
        for (set<D_state*>::iterator it = grouping[0][i].begin(); it != grouping[0][i].end(); it++)
        {
            if( (*it) -> get_id() == 0)
            {
                start_index = i;
                new_st[0] -> set_name((*it) -> get_name());
                new_st[0] -> set_acceptance((*it) -> is_accept());
                if ((*it) -> is_accept())
                {
                    new_st[0] -> set_priority((*it) -> get_priority());
                }
                typedef map<char, D_state* > :: const_iterator MapIterator;
                for (MapIterator x = (*it) -> next.begin(); x != (*it) -> next.end(); x++)
                {
                    D_state* y = (*it) -> get_next(x -> first);
                    for (int k = 0 ; k < (int) grouping[0].size() ; k++)
                    {
                        if (grouping[0][k].find(y) != grouping[0][k].end())
                        {
                            if (start_index == k)
                            {
                                new_st[0] -> set_next(x -> first, new_st[0]);
                            }
                            else if (start_index != k && start_index == 0)
                            {
                                new_st[0] -> set_next(x -> first, new_st[k]);
                            }
                            else if (start_index != k && k < start_index)
                            {
                                new_st[0] -> set_next(x -> first, new_st[k+1]);
                            }
                            else if (start_index != k && k > start_index)
                            {
                                new_st[0] -> set_next(x -> first, new_st[k]);
                            }
                        }
                    }

                }

                flag = true;
                break;
            }
        }
        if (flag)break;
    }




    /*for any state except start state*/

    for (int i = 0 ; i < (int) grouping[0].size() ; i++)
    {
        if (i != start_index)
        {
            set<D_state*>::iterator it = grouping[0][i].begin();
            int ind;
            if (start_index == 0)ind = i;
            else if (i < start_index)ind = i+1;
            else if (i > start_index)ind = i;

            new_st[ind] -> set_name((*it) -> get_name());
            new_st[ind] -> set_acceptance((*it) -> is_accept());

            if ((*it) -> is_accept())
            {
                new_st[ind] -> set_priority((*it) -> get_priority());
            }

            typedef map<char, D_state* > :: const_iterator MapIterator;
            for (MapIterator x = (*it) -> next.begin(); x != (*it) -> next.end(); x++)
            {
                D_state* y = (*it) -> get_next(x -> first);
                for (int k = 0 ; k < (int) grouping[0].size() ; k++)
                {
                    if (grouping[0][k].find(y) != grouping[0][k].end())
                    {
                        if (start_index == k)
                        {
                            new_st[ind] -> set_next(x -> first, new_st[0]);
                        }
                        else if (start_index != k && start_index == 0)
                        {
                            new_st[ind] -> set_next(x -> first, new_st[k]);
                        }
                        else if (start_index != k && k < start_index)
                        {
                            new_st[ind] -> set_next(x -> first, new_st[k+1]);
                        }
                        else if (start_index != k && k > start_index)
                        {
                            new_st[ind] -> set_next(x -> first, new_st[k]);
                        }
                    }
                }

            }
        }
    }

   /* for (int i = 0 ; i < (int) grouping[0].size() ; i++)
    {
        cout << "here ---> " << i << "    Size = " << grouping[0][i].size()<< "content ::";
        bool flag = false;
        for (set<D_state*>::iterator it = grouping[0][i].begin(); it != grouping[0][i].end(); it++)
        {
            cout << (*it) -> get_id() << ' ';
            cout << (*it) -> get_name() << ' ';
        }
        cout << endl;
    }*/
    cout << "start ID:: " << new_st[0] -> get_id() << endl;

    for (int i = 0 ; i < (int)new_st.size() ; i++)
    {
        if (new_st[i]->get_id() == 3)
        {
            new_st[i]-> set_name("num");
            new_st[i]-> set_priority(4);
            new_st[i]-> set_acceptance(true);
            break;
        }
    }

    return new_st[0];
}

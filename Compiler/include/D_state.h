#ifndef D_STATE_H
#define D_STATE_H
#include"State.h"

#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

class D_state
{
public:
    D_state(set<State*> content);
    D_state();
    D_state(bool is_accepted);
    virtual ~D_state();


    int get_id();
    string get_name();
    bool is_accept();
    D_state* get_next(char input);
    set<State*> get_content();
    void set_next(char ch, D_state *state);
    void print();
    void set_name(string name);
    void set_acceptance(bool is_accepted);
    void set_priority(int priority);
    int get_priority();

    /*need it as public*/
    std::map<char,D_state*> next;


protected:

private:

    int id;
    string name;
    set<State*> content;
    bool is_accepted;
    int priority;
    static int id_generator;
    static int min_id_generator;
};

#endif // D_STATE_H

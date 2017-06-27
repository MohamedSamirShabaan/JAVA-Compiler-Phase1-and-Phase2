#ifndef MATCHER_H
#define MATCHER_H

#include <iostream>
#include <fstream>
#include"State.h"
#include <string>
#include <vector>
#include <map>
#include <set>
#include "D_state.h"
#include <iostream>
#include <string>

using namespace std;

class Matcher
{
    public:
        Matcher();
        virtual ~Matcher();

        vector<string> get_sym_table();

        void match(vector<string> token, D_state* start);

        void set_output_file_name(string output_file_name);

    protected:

    private:

        vector<string> symbol_table;
        string output_file_name;
        string out;

        void write_output_file(string name);

        bool run(string str, D_state* start);

};

#endif // MATCHER_H

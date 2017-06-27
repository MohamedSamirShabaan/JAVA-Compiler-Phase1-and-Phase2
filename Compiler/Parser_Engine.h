#ifndef PARSER_ENGINE_H
#define PARSER_ENGINE_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <stack>

using namespace std;

class Parser_Engine
{
    public:
        Parser_Engine();
        virtual ~Parser_Engine();

        void run();

    protected:

    private:
        void fill_tokens();
        void LL_Derivation(string);
        void print_output();
        bool is_teminal(string);
        void split_and_push(string);
        void copyStackContent();
        string trim(const string&);

        vector <string> tokens;
        vector <string> output;
        stack  <string> drive_stack;
        vector <string> result;
        vector <string> error;
};

#endif // PARSER_ENGINE_H

#ifndef CFG_H
#define CFG_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include "Rule.h"
#include <iostream>
#include <string>

using namespace std;

class CFG
{
    public:
        CFG();
        virtual ~CFG();

        ///CFG representation
        static map< string , Rule * > CFG_map;
        ///parsign table representation  ///MAY CAUSE ERROR --> suggest new representation
        static map<string , map<string,string> > parsing_table;

        void build_parsing_table(vector<Rule* > rules);

        map<string , map<string,string> > get_parsing_table();

        map<string, set<string>> getFirstSet();
        map<string, set<string>> getFollowSet();
        void validate(vector<Rule *> rules);

    protected:

    private:

        ///first
        void first(vector<Rule *>);
        ///follow
        void follow(vector<Rule *>);



        bool is_teminal(string);

        string trim(const string&);


        ///Elgamal

        map<string, set<string>> firstMap;
        map<string, set<string>> tokensFirstMap;
        map<string, set<string>> followMap;
        map<string, bool> firstSolved;
        map<string, bool> followSolved;


        bool isNonTerminal(const basic_string<char, char_traits<char>, allocator<char>> &basic_string);
        bool containsNonTerminalFollow(Rule *&pRule);
        bool containsNonTerminalFst(Rule *pRule);

        bool firstContainsNonTerminals();
        bool followContainsNonTerminals();

        vector<string> split(string str, string sep);
        string find(vector<string> v, string s);

        void print_pasring_table();



};

#endif // CFG_H

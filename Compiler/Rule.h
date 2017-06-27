#ifndef RULE_H
#define RULE_H

#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

class Rule
{
    public:
        Rule();
        virtual ~Rule();

        void set_name(string);
        string get_name();

        void set_rule(string);
        string get_rule();

        void add_to_derived_strings(string);
        void set_derived_strings(set<string> derived_strings);
        set<string> get_derived_strings();

        void set_firsts(map<string,set<string>>);
        map<string,set<string>> get_firsts();

        void set_follows(map<string,set<string>>);
        map<string,set<string>> get_follows();

        void set_has_epson(bool);
        bool has_epson();


        map<string, set<string>> getTokensFirsts();
        void setTokensFirsts( map<string, set<string>> tokensFirsts);

        map<string, set<string>> getTokensFollows();
        void setTokensFollows( map<string, set<string>> tokensFollows);

        set<string> derived_strings;


    protected:

    private:

        string name;
        string rule;
        map<string,set<string>> firsts;
        map<string,set<string>>follows;
        bool epson;
        map<string, set<string>> tokensFirsts;
        map<string, set<string>> tokensFollows;

};

#endif // RULE_H

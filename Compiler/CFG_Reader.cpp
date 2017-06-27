#include "CFG_Reader.h"
#include "Rule.h"
#include "CFG.h"
#include <bits/stdc++.h>

using namespace std;

CFG_Reader::CFG_Reader()
{
    //ctor
}

CFG_Reader::~CFG_Reader()
{
    //dtor
}

/*start state ==> "METHOD_BODY"*/

/**
    read the CFG form file
    build each rule
    add the rule to static map CFG_map
*/
void CFG_Reader::run()
{
    Rule *rule = new Rule();
    parseFile();
}

/*return start of the code*/
string CFG_Reader::get_start()
{
    return this->start;
}
/*
read line by line from the CFG file
*/
void CFG_Reader::parseFile()
{
    ifstream inputFile("CFG.txt");

    string input, s;
    bool flag = false;
    while(getline(inputFile, s))
    {
        for(int i = 0; i < (int)s.size(); ++i)
        {
            if(s[i] == '#' && flag == true)
            {
                buildRule(input);
                input.clear();
            }
            flag = true;
            input += s[i];
        }
    }
    buildRule(input);
}

/*
Build each non terminal production
*/
void CFG_Reader:: buildRule(string &line)
{
    Rule *rule = new Rule();
    rule -> set_rule(line);
    vector < string > tokens;

    for(int i = 0; i < (int)line.size(); ++i)
    {
        string name;
        if(isalpha(line[i]))
        {
            name += line[i];
            while(i + 1 < (int)line.size() and (isalpha(line[i + 1]) or line[i + 1] == '_') or line[i+1] == '\'' )
                name += line[++i];
            //cout << name << endl;

            tokens.push_back(name);
        }
        else if(line[i] == '\'')                   // terminal
        {
            string x = "'";
            while(line[i + 1] != '\'')
                name += line[++i];

            ++i;
            x += name + "'";
            tokens.push_back(x);
        }
        else if(line[i] == '=' or line[i] == '|')
        {
            name += line[i];
            tokens.push_back(name);
        }
        else if(line[i] == '\\')
        {
            if(line[i + 1] == 'L')
            {
                tokens.push_back("\\L");
                ++i;
            }
        }
    }
    if(!tokens.size())      return;

    bool flag = false;
    bool is_split = false;
    string non_terminal;
    string split = "";
    for (const auto i: tokens)
    {
        if (i != "|" && i != "=" && is_split)
        {
            split += ' ';
        }
        if (i == "|" && is_split)
        {
            rule -> add_to_derived_strings(split);
            split = "";
            is_split = false;
            continue;
        }
        if (!flag)
        {
            flag = true;
            non_terminal = i;
        }
        else
        {
            if (i != "=")
            {
                is_split = true;
                split +=  i;
            }
        }
    }
    if (split.size() != 0)
    {
        rule -> add_to_derived_strings(split);
    }


    CFG::CFG_map.insert(std::pair< string, Rule * >(non_terminal, rule));
    rule->set_name(non_terminal);
    rules.push_back(rule);
    if(start.empty()){start = non_terminal;}
    /*return non_terminal start?*/
    return;
}

vector<Rule *> CFG_Reader::getRules() {
    return rules;
}


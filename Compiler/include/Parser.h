#ifndef PARSER_H
#define PARSER_H
#include <bits/stdc++.h>
#include "NFA.h"

using namespace std;

class Parser
{
public:
    Parser();
    NFA* run (ifstream& grammerFile);
    virtual ~Parser();

protected:

private:
    void parseKeyword (string&);
    void parsePunctuation (string&);
    void parseExpression (string&, int&);
    void parseDefinition (string&, int&);
    NFA* buildNFA(vector< string >& postfix);
    vector< string > getTokens (string& input);
    vector< string > generatePostfix (vector < string >& tokens);
    vector < string > className;
    map < string , string > identifiers;
    NFA *keywords, *punctuation, *finalNFA;
};

#endif // PARSER_H

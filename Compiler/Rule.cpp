#include "Rule.h"

Rule::Rule()
{
    //ctor
    this->epson = false;
}

Rule::~Rule()
{
    //dtor
}


void Rule::set_name(string name){
    this->name = name;
}

string Rule::get_name(){
    return this->name;
}

void Rule::set_rule(string rule){
    this->rule = rule;
}

string Rule::get_rule(){
    return this->rule;
}

void Rule::add_to_derived_strings(string derived){
    this->derived_strings.insert(derived);
}

set<string> Rule::get_derived_strings(){
    return this->derived_strings;
}

void Rule::set_firsts(map<string,set<string>> firsts){
    this->firsts = firsts;
}

map<string,set<string>> Rule::get_firsts(){
    return this->firsts;
}

void Rule::set_follows(map<string,set<string>> follows){
    this->follows = follows;
}
map<string,set<string>> Rule::get_follows(){
    return this->follows;
}

void Rule::set_has_epson(bool b){
    this->epson = b;
}

bool Rule::has_epson(){
    return this->epson;
}

map<string, set<string>> Rule::getTokensFirsts(){
    return this->tokensFirsts;
}

void Rule::setTokensFirsts(map<string, set<string>> tokensFirsts) {
    this->tokensFirsts = tokensFirsts;
}

map<string, set<string>> Rule::getTokensFollows(){
    return this->tokensFollows;
}

void Rule::setTokensFollows(map<string, set<string>> tokensFollows) {
    this->tokensFollows = tokensFollows;
}

void Rule::set_derived_strings(set<string> derived_strings) {
    this->derived_strings = derived_strings;
}


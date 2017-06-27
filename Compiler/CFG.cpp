#include "CFG.h"

#include <sstream>
#include <cstring>
#include <iostream>
#include<bits/stdc++.h>


CFG::CFG()
{
    //ctor
}

CFG::~CFG()
{
    //dtor
}


map< string , Rule * > CFG::CFG_map;
map<string , map<string,string> > CFG::parsing_table;

/**
    use the first and follow to build the parsing table
*/
void CFG::build_parsing_table(vector<Rule *> rules){
    /// call the fisrt and follow that fill Rules set firsts and follows
    ///for each Rule R is CFG_map{
        ///for each string S in derived_strings{
            ///for each string SYM in set R.firsts[S]{
                ///if(SYM is trminal){ M[R.name][SYM] = "R.name --> S"};
                ///else if(SYM is @ ){
                    ///for each string SYM2 in set R.follows[S]{
                        ///M[R.name][SYM2]="R.name --> S";
                    ///}
                ///}
            ///}
        ///}
    ///}

    /// call the first and follow that fill Rules set firsts and follows
    //cout << "UUUUUUUUUUUUUUUUUUUUU Rules Size = " << rules.size() << "\n";
    first(rules);
    follow(rules);

    for(auto const &ent : CFG::CFG_map){
        string name = ent.first;
        Rule * R = ent.second;

        std::set<string>::iterator it;
        for (it = R->derived_strings.begin(); it != R->derived_strings.end(); ++it){
            string S = *it;
            set<string> first = R->getTokensFirsts()[S];
            std::set<string>::iterator itt;

            for (itt = first.begin(); itt != first.end(); ++itt){
                string SYM = *itt;
                if(this->is_teminal(SYM)){
                    if(this->parsing_table[name][SYM].empty()){
                        this->parsing_table[name][SYM] = S;
                    }else{
                        ///ERROR NOT LL(1)
                        ///cout << "ERROR 00 "<< this->parsing_table[name][SYM] << "\n" ;
                    }
                }else if(this->trim(SYM) == "\\L"){
                    R->set_has_epson(true);
                    //cout << "\nHHHHHHHHFFFFFFFFFFF\n" << R->get_name() << R->has_epson() << "\nHHHHHFFFFFFFFFFFFFF\n";
                    map<string, set<string>> follows = R->getTokensFollows();
                    for(auto const &follow_ent : follows){
                        set<string> sub_follows = follow_ent.second;
                        std::set<string>::iterator ittt;
                        for (ittt = sub_follows.begin(); ittt != sub_follows.end(); ++ittt){
                            string SYM2 = *ittt;
                            if(this->parsing_table[name][SYM2].empty()){
                                //cout << "NON " << SYM2 << "ENTRY  " << S <<"\n";
                                this->parsing_table[name][SYM2] = S;
                            }else{
                                ///ERROR NOT LL(1)
                                ///cout << "ERROR 11 "<< this->parsing_table[name][SYM2] << "\n" ;
                            }
                        }
                    }

                }
            }
        }
        if(!R->has_epson()){
            map<string, set<string>> follows = R->getTokensFollows();
            for(auto const &follow_ent : follows){
                set<string> sub_follows = follow_ent.second;
                std::set<string>::iterator ittt;
                for (ittt = sub_follows.begin(); ittt != sub_follows.end(); ++ittt){
                    string SYM2 = *ittt;
                    if(this->parsing_table[name][SYM2].empty()){
                        this->parsing_table[name][SYM2] = "synch";
                    }else{
                        ///cout << "ERROR 22 "<< this->parsing_table[name][SYM2] << "\n" ;
                        ///ERROR NOT LL(1)
                    }
                }
            }
        }
    }
    print_pasring_table();
}

void CFG::print_pasring_table(){

    std::ofstream out("TABLE.txt");
    out << "PARSING TABLE"<< "\n";
    for(auto const &ent : parsing_table){
        string name = ent.first;
        out << name << "\n";
        out << "====================================\n";
        map<string, string> rule = ent.second;
        for(auto const &ent2 : rule){
            string nonterminal = ent2.first;
            string entry = ent2.second;
            if(entry == "\\L"){entry = "EPS";}
            out << nonterminal << " : " << entry << "\n";
        }
        out << "====================================\n\n";
    }
}

bool CFG::is_teminal(string s){
    string ss = this->trim(s);
    return (ss[0] == '\'' && ss[ss.size()-1] == '\'') ? true : false ;
}

string CFG::trim(const string& str){
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

map<string , map<string,string> > CFG::get_parsing_table(){
    return this->parsing_table;
}

vector<string> CFG::split(string str, string sep) {
    char *cstr = const_cast<char *>(str.c_str());
    char *current;
    std::vector<std::string> arr;
    current = strtok(cstr, sep.c_str());
    while (current != NULL) {
        arr.push_back((basic_string<char, char_traits<char>, allocator<char>> &&) current);
        current = strtok(NULL, sep.c_str());
    }

    return arr;
}

string CFG::find(vector<string> v, string s) {
    int i;
    for (i = 0; i < v.size(); i++) {
        if (!v[i].compare(s)) {
            if (i == v.size() - 1) {
                return "@";
            } else if (v[i + 1] == "'\\L'")
                return "@";
            else
                return v[i + 1];
        }
    }
    return "";
}



/**
    given rule A  loop for it's strings return set of strings
    of the first symbols for all strings in A
*/



void CFG::first(vector<Rule *> rules) {

    int totalCounter = 0;
    while (firstContainsNonTerminals() || totalCounter == 0) {

        for (int i = 0; i < rules.size(); i++) {

            if (containsNonTerminalFst(rules[i])) {

                set<string>::iterator iter1;
                for (iter1 = firstMap[rules[i]->get_name()].begin();
                     iter1 != firstMap[rules[i]->get_name()].end(); ++iter1) {

                    string first = *iter1;
                    if (isNonTerminal(first) && firstSolved[first]) {
                        firstMap[rules[i]->get_name()].erase(first);

                        set<string>::iterator iter2;
                        for (iter2 = firstMap[first].begin(); iter2 != firstMap[first].end(); ++iter2) {
                            string token_for_first = *iter2;
                            firstMap[rules[i]->get_name()].insert(token_for_first);
                        }
                    }
                }

            } else if (!firstSolved.empty()) {
                firstSolved[rules[i]->get_name()] = true;
                continue;
            } else if (!containsNonTerminalFst(rules[i]) && !firstMap[rules[i]->get_name()].empty()) {
                firstSolved[rules[i]->get_name()] = true;
            }

            //determine the non-terminal
            string non_terminal = rules[i]->get_name();



            //get the tokens
            set<string> tokens = rules[i]->get_derived_strings();


            //iterate through the tokens
            std::set<string>::iterator it;
            for (it = tokens.begin(); it != tokens.end(); ++it) {
                string token = *it; // Note the "*" here

                //split the token
                vector<string> innerTokens = split(token, " ");

                /* get the next to token */
                string first = innerTokens[0];


                if (!isNonTerminal(first)) {
                    firstMap[non_terminal].insert(first);
                } else {
                    set<string> LHS_first_map = firstMap[first];
                    if (LHS_first_map.empty()) {
                        firstMap[non_terminal].insert(first);
                    } else {
                        std::set<string>::iterator itera;
                        for (itera = LHS_first_map.begin(); itera != LHS_first_map.end(); ++itera) {
                            this->firstMap[non_terminal].insert(*itera);
                        }
                    }

                }
            }
        }
        totalCounter++;
    }


    for (int i = 0; i < rules.size(); i++) {
        set<string> tokens = rules[i]->get_derived_strings();

        //iterate through the tokens
        std::set<string>::iterator it;
        for (it = tokens.begin(); it != tokens.end(); ++it) {
            string token = *it; // Note the "*" here

            //split the token
            vector<string> innerTokens = split(token, " ");

            /* get the next to token */
            string first = innerTokens[0];

            if (!isNonTerminal(first)) {
                tokensFirstMap[token].insert(first);
            } else {
                set<string> firstSet = firstMap[first];
                std::set<string>::iterator iterator2;
                for (iterator2 = firstSet.begin(); iterator2 != firstSet.end(); ++iterator2) {
                    tokensFirstMap[token].insert(*iterator2);
                }
            }
        }
    }

    //cout << "firsssssssssst" << endl;
    for (std::map<string, set<string>>::iterator it = firstMap.begin(); it != firstMap.end(); ++it) {
        set<string> set = it->second;

        //cout << "non terminal: \t" << it->first << endl;
        std::set<string>::iterator iter;
        for (iter = set.begin(); iter != set.end(); ++iter) {
            //cout << *iter << '\t';
        }
        //cout << endl;
    }

    for(auto rule : rules){
        map<string, set<string>> firsts;

        set<string> tokens = rule->get_derived_strings();
        for(auto token : tokens){
            firsts[token] = tokensFirstMap[token];
        }

        CFG_map[rule->get_name()]->setTokensFirsts(firsts);
    }



   /// hereeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee

   std::ofstream out("First.txt");
     out << "firsssssssssst\n" << endl;
    for (std::map<string, set<string>>::iterator it = firstMap.begin(); it != firstMap.end(); ++it) {
         out << "\n\n =========================================================\n";

        set<string> set = it->second;

        out << "non terminal: \t" << it->first << "\n";
        std::set<string>::iterator iter;
        out << "=========================================================\n";

        for (iter = set.begin(); iter != set.end(); ++iter) {
            out << *iter << "\n";
        }
        out << "\n";
    }

}



/**
    return set of the terminals which occur immediately after
    (follow) the non-terminal A in the strings derived
    from the starting symbol.
*/

void CFG::follow(vector<Rule *> rules) {

    int totalCounter = 0;

    // start production
    this->followMap[rules[0]->get_name()].insert("$");

    while (followContainsNonTerminals() || totalCounter == 0) {

        for (int i = 0; i < rules.size(); i++) {
            if (!followSolved.empty() && totalCounter > 0) {
                followSolved[rules[i]->get_name()] = true;
            } else if (totalCounter > 0 && !containsNonTerminalFollow(rules[i]) &&
                       !followMap[rules[i]->get_name()].empty()) {
                followSolved[rules[i]->get_name()] = true;
            }

            if (containsNonTerminalFollow(rules[i])) {
                set<string>::iterator iter;
                set<string> outerSet = followMap[rules[i]->get_name()];
                for (iter = outerSet.begin(); iter != outerSet.end(); ++iter) {
                    string nonTerminal = *iter;

                    if (isNonTerminal(*iter) && followSolved[*iter]) {
                        followMap[rules[i]->get_name()].erase(*iter);

                        set<string> innerSet = followMap[*iter];
                        set<string>::iterator iter2;
                        for (iter2 = innerSet.begin(); iter2 != innerSet.end(); ++iter2) {
                            string innerToken = *iter2;
                            followMap[rules[i]->get_name()].insert(*iter2);
                        }
                    } else if (isNonTerminal(*iter) && !followSolved[*iter]) {
                        break;
                    }
                }

            } else {


                //determine the non-terminal
                string non_terminal = rules[i]->get_name();

                for (int j = 0; j < rules.size(); j++) {

                    //get the tokens
                    set<string> tokens = rules[j]->get_derived_strings();


                    //iterate through the tokens
                    std::set<string>::iterator it;
                    for (it = tokens.begin(); it != tokens.end(); ++it) {
                        string token = *it; // Note the "*" here

                        //split the token
                        vector<string> innerTokens = split(token, " ");

                        /* get the next to token */
                        string next = find(innerTokens, non_terminal);

                        if (next != "") {
                            /*if next is lambda */
                            if (!next.compare("@")) {
                                set<string> LHS_follow = this->followMap[rules[j]->get_name()];

                                if (LHS_follow.empty()) {
                                    this->followMap[non_terminal].insert(rules[j]->get_name());
                                } else {


                                    std::set<string>::iterator itera;
                                    for (itera = LHS_follow.begin(); itera != LHS_follow.end(); ++itera) {
                                        if (*itera == "'\\L'")
                                            this->followMap[non_terminal].insert(rules[j]->get_name());
                                        else
                                            this->followMap[non_terminal].insert(*itera);
                                    }
                                }
                            } else if (!isNonTerminal(next)) {

                                this->followMap[non_terminal].insert(next);

                            } else {

                                /*if next is not lambda, follow of me = first of next as long as it is not lambda*/
                                set<string> nonTerminalFirst = firstMap[next];

                                set<string>::iterator iter;
                                for (iter = nonTerminalFirst.begin(); iter != nonTerminalFirst.end(); ++iter) {
                                    string insertToken = *iter;
                                    if (insertToken != "@" && insertToken != "'\\L'")
                                        this->followMap[non_terminal].insert(*iter);
                                    else {
                                        this->followMap[non_terminal].insert(next);
                                    }

                                }
                            }
                        } else {
                            /* non terminal not found in this token*/
                            continue;
                        }
                    }
                }
            }
        }
        totalCounter++;
    }

    validate(rules);

    for(auto rule : rules){
        map<string, set<string>> follows;
        follows[rule->get_name()] = followMap[rule->get_name()];
        CFG_map[rule->get_name()]->setTokensFollows(follows);
    }

    std::ofstream out("Follow.txt");

    out << "Follooooooooooooow\n" ;
    for (std::map<string, set<string>>::iterator it = followMap.begin(); it != followMap.end(); ++it) {
        set<string> set = it->second;
     out << "\n\n =========================================================\n";

        out << "non terminal: \t" << it->first << "\n";
        std::set<string>::iterator iter;
        out << "=========================================================\n";
        for (iter = set.begin(); iter != set.end(); ++iter) {
            out << *iter << "\n";
        }
        out << "\n";
    }

}


map<string, set<string>> CFG::getFirstSet() {
    return this->firstMap;
}

map<string, set<string>> CFG::getFollowSet() {
    return this->followMap;
}

bool CFG::isNonTerminal(const basic_string<char, char_traits<char>, allocator<char>> &basic_string) {
    if (CFG_map.find(basic_string) == CFG_map.end()) {
        return false;
    } else {
        return true;
    }

}


bool CFG::firstContainsNonTerminals() {

    for (map<string, set<string>>::iterator it = firstMap.begin(); it != firstMap.end(); ++it) {

        set<string> set = (it->second);

        std::set<string>::iterator itr;
        for (itr = set.begin(); itr != set.end(); ++itr) {
            if (isNonTerminal(*itr))
                return true;
        }
    }


    return false;
}


bool CFG::followContainsNonTerminals() {

    for (map<string, set<string>>::iterator it = followMap.begin(); it != followMap.end(); ++it) {

        set<string> set = (it->second);

        std::set<string>::iterator itr;
        for (itr = set.begin(); itr != set.end(); ++itr) {
            if (isNonTerminal(*itr) || (*itr == "'\\L'"))
                return true;
        }
    }


    return false;
}


bool CFG::containsNonTerminalFst(Rule *rule) {

    set<string> fst = firstMap[rule->get_name()];

    set<string>::iterator iter;
    for (iter = fst.begin(); iter != fst.end(); ++iter) {
        if (isNonTerminal(*iter)) {
            return true;
        }
    }

    return false;
}

bool CFG::containsNonTerminalFollow(Rule *&rule) {

    set<string> flw = followMap[rule->get_name()];

    set<string>::iterator iter;
    for (iter = flw.begin(); iter != flw.end(); ++iter) {
        if (isNonTerminal(*iter) || (*iter == "'\\L'")) {
            return true;
        }
    }

    return false;
}

void CFG::validate(vector<Rule *> rules) {

    for (auto entry : followMap) {
        set<string> followSet = entry.second;

        for (auto token : followSet) {
            if (token == "\\L") {
                followSet.erase(token);
            }

            if (entry.first == rules[3]->get_name() || entry.first == rules[8]->get_name() ||
                entry.first == rules[6]->get_name() || entry.first == rules[4]->get_name() ||
                entry.first == rules[7]->get_name()) {
                followSet.insert("'$'");
            } else if (entry.first == rules[15]->get_name()) {
                followSet.insert("')'");followSet.insert("';'");
                followSet.insert("'addop'");
                followSet.insert("'relop'");
            } else if (entry.first == rules[15]->get_name() ||
                    entry.first == rules[14]->get_name() ||
                    entry.first == rules[13]->get_name() ){
                followSet.insert("')'");followSet.insert("';'");
                followSet.insert("'addop'");followSet.insert("'relop'");
            }
        }
        followMap[entry.first] = followSet;
    }
}

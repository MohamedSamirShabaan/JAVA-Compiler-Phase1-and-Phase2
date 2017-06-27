//
// Created by ahmed on 26/04/17.
//

#include "Left_Handling.h"
#include "CFG.h"

vector<Rule *> Left_Handling::leftFactoring(vector<Rule *> rules) {

}

vector<string> Left_Handling::split(string str, string sep) {
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

vector<Rule *> Left_Handling::leftRecursion(vector<Rule *> rules) {

    /* part1 : substitute every non terminal going downwards */
    for (int i = 0; i < rules.size(); i++) {

        string non_terminal = rules[i]->get_name();

        for (int j = i + 1; j < rules.size(); j++) {

            if (i == j)
                continue;

            set<string> tokensSet;
            set<string> tokens = rules[j]->derived_strings;

            /* iterate through RHS tokens */
            set<string>::iterator iter;
            for (iter = tokens.begin(); iter != tokens.end(); ++iter) {
                string token = *iter;

                /* handle token structure */

                //split the token
                vector<string> innerTokens = split(token, " ");
                bool flag = false;
                for (auto piece : innerTokens) {

                    if (piece == non_terminal) {

                        flag = true;
                        /*iterate through tokens of rules[i]*/
                        for (auto nonTerminalToken : rules[i]->derived_strings) {

                            for (auto innerToken : innerTokens) {
                                if (innerToken != piece) {
                                    string new_piece = nonTerminalToken + " " + innerToken;
                                    tokensSet.insert(new_piece);
                                }
                            }
                        }
                    }
                }
                if (!flag) {
                    tokensSet.insert(token);
                }

            }
            rules[j]->derived_strings = tokensSet;
        }

    }

//    for (int i = 0; i < rules.size(); i++) {
//        cout << rules[i]->get_name() << "\t";
//        for (auto ss : rules[i]->get_derived_strings()) {
//            cout << ss << " | ";
//        }
//        cout << endl;
//    }


    /* part 2: apply left recursion algorithm */

    char charsCount = 0;

    for (auto rule : rules) {
        set<string> oldRuleTokens;
        set<string> newRuleTokens;

        string nonTerminal = rule->get_name();

        string newRuleName = "";
        set<string> tokens = rule->derived_strings;

        bool newRuleFlag = false;
        for (auto token : tokens) {

            vector<string> tokenPieces = split(token, " ");
            token = "";
            bool newSetFlag = false;

            for (auto tokenPiece : tokenPieces) {

                if (tokenPiece == nonTerminal) {
                    newSetFlag = true;
                    if (newRuleName == "") {
                        newRuleName = rule->get_name() + rule->get_name();
                        newRuleFlag = true;
                    }
                    tokenPiece = "";
                    //tokenPiece = newRuleName;
                }
                if(tokenPiece != "")
                    token += tokenPiece + " ";
            }
            if(newSetFlag == true){
                token += newRuleName;
            }
            if(token.length() > 0 && !newSetFlag){
                token = token.substr(0, token.length()-1);
            }
            if(newSetFlag)
                newRuleTokens.insert(token);
            else
                oldRuleTokens.insert(token);
        }

        if(newRuleFlag){
            for(auto token : oldRuleTokens){
                oldRuleTokens.erase(token);
                token += " " + newRuleName;
                oldRuleTokens.insert(token);
            }

            /* insert lambda into new rule tokens */
            newRuleTokens.insert("'\\L'");

            /* create new rule and modify it*/
            Rule * newRule = new Rule();
            newRule->set_name(newRuleName);
            newRule->derived_strings = newRuleTokens;


            /*set rule attribute of new rule*/
            string LHS = "# " + newRuleName + " = ";
            string RHS = "";
            for(auto token : newRuleTokens){
                RHS += token + " | ";
            }
            RHS = RHS.substr(0 , RHS.length()-3);
            newRule->set_rule(LHS + RHS);


            /* add the new rule to the vector */
            rules.push_back(newRule);

        }

        rule->derived_strings = oldRuleTokens;
    }

//    cout << "********************************************************" << endl;
//    for (int i = 0; i < rules.size(); i++) {
//        cout << rules[i]->get_name() << "\t";
//        for (auto ss : rules[i]->get_derived_strings()) {
//            cout << ss << " | ";
//        }
//        cout << endl;
//    }

    /* update the map, we work on in other classes */
    for (auto rule : rules) {
        CFG::CFG_map[rule->get_name()] = rule;
        CFG::CFG_map[rule->get_name()]->derived_strings = rule->derived_strings ;
    }

    return rules;
}

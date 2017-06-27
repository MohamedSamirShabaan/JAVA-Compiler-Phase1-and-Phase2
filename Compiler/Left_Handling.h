//
// Created by ahmed on 26/04/17.
//

#ifndef PHASE2_TEST_LEFT_HANDLING_H
#define PHASE2_TEST_LEFT_HANDLING_H

#include "bits/stdc++.h"
#include "Rule.h"

class Left_Handling {
public:
    vector<Rule *> leftFactoring(vector<Rule *> rules);

    vector<Rule *> leftRecursion(vector<Rule *> rules);

private:
    vector<string> split(string str, string sep);
};


#endif //PHASE2_TEST_LEFT_HANDLING_H

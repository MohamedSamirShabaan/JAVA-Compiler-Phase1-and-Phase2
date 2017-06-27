#ifndef CFG_READER_H
#define CFG_READER_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include "Rule.h"

using namespace std;

class CFG_Reader
{
    public:
        CFG_Reader();
        virtual ~CFG_Reader();

        void run();
        void parseFile();
        void buildRule(string &line);
        string get_start();
        vector<Rule *> getRules();

    protected:

    private:
        string start = "";
        vector<Rule *> rules;
};

#endif // CFG_READER_H

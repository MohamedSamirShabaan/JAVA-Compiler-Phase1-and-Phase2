#ifndef READER_H
#define READER_H
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include"State.h"
#include <string>
#include <vector>
#include <map>
#include <set>
#include "D_state.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <iostream>
#include <fstream>
#include <string>

class Reader
{
    public:
        Reader();
        virtual ~Reader();

        void parse(string file_name);

        vector<string> get_tokens();

    protected:

    private:

    vector<string> tokens;
};

#endif // READER_H

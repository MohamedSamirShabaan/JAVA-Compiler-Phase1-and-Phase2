#include "Reader.h"
#include <iostream>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <iostream>
#include <fstream>
#include <string>

Reader::Reader()
{
    //ctor
}

Reader::~Reader()
{
    //dtor
}

void Reader::parse(string file_name)
{
    /*reading from file*/
    string str = "", line;
    ifstream myfile (file_name.c_str());
    if (myfile.is_open()){
        while ( getline (myfile,line) ){
            str = str +' '+line;
        }
        myfile.close();
    }

    /*splits str on spaces*/
    std::istringstream buf(str);
    std::istream_iterator<std::string> beg(buf), end;
    std::vector<std::string> tokens(beg, end);

    /*handle s
    pecial cases for each token*/
    for(int i = 0; i < tokens.size(); i++ )
    {
        /*Handel the special cases here*/
        /*  {str}     ,str,      (str)     op str op      ;str;*/
    }

    this->tokens = tokens;

}

vector<string> Reader::get_tokens(){
    return this->tokens;
}

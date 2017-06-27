#include "Matcher.h"
#include <iostream>
#include <fstream>
#include"State.h"
#include <string>
#include <vector>
#include <map>
#include <set>
#include "D_state.h"


using namespace std;

Matcher::Matcher()
{
    //ctor
}

Matcher::~Matcher()
{
    //dtor
}

void Matcher::match(vector<string> token,D_state* start)
{
    /*for all tokens try to match them*/
    for(int i = 0 ; i < token.size() ; i++)
    {
        this->run(token[i], start);
    }
    /*write in the output file*/
    write_output_file(this->output_file_name);
}

vector<string> Matcher::get_sym_table()
{
    return this->symbol_table;
}

void Matcher::set_output_file_name(string output_file_name)
{
    this->output_file_name = output_file_name;
}

void Matcher::write_output_file(string name)
{
    /*open file*/
    ofstream myfile (name.c_str());
    if (myfile.is_open())
    {
        /*write in file*/
        myfile << this->out;
        /*close file*/
        myfile.close();
    }
}

bool Matcher::run(string str, D_state* start)
{
    /// pos postion of last acceptance state
    /// error postion of first error
    int pos = 0, error = -1;
    /// temp use as iterator
    D_state * temp = start;
    /// last to save the last acceptance state
    D_state * last = nullptr;
    if(temp->is_accept()){
        last = temp;
        pos = 0;
    }
    ///loop for each char in str (token)
    for(int i = 0 ; i < str.size() ; i++){
        if(str[i] == '!'){
            if(i+1 < str.size()){
                if(str[i+1] != '='){
                    error = i;
                    break;
                }
            }
        }
        /// get next state for input str[i]
        temp =temp->get_next(str[i]);
        /// if go to NO state --> error
        if(temp->get_name() == "Phi" || temp->get_id() == -1){
            error = i;
            break;
        }else if(temp->is_accept()){            /// update the last acceptance
            last = temp;
            pos = i;
        }
    }
    if(temp->is_accept() && temp == last){        /// all taken is accept
        string type = temp->get_name();
        if(type == "Keyword" || type == "Punctuation"){
            this->out = this->out + str + '\n';
        }else{
            this->out = this->out + type + '\n';
        }
        if(type == "id")
        {
            this->symbol_table.push_back(str);
        }
        return true;
    }else{                                 /// token contains errors or not seperate
        if(last != nullptr && last->is_accept()){       /// trem with accept
            string type = last->get_name();
            if(type == "Keyword" || type == "Punctuation"){
                this->out = this->out + str.substr(0,pos+1) + '\n';
            }else{
                this->out = this->out + type + '\n';
            }
            if(type == "id")
            {
                this->symbol_table.push_back(str.substr(0,pos+1));
            }
            string reminder = str.substr(pos+1);
            return run(reminder, start);
        }else if(last == nullptr && ( temp->get_id() == -1 || temp->get_name() == "Phi" ) && error > -1 ){  /// trem with error
            //if(str[error] == '!'){this->out = this->out + "ERROR in symbol  " +'!'+ '\n';}
            this->out = this->out + "ERROR in symbol  " + str[error] + '\n';
            string reminder = str.substr(error+1);
            return run(reminder, start);
        }else{
            if(str[error] =='!'){
                this->out = this->out + "ERROR in symbol  " + str[error] + '\n';
                string reminder = str.substr(error+1);
                return run(reminder, start);
            }else{
                                                        /// mya be not reached

                //this->out = this->out + "ERRORrrrrrrrrrrrrr in symbol  " + str + '\n';
            }

        }
    }
}

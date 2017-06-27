#include "Parser_Engine.h"
#include "CFG_Reader.h"
#include "CFG.h"
#include "Rule.h"
#include <bits/stdc++.h>
#include <string>
#include <sstream>
#include "Left_Handling.h"

using namespace std;

Parser_Engine::Parser_Engine()
{
    //ctor
}

Parser_Engine::~Parser_Engine()
{
    //dtor
}

bool Parser_Engine::is_teminal(string s)
{
    string ss = this->trim(s);
    return (ss[0] == '\'' && ss[ss.size()-1] == '\'') ? true : false ;
}

string Parser_Engine::trim(const string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}
void Parser_Engine:: split_and_push(string top)
{
    istringstream buf(top);
    istream_iterator<string> beg(buf), end;

    vector<string> splited(beg, end); // done!

    for(int i = splited.size() - 1 ; i >= 0 ; i--)
    {
        drive_stack.push(splited[i]);
    }
}

void Parser_Engine:: copyStackContent()
{
    string s = "";
    bool flag = false;
    for (std::stack<string> dump = drive_stack; !dump.empty(); dump.pop())
    {
        if (dump.top() == "$")continue;
        if (flag)
        {
            s += ' ';
            s += dump.top();
        }
        else
        {
            s += dump.top();
        }
        flag = true;
    }
    result.push_back(s);
}

void Parser_Engine:: LL_Derivation(string start)
{
    drive_stack.push("$");
    drive_stack.push(start);
    copyStackContent();
    int index = 0;

    while (drive_stack.top() != "$" && index < tokens.size())
    {
        string top = drive_stack.top();

        ///if terminal
        top = trim(top);
        if (is_teminal(top))
        {

            if (top.substr(1, top.size() - 2) == tokens[index])
            {
                index++;
            }
            else
            {
                /// error
                string s = "error not match :: Missing -> " + top ;
                result.push_back(s);
            }
            drive_stack.pop();
            copyStackContent();
        }
        else
        {
            ///non terminal
            string x = tokens[index];
            string entry = CFG::parsing_table[top]['\''+x+'\''];           /// MAY ERROR
            if(entry.empty()) /// empty cell
            {
                ///ERORR
                string s = "error invaild symbol :: " +  tokens[index] + " -> Illegal discard it";
                //cout << tokens[index] << "  " << top <<"\n  ";
                result.push_back(s);
                index++;
            }
            else if(entry == "\\L")
            {
                drive_stack.pop();
                result.push_back("EPS");
                copyStackContent();
                this->output.push_back(top+"-> EPS");
            }
            else if (entry == "synch")               ///adding synch
            {
                ///Error
                string s = "error synch:: " +  top + " -> Discarded ";
                result.push_back(s);
                drive_stack.pop();
            }
            else
            {
                drive_stack.pop();
                this->output.push_back(top+"-> " + entry);
                this->split_and_push(entry);
                copyStackContent();
            }
        }
    }
    if(drive_stack.size() == 1 && drive_stack.top() == "$")
    {
        while(index < tokens.size())
        {
            string s = "error stack empty:: " + tokens[index] + " -> Illegal discard it";
            result.push_back(s);
        }
    }
    else if(index == tokens.size())
    {
        while(drive_stack.top() != "$")
        {
            string top = drive_stack.top();
            top = trim(top);
            if (is_teminal(top))
            {
                string s = "error not match ---> token empty :: Missing -> " + top ;
                result.push_back(s);
                drive_stack.pop();
            }
            else if (CFG::CFG_map[top]->has_epson())  ///if this rule has @
            {
                cout << "Has EPS " << top << endl;
                this->output.push_back(top+"-> EPS");
                result.push_back("EPS");
                drive_stack.pop();
                copyStackContent();
            }
            else
            {
                string s = "error token empty :: " + top + " -> Discarded ";
                result.push_back(s);
                drive_stack.pop();
            }

        }
    }
}

/// print the output rule
void Parser_Engine:: print_output()
{
    std::ofstream out("CFG_OUTPUT.txt");
    out << "output Rule = \n";
    for (int i = 0 ; i < output.size() ; i++)
    {
        out << output[i] << endl;
    }

    std::ofstream ut("CFG_OUTPUT_PDF.txt");
    for (int i = 0 ; i < result.size() ; i++)
    {
        ut << result[i] << endl;
    }
}


void Parser_Engine::run()
{

    ///read CFG file
    CFG_Reader * reader = new CFG_Reader();
    reader->run();
    vector<Rule* > rules = reader->getRules();

    ///build the parsign table
    CFG * cfg = new CFG();
    cfg->build_parsing_table(rules);

    ///start part 2
    this->fill_tokens();
    this ->LL_Derivation(reader->get_start());
    this -> print_output();

}

void Parser_Engine::fill_tokens()
{
    ifstream inputFile("output.txt");
    string input, s;
    bool flag = false;
    while(getline(inputFile, s))
    {
        this->tokens.push_back(s);
    }
}

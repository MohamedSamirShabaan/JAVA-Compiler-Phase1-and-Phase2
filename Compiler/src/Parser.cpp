#include "Parser.h"
#include <bits/stdc++.h>

using namespace std;

Parser::Parser() : className({"" , "" ,""})
{
    keywords = nullptr;
    punctuation = nullptr;
    finalNFA = nullptr;
}

Parser::~Parser()
{
    //dtor
}

NFA* Parser::run (ifstream& grammer)
{
    string input;

    while(getline(grammer , input))
    {
        if(input[0] == '{')                  //keyword
        {
            parseKeyword (input);
        }

        else if(input[0] == '[')             //punctuation
        {
            parsePunctuation(input);
        }

        else
        {
            for(int i = 0;i < (int)input.size();++i)
            {
                if(input[i] == ':')          //regular expression
                {
                    parseExpression(input, i);
                    break;
                }

                else if(input[i] == '=')     //regular Definition
                {
                    parseDefinition (input, i);
                    break;
                }
            }
        }
    }
    if(keywords != nullptr)
    {
        keywords -> endNode -> set_is_accepted(true);
        keywords -> endNode -> set_name(className[1]);
        keywords -> endNode -> set_priority(1);

        if(finalNFA == nullptr)
            finalNFA = keywords;
        else
            finalNFA -> combine(keywords);
    }

    if(punctuation != nullptr)
    {
        punctuation -> endNode -> set_is_accepted(true);
        punctuation -> endNode -> set_name(className[2]);
        punctuation -> endNode -> set_priority(2);

        if(finalNFA == nullptr)
            finalNFA = punctuation;
        else
            finalNFA -> combine(punctuation);
    }

    return finalNFA;
}

void Parser::parseKeyword (string& input)
{
    NFA *currentKeyword = nullptr;

    for(int i = 1;i < (int)input.size();++i)
    {
        if(input[i] == ' ' or input[i] == '}') continue;

        string token;

        while(input[i] != ' ' and input[i] != '}') token += input[i++];

        currentKeyword = new NFA();
        currentKeyword -> startingNode = new State(false);
        currentKeyword -> endNode = new State(false);
        currentKeyword -> startingNode -> add_to_table(token[0] , currentKeyword -> endNode);

        for(int i = 1;i < (int)token.size();++i)
            currentKeyword -> extend(token[i]);

        className[1] = "Keyword";

        if(keywords == nullptr)     keywords = currentKeyword;
        else                        keywords -> OR(currentKeyword);
    }
}

void Parser::parsePunctuation (string& input)
{
    NFA *currentPunctuation = nullptr;

    for(int i = 1;i < (int)input.size();++i)
    {
        if(input[i] == ' ' or input[i] == ']')     continue;

        std::string token;
        if(input[i] == '\\')
            token += input[++i];

        else
            token +=input[i];

        assert(token.size() == 1);


        currentPunctuation = new NFA();                        ///here
        currentPunctuation -> startingNode = new State(false);
        currentPunctuation -> endNode = new State(false);
        currentPunctuation -> startingNode -> add_to_table(token[0] , currentPunctuation -> endNode);

        currentPunctuation -> endNode -> set_name(className[2]);
        currentPunctuation -> endNode -> set_priority(2);

        className[2] = "Punctuation";

        if(punctuation == nullptr)      punctuation = currentPunctuation;
        else                            punctuation -> OR(currentPunctuation);
    }
}

void Parser::parseExpression (string& input, int& i)
{
    string expression = input.substr(0 , i);
    className.push_back(expression);

    int k = ++i;

    string finaExpression;
    for(;i < (int)input.size();++i)
    {
        if(input[i] == ' ')
        {
            if(i - 1 >= k)
            {
                string x = input.substr(k , i - k);
                if(identifiers.find(x) != identifiers.end())
                    finaExpression += "(" + identifiers[x] + ")";
                else
                    finaExpression += x;
            }

            k = i + 1;
        }

        else if(input[i] == '(' or input[i] == '*'  or input[i] == '+'  or input[i] == ')' or input[i] == '-' or input[i] == '|')
        {
            if(i and input[i - 1] == '\\')  continue;

            if(i - 1 >= k)
            {
                string x = input.substr(k , i - k);

                if(identifiers.find(x) != identifiers.end())
                    finaExpression += "(" + identifiers[x] + ")";
                else
                    finaExpression += x;
            }

            finaExpression += input[i];
            k = i + 1;
        }
    }

    if(i > k)
    {
        string x = input.substr(k , i - k);

        if(identifiers.find(x) != identifiers.end())
            finaExpression += "(" + identifiers[x] + ")";
        else
            finaExpression += x;
    }

    vector < std::string > tokens = getTokens(finaExpression);

    vector < string > expressionPost = generatePostfix(tokens);

    NFA* NFAExpression = buildNFA(expressionPost);

    NFAExpression -> endNode -> set_is_accepted(true);
    NFAExpression -> endNode -> set_name(expression);
    NFAExpression -> endNode -> set_priority(className.size() - 1);

    if(finalNFA == nullptr)     finalNFA = NFAExpression;
    else                        finalNFA -> combine(NFAExpression);
}

vector< string > Parser::getTokens(string& input)
{
    vector < string > result;

    int k = 0;
    int i = 0;
    for(;i < (int)input.size();++i)
    {
        if(input[i] == '(' or input[i] == '*'  or input[i] == '+'  or input[i] == ')' or input[i] == '-' or input[i] == '|')
        {
            if(i and input[i - 1] == '\\')  continue;
            if(i - 1 >= k)
            {
                string x = input.substr(k , i - k);
                if(identifiers.find(x) != identifiers.end())
                    result.push_back("(" + identifiers[x] + ")");
                else
                    result.push_back(x);
            }

            string z;
            z += input[i];
            result.push_back(z);
            k = i + 1;
        }
    }
    if(i > k)
    {
        string x = input.substr(k , i - k);
        if(identifiers.find(x) != identifiers.end())
            result.push_back("(" + identifiers[x] + ")");
        else
            result.push_back(x);
    }

    return result;
}

vector< string > Parser::generatePostfix (vector < string >& tokens)
{
    vector < string > result;
    string operations = "+-*|()";
    stack < string > s;
    for(int i = 0;i < (int)tokens.size();++i)
    {
        if(i and tokens[i - 1] != "|" and tokens[i - 1] != "(" and tokens[i - 1] != "-" and (tokens[i] == "(" or operations.find(tokens[i]) == std::string::npos))
        {
            while(!s.empty())
            {
                if(s.top() == "*" or s.top() == "+" or s.top() == "-")
                {
                    result.push_back(s.top());
                    s.pop();
                }
                else        break;
            }
            s.push(" ");
        }
        if(tokens[i] == "(" or tokens[i] == "-")
            s.push(tokens[i]);

        else if(tokens[i] == ")")
        {
            while(!s.empty())
            {
                if(s.top() == "(")
                {
                    s.pop();
                    break;
                }
                result.push_back(s.top());
                s.pop();
            }
        }
        else if(tokens[i] == "*")
        {
            while(!s.empty())
            {
                if(s.top() == "-")
                {
                    result.push_back(s.top());
                    s.pop();
                }
                else        break;
            }
            s.push(tokens[i]);
        }
        else if(tokens[i] == "+")
        {
            while(!s.empty())
            {
                if(s.top() == "*" or s.top() == "-")
                {
                    result.push_back(s.top());
                    s.pop();
                }
                else        break;
            }
            s.push(tokens[i]);
        }
        else if (tokens[i] == "|")
        {
            while(!s.empty())
            {
                if(s.top() == "*" or s.top() == "-" or s.top() == "+" or s.top() == " ")
                {
                    result.push_back(s.top());
                    s.pop();
                }
                else        break;
            }
            s.push(tokens[i]);
        }
        else
            result.push_back(tokens[i]);
    }
    while(!s.empty())
    {
        result.push_back(s.top());
        s.pop();
    }

    return result;
}

void Parser::parseDefinition (string& input, int& i)
{
    string finalIdentifier;
    int z = i;

    while(input[z - 1] == ' ')  --z;

    string id = input.substr(0 , z);
    int k = ++i;

    for(;i < (int)input.size();++i)
    {
        if(input[i] == ' ')
        {
            if(i - 1 >= k)
            {
                string x = input.substr(k , i - k);
                if(identifiers.find(x) != identifiers.end())
                    finalIdentifier += "(" + identifiers[x] + ")";
                else
                    finalIdentifier += x;
            }

            k = i + 1;
        }
        else if(input[i] == '(' or input[i] == '*'  or input[i] == '+'  or input[i] == ')' or input[i] == '-' or input[i] == '|')
        {
            if(i and input[i - 1] == '\\')  continue;

            if(i - 1 >= k)
            {
                string x = input.substr(k , i - k);
                if(identifiers.find(x) != identifiers.end())
                    finalIdentifier += "(" + identifiers[x] + ")";

                else
                    finalIdentifier += x;
            }

            finalIdentifier += input[i];
            k = i + 1;
        }
    }

    if(i > k)
        finalIdentifier += input.substr(k , i - k);

    identifiers[id] = finalIdentifier;
}

NFA* Parser::buildNFA(vector < string >& post)
{
    stack < NFA* > s;

    for(int i = 0;i < (int)post.size();++i)
    {
        if(post[i] == " ")
        {
            assert(s.size() >= 2 and "error");
            NFA *A = s.top();    s.pop();
            NFA *B = s.top();    s.pop();
            B -> concatenate(A);
            s.push(B);
        }
        else if(post[i] == "*")
        {
            NFA *A = s.top();    s.pop();
            A -> zeroOrMore();
            s.push(A);
        }
        else if(post[i] == "+")
        {
            NFA *A = s.top();    s.pop();
            A -> oneOrMore();
            s.push(A);
        }
        else if(post[i] == "|")
        {
            assert(s.size() >= 2 and "error");
            NFA *A = s.top();    s.pop();
            NFA *B = s.top();    s.pop();
            B -> OR(A);
            s.push(B);
        }
        else if(post[i] == "\\L")
        {
            NFA *str = new NFA();
            str -> startingNode = new State(false);
            str -> endNode = new State(false);

            str -> startingNode -> add_to_table('\0' , str -> endNode);
            s.push(str);
        }
        else if((i + 2) < (int)post.size() and post[i].size() * post[i + 1].size() * post[i + 2].size() == 1 and
            post[i + 2] == "-" and
            ((isalpha(post[i][0]) and isalpha(post[i + 1][0])) or
            (isdigit(post[i][0]) and isdigit(post[i + 1][0]))))
        {
            char a = post[i++][0];
            char b = post[i++][0];

            assert(post[i] == "-" and "error invalid postfix");

            NFA *range = new NFA();
            range -> startingNode = new State(false);
            range -> endNode = new State(false);

            for(int j = a;j <= b;++j)
                range -> startingNode -> add_to_table(j , range -> endNode);

            range -> endNode -> set_is_accepted(true);
            s.push(range);
        }
        else
        {
            NFA *nfa = new NFA();
            nfa -> startingNode = new State(false);
            nfa -> endNode = new State(false);

            int z = 0;
            if(post[i][z] == '\\')      ++z;

            nfa -> startingNode -> add_to_table(post[i][z++] , nfa -> endNode);

            while(z < (int)post[i].size())
            {
                if(post[i][z] == '\\')
                    ++z;

                nfa -> extend(post[i][z++]);
            }
            s.push(nfa);
        }
    }

    NFA *result = s.top();    s.pop();

    return result;
}

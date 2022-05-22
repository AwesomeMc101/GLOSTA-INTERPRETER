
#include <iostream>
#include <Windows.h>
#include <string>
#include <map>
#include <functional>
#include <stack>
#include <vector>
#include <sstream>
#include <exception>
#include <excpt.h>
#include <istream>
#include <ostream>
//#include <strstream>
#include <fstream>

#include <stdio.h>
#include <string.h>

#include "glostaState.h"

namespace Execution
{
    //function shit

    void print(std::string arg)
    {
        std::cout << arg << std::endl;
    }

    void strlen(std::vector<std::string>& stack, std::string arg)
    {
        stack.push_back(std::to_string(arg.length()));
    }

    void add(std::vector<std::string>& stack, std::string arg)
    {
        //STACK ITEM: 0 | 3,1)
        //) removed

        std::vector<double> keys;

        char* token;
        char* rest = (char*)arg.c_str();
        //strcpy(rest, _code.c_str());
        int i = 2;
        while ((token = strtok_s(rest, ",", &rest)))
        {
            keys.push_back(std::stod(token));
        }

        double finalResult = 0.0;
        for (double z : keys)
        {
            finalResult += z;
        }
        stack.push_back(std::to_string(finalResult));

    }
    void defvar(std::string args, std::vector<std::string>& variableNameStack, std::vector<std::string>& variableNameValue)
    {
        std::vector<std::string> keys;
        char* token;
        char* rest = (char*)args.c_str();
        //strcpy(rest, _code.c_str());
        int i = 2;
        while ((token = strtok_s(rest, ",", &rest)))
        {
            keys.push_back((token));
        }

        //std::cout << "Vars set;";
        variableNameStack.push_back(keys[0]);
        variableNameValue.push_back(keys[1]);
    }

    void callFunction(std::string function, std::vector<std::string> &stack, std::vector<std::string> &variableNameStack, std::vector<std::string>& variableNameValue, std::string arg)
    {
        //this obviously needs to be rewritten but i put this here to test it lol
        arg.erase(std::remove(arg.begin(), arg.end(), ')'), arg.end());
        if (function == "glosta::print")
        {
          //  std::cout << "PRINT (ARG: " << arg << ")\n";
            print(arg);
        }
        if (function == "glosta::strlen")
        {
            //std::cout << "STEJODK";
            strlen(stack, arg);
        }
        if (function == "glosta::add")
        {
            add(stack, arg);
        }
        if (function == "glosta::defvar")
        {
            defvar(arg, variableNameStack, variableNameValue);
        }
    }
}

namespace Tokenizer
{
    void tokenizeLine(std::vector<std::string>& vec, std::string _code)
    {
        //char str[] = _code;
        
        char* token;
        char* rest = (char*)_code.c_str();
        //strcpy(rest, _code.c_str());
        
        while ((token = strtok_s(rest, ";;", &rest)))
            vec.push_back(token);
    }

    void tokenizeCode(glostaItem* gs, std::vector<std::string>& vec, std::vector<std::string>& vec1, std::string _code)
    {
        //char str[] = _code;

        char* token;
        char* rest = (char*)_code.c_str();
        //strcpy(rest, _code.c_str());
        int i = 2;
        while ((token = strtok_s(rest, "(", &rest))) 
        {
            if (i == 2) {
                vec.push_back(token);
            }
            else
            {
                vec1.push_back(token);
            }
            i++;
        }
    }
}

namespace Variables
{
    void variableCallHandler(std::vector<std::string>& stack, std::vector<std::string>& variableNameStack, std::vector<std::string>& variableNameValue, std::string arg)
    {
        arg.erase(std::remove(arg.begin(), arg.end(), ')'), arg.end());

        size_t pos = 0;
        for (int i = 0; i < variableNameStack.size(); i++)
        {
            if (variableNameStack[i] == arg)
            {
                pos = i;
                break;
            }
        }

        stack.push_back(variableNameValue[pos]);

    }
}

int interpret(glostaItem* gs, std::string _code)
{
    std::vector<std::string> codeBlocks;
    Tokenizer::tokenizeLine(codeBlocks, _code);

    std::vector<std::string> variableNameStack;
    std::vector<std::string> variableValueStack;

    for (int i = 0; i < codeBlocks.size(); i++)
    {
        std::string code = codeBlocks[i];
       // std::cout << "CODEBLOCK: " << code << std::endl;

        std::vector<std::string> functions;
        std::vector<std::string> args;
        Tokenizer::tokenizeCode(gs, functions, args, code);
        for (std::string x : functions)
        {
           // std::cout << "FUNCTION: " << x << std::endl;
            for (std::string y : args)
            {
              //  std::cout << "ARG: " << y << std::endl;
            }
        }

       /* for (int i = args.size() - 1; i >= 0; i--)
        {
            std::cout << vec_of_num[i] << ", ";
        }*/

        std::vector<std::string> stack;
        for (int z = args.size() - 1; z >= 0; z--)
        {
           // std::cout << "idx: " << z << " | arg: " << args[z] << std::endl;
            //"hello" [0] -> strlen [1]
            if (isGFunction(gs, args[z]))
            {
               // printf("FUNTIONNNNNNNNNNN");
                if (z == args.size()) { continue; } //prevent vector range error
                Execution::callFunction(args[z], stack, variableNameStack, variableValueStack, args[z + 1]);
            }
            else if (isVarName(variableNameStack, args[z]))
            {
                //std::cout << "VAR NAME DETECE";
                Variables::variableCallHandler(stack, variableNameStack, variableValueStack, args[z]);
            }
            else
            {
                //  printf("else");
                stack.push_back(args[z]);
                // printf("done");
            }
        }
        //for (int z = 0; z < args.size(); z++)
        //{
        //    std::cout << "idx: " << z << " | arg: " << args[z] << std::endl;
        //    //"hello" [0] -> strlen [1]
        //    if (isGFunction(gs, args[z]))
        //    {
        //        if (z == 0) { continue; } //prevent vector range error
        //        Execution::callFunction(args[z], stack, args[z + 1]);
        //    }
        //    else
        //    {
        //      //  printf("else");
        //        stack.push_back(args[z]);
        //       // printf("done");
        //    }
        //}

       // printf("execution");

        for (int u = 0; u < stack.size(); u++)
        {
            //std::cout << "STACK ITEM: " << u << " | " << stack[u] << std::endl;
        }

        Execution::callFunction(functions[0], stack, variableNameStack, variableValueStack, stack[stack.size()-1]);

    }



    return 1;
}

int main()
{
    /*
    read line by line
    
   glosta::print('kys')
   glosta::defvar(varname, varvalue)
   glosta::setvar(varname, newvarvalue)
    glosta::add(3,1)
    */
    /*TODO:
    - function def
    - variable in variable
    - variable in add() and add() in defvar
    
    L8R DATE TODO:
    - forlooping/whilelooping
    - graphics library
    - exe compilation
    - FINISH GLOSTABYE

    Made By AwesomeMc101 for Lunar Guard Technologies
    5.22.2022

    */
    glostaItem* gs = new glostaItem;
    pushGFunctions(gs);
    setTypeClass(gs);
    interpret(gs, "glosta::print(glosta::strlen(youre gay));;glosta::print(i luv u bbg);;glosta::print(glosta::add(3,1,5,6));;");

    interpret(gs, "glosta::defvar(hop,hello);;glosta::print(hop);;");

    while (1)
    {
        std::string ex = "";
        std::cout << std::endl;
        std::getline(std::cin, ex);
        interpret(gs, ex);
        ex.clear();
    }
}


#include "MyShell.h"

vector<string> parseArg(string arg){
    vector<string> args;
    size_t start = 0; size_t end = 0;

    while((end = arg.find_first_of(delim, start)) != string::npos){
        if(end != start) args.push_back(arg.substr(start, end-start));
        start = end+1;
    }

    if(start < arg.length()) args.push_back(arg.substr(start));
    
    return args;
}  
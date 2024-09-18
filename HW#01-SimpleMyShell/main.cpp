#include "MyShell.h"

bool IsEXIT;

time_t SHELL_TIME;
string SHELL_NAME;
string SHELL_USER;

string delim;
vector<string> internal_cmd;

int main(int argc, char * argv[]){
    SHELL_TIME = time(NULL);
    SHELL_NAME = "MyShell";
    SHELL_USER = getenv("USER");

    delim = " \n\t\r\a";
    internal_cmd = {"cd", "exit", "pwd"};

    ConfigEnv();
    InitShell();

    cout<<"Finish SiSH Shell !!!"<<endl;    
    return 0;
}
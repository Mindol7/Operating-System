#include "MyShell.h"

bool isEXIT;

time_t SHELL_TIME;
string SHELL_NAME;
string SHELL_USER;

string delim;
vector<string> internalCommand;

int main(int argc, char * argv[]){
    SHELL_TIME = time(NULL);
    SHELL_NAME = "minhyuk:";
    SHELL_USER = getenv("USER");

    delim = " \n\t\r\a";
    internalCommand = {"cd", "exit", "quit", "q", "pwd", "help"};

    configEnv();
    initShell();

    cout<<"Finish SiSH Shell !!!"<<endl;    
    return 0;
}
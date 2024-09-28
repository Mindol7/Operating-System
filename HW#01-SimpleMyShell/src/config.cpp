#include "MyShell.h"
using namespace std;

void configEnv() {
    cout<<"\033[34m\033[1m";
    cout<<"*****************************************"<<endl;
    cout<<"******  Welcome to MinHyuk Shell - SiSH  ******"<<endl;
    cout<<"*****************************************"<<endl;
    cout<<"\033[0m";  

    cout<<"\033[35m\033[1m";
    cout<<"Current Time: "<<ctime(&SHELL_TIME);
    cout<<"User: "<<SHELL_USER<<endl;
    cout<<"\033[0m";

    cout<<"\033[36m\033[1m";
    cout<<"---------------------------------------"<<endl;
    cout<<"Type 'help' to get started!"<<endl;
    cout<<"---------------------------------------"<<endl;
    cout<<"\033[0m";
    cout<<endl;
}

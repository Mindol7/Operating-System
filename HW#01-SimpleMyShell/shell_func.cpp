#include "MyShell.h"

void InitShell(){
    string cmd;
    vector<string> tok_cmd;

    while(!IsEXIT){
        cout<<SHELL_NAME<<"$ ";
        getline(cin, cmd);
        tok_cmd = ParseArg(cmd);
        SelectShell(tok_cmd);
    }
}

void SelectShell(vector<string> args){
    if(args.empty()) perror("Empty Command");

    if(args.size() >= 3) RedirectCmd(args);
    else{
        for(int idx = 0; idx < internal_cmd.size(); ++idx){
            if(args[0] == internal_cmd[idx]) {
                InternalCmd(args);
                return;
            }
        }
        ExternalCmd(args);
    }
    return;
}

void InternalCmd(vector<string> args){
    if(args[0] == "cd"){
        if(args.size() == 1) {
            perror("No Argument, So ChangeDir to $HOME\n");
            const char * home_dir = getenv("HOME");
            if(home_dir != NULL){
                if (chdir(home_dir) != 0) {
                    perror("Failed to change directory");
                }
            }
            else{
                perror("There is no set Home Dir");
            }
        }
        else if(!args[1].empty()){
            if(chdir(args[1].c_str()) != 0) perror("No such file or directory\n");
        }
    }
    else if(args[0] == "exit" || args[0] == "quit" || args[0] == "q"){
        IsEXIT = true;
        return;
    }
    else if(args[0] == "pwd"){
        cout<<"Current Working Directory: "<<filesystem::current_path()<<endl;
        return;
    }
}

void ExternalCmd(vector<string> args){
    pid_t pid;

    char* argv[args.size() + 1];

    for (int i = 0; i < args.size(); ++i) {
        argv[i] = const_cast<char*>(args[i].c_str());
    }
    argv[args.size()] = NULL;

    pid = fork();

    if(pid < 0) perror("Fork Error\n");
    else if(pid == 0){
        if(execvp(args[0].c_str(), argv) == -1) perror("execvp failed");
        exit(EXIT_FAILURE);
    }
    else{
        int status;
        pid_t wait_pid = waitpid(pid, &status, 0);

        if(wait_pid == -1) perror("Wait PID Failed");
        
    }
}

void RedirectCmd(vector<string> args){
    for(int i = 0; i < args.size(); ++i){
        if(args[i] == ">"){

        }
        else if(args[i] == "<"){

        }
        else if(args[i] == "|"){

        }
    }
}
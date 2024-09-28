#include "MyShell.h"

void initShell(){
    string cmd;
    vector<string> tokCmd;

    while(!isEXIT){
        cout<<"\033[32m\033[1m"<<SHELL_NAME<<"\033[0m"<<"\033[34m\033[1m"<<getcwd(NULL, 0)<<"\033[0m"<<"$ ";
        
        getline(cin, cmd);
        tokCmd = parseArg(cmd);
        selectShell(tokCmd);
    }
}

void selectShell(vector<string> args){
    if(args.empty()) perror("Empty Command");

    if(args.size() >= 3) redirectCmd(args);
    else{
        for(int idx = 0; idx < internalCommand.size(); ++idx){
            if(args[0] == internalCommand[idx]) {
                internalCmd(args);
                return;
            }
        }
        externalCmd(args);
    }
    return;
}

void internalCmd(vector<string> args){
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
        isEXIT = true;
        return;
    }
    else if(args[0] == "pwd"){
        cout<<"Current Working Directory: "<<filesystem::current_path()<<endl;
        return;
    }
    else if(args[0] == "help"){
        cout<<"==========================================================================="<<endl;
        cout<<"This shell provides external and internal commands, as well as redirection. " 
        <<endl<<"Although it's a simple shell and does not offer advanced features, "
        <<endl<<"it supports essential commands. Please use the shell with commands such as \"ls -al\", \"cd\", \"cat *.c | grep z\" !" 
        <<endl<<"Thanks!!!"<<endl;
        cout<<"==========================================================================="<<endl<<endl;
    }
}

void externalCmd(vector<string> args){
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

void redirectCmd(vector<string> args){
    int fd;
    int saved_fd;
    string redirectChar;

    for(int i = 0; i < args.size(); ++i){
        if(args[i] == ">"){
            fd = open(args[i + 1].c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if(fd < 0){
                perror("open");
                return;
            }

            redirectChar = ">";
            saved_fd = dup(STDOUT_FILENO);
            dup2(fd, STDOUT_FILENO);
            close(fd);

            args.erase(args.begin() + i, args.begin() + i + 2);
            break;
        }
        else if(args[i] == "<"){
            fd = open(args[i + 1].c_str(), O_RDONLY);
            if(fd < 0){
                perror("open");
                return;
            }

            redirectChar = "<";
            saved_fd = dup(STDIN_FILENO);
            dup2(fd, STDIN_FILENO); // ls | test.txt면 뒤에꺼를 열어서 입력을 받음
            close(fd);

            args.erase(args.begin() + i, args.begin() + i + 2); // 명령어 목록에서 제거
            break;
        }
        else if(args[i] == "|"){
            int pipefd[2];
            if(pipe(pipefd) == -1){
                perror("Pipe Failed");
                return;
            }

            pid_t pid = fork();

            if(pid == -1){
                perror("Fork Failed");
                return;
            }

            if(pid == 0){
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]); close(pipefd[1]);

                vector<string> leftCmd(args.begin(), args.begin() + i);
                char * argv[leftCmd.size() + 1];

                for(int i = 0; i < leftCmd.size(); ++i){
                    argv[i] = const_cast<char*>(args[i].c_str());
                }
                argv[leftCmd.size()] = NULL;
                execvp(argv[0], argv);
        
                perror("execvp Failed");
                exit(EXIT_FAILURE);
            }
            else{
                wait(NULL);

                dup2(pipefd[0], STDIN_FILENO);
                close(pipefd[0]); close(pipefd[1]);

                vector<string> rightCmd(args.begin() + i + 1, args.end());
                char* argv[rightCmd.size() + 1];

                for(int i = 0; i < rightCmd.size(); ++i){
                    argv[i] = const_cast<char*>(rightCmd[i].c_str());
                }
                argv[rightCmd.size()] = NULL;
                execvp(argv[0], argv);

                perror("execvp failed");
                exit(EXIT_FAILURE);
            }
        }
    }

    pid_t pid;
    char* argv[args.size() + 1];

    for(int i = 0; i < args.size(); ++i){
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

        if(redirectChar == ">") dup2(saved_fd, STDOUT_FILENO);
        else if(redirectChar == "<") dup2(saved_fd, STDIN_FILENO);
        
        close(saved_fd);
    }
}
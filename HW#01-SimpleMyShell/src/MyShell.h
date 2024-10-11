#pragma once

#include "bits/stdc++.h"

#include <filesystem>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

/* Global Variables */
extern bool isEXIT;
extern string SHELL_NAME;
extern time_t SHELL_TIME;
extern string SHELL_USER;
extern string delim;
extern vector<string> internalCommand;

/* Functions */
void configEnv();
void initShell();
void selectShell(vector<string> args);
void internalCmd(vector<string> args);
void externalCmd(vector<string> args);
void redirectCmd(vector<string> args);
vector<string> parseArg(string arg);
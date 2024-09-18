#pragma once

#include "bits/stdc++.h"

#include <filesystem>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;


/* 필요한 전역변수 선언 */
extern bool IsEXIT;
extern string SHELL_NAME;
extern time_t SHELL_TIME;
extern string SHELL_USER;
extern string delim;
extern vector<string> internal_cmd;

/* 필요한 함수 선언 */
void ConfigEnv();
void InitShell();
void SelectShell(vector<string> args);
void InternalCmd(vector<string> args);
void ExternalCmd(vector<string> args);
void RedirectCmd(vector<string> args);
vector<string> ParseArg(string arg);
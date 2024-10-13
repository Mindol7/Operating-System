/*
* Author: Minhyuk Cho
* Date: 2024-10-12
* Description: Semaphore Ver. minhyuk_char_stat.cpp
*/

#include "word_count.h"

// void process_line(SharedObject& so, const string& line) {
//     char sep[] = "{}()[],;\" \n\t^";
//     char* cstr = new char[line.length() + 1];
//     strcpy(cstr, line.c_str());

//     char* token = strtok(cstr, sep);
//     while (token != nullptr) {
//         size_t length = strlen(token);

//         if (length >= MAX_STRING_LENGTH) length = MAX_STRING_LENGTH;

//         so.stat[length - 1]++;

//         for (size_t i = 0; i < length; i++) {
//             unsigned char ch = token[i];
//             if (ch < ASCII_SIZE) {
//                 so.stat2[ch]++;
//             }
//         }

//         token = strtok(nullptr, sep);
//     }

//     delete[] cstr;
// }
void process_line(SharedObject& so, const std::string& line) {
    std::istringstream iss(line);  // 문자열을 한 번에 처리할 수 있도록 stringstream 사용
    std::string token;
    
    // 구분자를 사용하지 않고 직접 공백과 특수문자를 기준으로 토큰화
    while (iss >> token) {
        size_t length = token.length();

        // 토큰의 길이 제한 처리
        if (length > MAX_STRING_LENGTH) {
            length = MAX_STRING_LENGTH;
        }

        // 단어 길이 통계
        so.stat[length - 1]++;

        // 각 문자에 대해 아스키 코드별 통계
        for (size_t i = 0; i < length; i++) {
            unsigned char ch = token[i];
            if (ch < ASCII_SIZE) {
                so.stat2[ch]++;
            }
        }
    }
}


void print_statistics(SharedObject& so){
	int sum = 0;
	for (int i = 0 ; i < MAX_STRING_LENGTH ; i++) {
		sum += so.stat[i];
	}

    if (sum == 0) {
        printf("No words were processed.\n");
        return;
    }

	printf("*** print out distributions *** \n");
	printf("  #ch  freq \n");

	for (int i = 0 ; i < 30 ; i++) {
		int j = 0;
		int num_star = so.stat[i]*80/sum;
		printf("[%3d]: %4d \t", i+1, so.stat[i]);
		for (j = 0 ; j < num_star ; j++)
			printf("*");
		printf("\n");
	}

	printf("       A        B        C        D        E        F        G        H        I        J        K        L        M        N        O        P        Q        R        S        T        U        V        W        X        Y        Z\n");
	printf("%8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d %8d\n",
			so.stat2['A']+so.stat2['a'], so.stat2['B']+so.stat2['b'],  so.stat2['C']+so.stat2['c'],  so.stat2['D']+so.stat2['d'],  so.stat2['E']+so.stat2['e'],
			so.stat2['F']+so.stat2['f'], so.stat2['G']+so.stat2['g'],  so.stat2['H']+so.stat2['h'],  so.stat2['I']+so.stat2['i'],  so.stat2['J']+so.stat2['j'],
			so.stat2['K']+so.stat2['k'], so.stat2['L']+so.stat2['l'],  so.stat2['M']+so.stat2['m'],  so.stat2['N']+so.stat2['n'],  so.stat2['O']+so.stat2['o'],
			so.stat2['P']+so.stat2['p'], so.stat2['Q']+so.stat2['q'],  so.stat2['R']+so.stat2['r'],  so.stat2['S']+so.stat2['s'],  so.stat2['T']+so.stat2['t'],
			so.stat2['U']+so.stat2['u'], so.stat2['V']+so.stat2['v'],  so.stat2['W']+so.stat2['w'],  so.stat2['X']+so.stat2['x'],  so.stat2['Y']+so.stat2['y'],
			so.stat2['Z']+so.stat2['z']);
}
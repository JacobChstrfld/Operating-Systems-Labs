#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <cctype>
#include <vector>

//Jacob Chesterfield
//Lab 1 - CS 470

void manual() {
    printf("\nList of available commands: ");

    printf("\n\nmanual");
    printf("\n    displays a list of commands available in this shell");

    printf("\n\nsysteminformation -switch");
    printf("\n This command outputs information about the current system");
    printf("\n  available switches: -p, -m, -o");

    printf("\n\nmemoryinformation -switch");
    printf("\n This command outputs information about the current system memory");
    printf("\n  availabe switches: -s, -u, -S \n");
}

bool isNum(const char *str) {
    for (int i = 0; i < (strlen(str)); i++) {
        if (i == 0 && str[i] == '-') {
            continue;
        } else if (isdigit(str[i]) == 0) {
            return false;
        }
    }
}

void sysInfo(char *str) {
    char *t;
    t = strtok(str, " ");
    std::vector<char> switches;
    t = strtok(NULL, " ");
    if (t == NULL) {
        printf("Invalid use of memoryinformation\n");
        printf("Try \'memoryinformation --help\' for more information. \n");
    }

    while (t != NULL) {

        for (int i = 0; i < (strlen(t)); i++) {
            if (i == 0 && t[i] == '-') {
                continue;
            } else if (i == 0 && t[i] != '-') {
                printf("Invalid use of systeminformation\n");
                printf("Try \'systeminformation --help\' for more information.\n");
                break;
            } else if (t[i] != 'p' && t[i] != 'm' && t[i] != 'o') {
                std::string s = "systemonformation: invalid option -- '";
                s.push_back(t[i]);
                s.push_back('\'');
                std::cout << s << std::endl;
                printf("Try \'systeminformation --help\' for more information.");
                return;
            } else {
                switches.push_back(t[i]);

            }
        }
        t = strtok(NULL, " ");
    }

    for (int i = 0; i < switches.size(); i++) {
        if (switches[i] == 'p') {
            write(1, "Number of available processors: ", 32);
            system("nproc");
            printf("\n");
        }
        if (switches[i] == 'm') {
            write(1, "Maximum number of open files per process: ", 42);
            system("ulimit -n");
            printf("\n");
        }
        if (switches[i] == 'o') {
            write(1, "CPU type: ", 10);
            system("cat /proc/cpuinfo | grep 'model name' | uniq");
            printf("\n");
        }
    }


}

void memInfo(char *str) {
    char *t;
    t = strtok(str, " ");
    std::vector<char> switches;
    t = strtok(NULL, " ");
    if (t == NULL) {
        printf("Invalid use of memoryinformation\n");
        printf("Try \'memoryinformation --help\' for more information. \n");
    }

    while (t != NULL) {

        for (int i = 0; i < (strlen(t)); i++) {
            if (i == 0 && t[i] == '-') {
                continue;
            } else if (i == 0 && t[i] != '-') {
                printf("Invalid use of memoryinformation\n");
                printf("Try \'memoryinformation --help\' for more information.\n");
                break;
            } else if (t[i] != 's' && t[i] != 'u' && t[i] != 'S') {
                std::string s = "memoryonformation: invalid option -- '";
                s.push_back(t[i]);
                s.push_back('\'');
                std::cout << s << std::endl;
                printf("Try \'memoryinformation --help\' for more information.");
                return;
            } else {
                switches.push_back(t[i]);

            }
        }
        t = strtok(NULL, " ");

    }

    for (int i = 0; i < switches.size(); i++) {
        if (switches[i] == 's') {
            write(1, "Total swap space in bytes: ", 27);
            system("free -b |awk '/^Swap:/{print $2}'");
            printf("\n");
        }
        if (switches[i] == 'u') {
            write(1, "Total memory in bytes: ", 23);
            system("free -b |awk '/^Mem:/{print $2}'");
            printf("\n");
        }
        if (switches[i] == 'S') {
            write(1, "Total shared memory in bytes: ", 30);
            system("free -b |awk '/^Mem:/{print $5}'");
            printf("\n");
        }
    }


}

int main(int arg1, char **arg2) {
    using namespace std;
    char input[90];
    string inputString;

    while (1) {
        printf("\ncwushell>");
        cin.getline(input, 90);
        inputString = input;

        if (input == "exit") {
            return 0;
        } else if (inputString.find("exit") == 0) {
            char *t;
            t = strtok(input, " ");
            t = strtok(NULL, " ");
            if (t == NULL) return 0;
            if (isNum(t)) {
                int exit = atoi(t);
                return exit;
            }
            break;
        } else if (inputString.find("manual") == 0) {
            char *t;
            t = strtok(input, " ");
            t = strtok(NULL, " ");
            if (t == NULL) manual();
            else {
                printf("Invalid use of manual: type 'manual' to see list of commands");
            }
        } else if (inputString == "systeminformation" || inputString == "systeminformation --help"
                   || inputString == "systeminformation --h") {
            printf("Usage: systeminformation [OPTION]\n");
            printf("Outputs various information about system\n");
            printf("Available switches: \n");
            printf("-p: will print the number of processors available in the system \n");
            printf("-m: will print the maximum number of open files per process \n");
            printf("-o: will print the processor type \n");
        } else if (inputString.find("systeminformation") == 0) {
            sysInfo(input);
        } else if (inputString == "memoryinformation" || inputString == "memoryinformation --help"
                   || inputString == "memoryinformation --h") {
            printf("Usage: memoryinformation [OPTION]\n");
            printf("Outputs various information about system memory\n");
            printf("Available switches: \n");
            printf("-s: will print the total swap space in bytes \n");
            printf("-u: will print the total memory in bytes \n");
            printf("-S: will print the total shared memory in bytes\n");
        } else if (inputString.find("memoryinformation") == 0) {
            memInfo(input);
        } else {
            system(input);
            printf("\n\n");

        }
    }
    return 0;
}

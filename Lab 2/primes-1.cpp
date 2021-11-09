#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>	/* needed to define exit() */
#include <unistd.h>	/* needed for fork() and getpid() */
#include <errno.h>	/* needed for errno */
#include <stdio.h>
#include <sys/wait.h>

//Jacob Chesterfield Fork Lab

/*This function's code was sourced from https://www.cs.rutgers.edu/~pxk/416/notes/c-tutorials/wait.html
 * It's role is to have the parent process wait until the child has exited
 * */
void parentProcess(int pid){
    int got_pid, status;
    while (got_pid = wait(&status)) {
        if (got_pid == pid)
            break;
        if ((got_pid == -1) && (errno != EINTR)) {
            /* an error other than an interrupted system call */
            perror("waitpid");
            return;
        }
    }

}

int main(int argc, char* argv[]) {
    int a;
    std::vector<int> arr;
    std::string inputFile = argv[1];
    std::ifstream infile(inputFile);
    pid_t pid, wpid;
    int status = 0;
    while(infile >> a){
        arr.push_back(a);
    }
    infile.close();
    int n = arr.size();
    std::string s;


    for (int i = 2; i < n / 2; i++){
            pid = fork();
            if (pid > 0) {
                parentProcess(pid);
            }
            else if (pid < 0){
                printf("error with pid");
            }
            else{
                s = "awk '($1 % " + std::to_string(i) + " != 0) || ($1 == " + std::to_string(i)+")' "+ inputFile + " > n.txt";
                system(s.data());
                s = "rm " + inputFile;
                system(s.data());
                s = "cat n.txt > " + inputFile;
                system(s.data());
                system("rm n.txt");
                //printf("exited child\n");
                exit(0);
            }

    }
    s = "cat " + inputFile;
    system(s.data());

    return 0;
}

#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <pthread.h>
#include <unistd.h>



struct PCB{
    char name[16];
    int32_t processID;
    char status;
    int32_t bt;
    int32_t base;
    int64_t limit;
    char priority;
};

struct Queue{
    std::vector<PCB> pcbQueue;
    int algo;
    double percent;
};

std::vector<Queue> cpusQ;

std::vector<int> done;


bool compareBT(const PCB &a, const PCB &b)
{
    return a.bt > b.bt;
}

bool comparePri(const PCB &a, const PCB &b)
{
    return a.priority > b.priority;
}


void * shortestJob(void * ind){
    pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
    using namespace::std;
    int in = *((int*)ind);
    unsigned int microseconds = 300000;
   // pthread_mutex_lock(&myMutex);
    std::sort(cpusQ[in].pcbQueue.begin(), cpusQ[in].pcbQueue.end(), compareBT);
    //pthread_mutex_unlock(&myMutex);

    for(int i = cpusQ[in].pcbQueue.size() - 1; i >= 0; i--){
        cout << "Cpu: " + to_string(in+1) + " using shortest job first scheduling algorithm" << endl;
        cout << "Executing process: " + to_string(cpusQ[in].pcbQueue[i].processID) + " with a burst time of " + 
        to_string(cpusQ[in].pcbQueue[i].bt) + "\n" << endl;
        usleep(microseconds);

    }

    done[in] = 1;
    return NULL;
}

void * priority(void * ind){

     pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
    using namespace::std;
    int in = *((int*)ind);
    unsigned int microseconds = 300000;
    //pthread_mutex_lock(&myMutex);
    std::sort(cpusQ[in].pcbQueue.begin(), cpusQ[in].pcbQueue.end(), comparePri);
   // pthread_mutex_unlock(&myMutex);

    for(int i = cpusQ[in].pcbQueue.size() - 1; i >= 0; i--){
        cout << "Cpu: " + to_string(in+1) + " using priority scheduling algorithm" << endl;
        cout << "Executing process: " + to_string(cpusQ[in].pcbQueue[i].processID) + " with a priority of " + 
        to_string(cpusQ[in].pcbQueue[i].priority) + "\n" << endl;
        usleep(microseconds);

    }
    done[in] = 1;
    return NULL;
}

void * roundRobin(void * ind){
    pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
    using namespace::std;
    int in = *((int*)ind);
    unsigned int microseconds = 300000;
   // pthread_mutex_lock(&myMutex);
    //pthread_mutex_unlock(&myMutex);

   for(int i = cpusQ[in].pcbQueue.size() - 1; i >= 0; i--){
        cout << "Cpu: " + to_string(in+1) + " using round robin scheduling algorithm" << endl;
        cout << "Executing process: " + to_string(cpusQ[in].pcbQueue[i].processID) + " with a priority of " + 
        to_string(cpusQ[in].pcbQueue[i].priority) + "\n" << endl;
        usleep(microseconds);

    }
    done[in] = 1;
    return NULL;
}


int main(int argc, char *argv[]){
    using namespace::std;
    FILE *f;

    if(argc % 2 != 0){
        cout << "Incorrect number of command line arguments, terminating program..." << endl;
        return(-1);
    }

    int cpuCount = (argc - 2 )/ 2;


    //printf("hello?\n");
    //f = fopen("processes.bin", "rb+");
    std::ifstream file;
    
    file.open(argv[argc- 1], ios::in | ios::binary);
    
    if(!file){
        cout << argv[argc- 1] << endl;
        printf("Error opening file, terminating program... \n");
        return(-1);
    }
    const auto begin = file.tellg();
    file.seekg (0, ios::end);
    const auto end = file.tellg();
    const auto fsize = (end-begin);
    file.seekg(0, ios::beg);
    //string str;

    int pcbCount = fsize/38;
    vector<PCB> pcbs(pcbCount);
    vector<Queue> cpus(cpuCount);

   




    //PCB pcbs[pcbCount];
       
        for(int i = 0 ; i < pcbCount; i++) {
            file.read(pcbs[i].name, sizeof(char)* 16);
            file.read(reinterpret_cast<char*>(&pcbs[i].processID), sizeof(int32_t));
            file.read(reinterpret_cast<char*>(&pcbs[i].status), sizeof(char));
            file.read(reinterpret_cast<char*>(&pcbs[i].bt), sizeof(int32_t));
            file.read(reinterpret_cast<char*>(&pcbs[i].base), sizeof(int32_t));
            file.read(reinterpret_cast<char*>(&pcbs[i].limit), sizeof(int64_t));
            file.read(reinterpret_cast<char*>(&pcbs[i].priority), sizeof(char));
        }
        file.close();

        int totalMem = 0;

         cout << "Number of processes: " + to_string(pcbCount) << endl;
         for(int i = 0; i < sizeof(pcbs); i++){
            totalMem += pcbs[i].limit - pcbs[i].base;
         }
         cout << "Memory amount: " + to_string(totalMem) + "\n"<< endl;

        int count = 0;
        int lo = 0;
        double val;
        int lt;
        for(int i = 1; i < argc - 1; i+=2){
            cpus[count].algo = strtol(argv[i], NULL, 10);
            val = atof(argv[i+1]);
            cpus[count].percent = val;
            lt = (cpus[count].percent * pcbCount) + lo;
            for(int j = lo; j < lt; j++){
                cpus[count].pcbQueue.push_back(pcbs[j]);
            }
            lo += cpus[count].percent * pcbCount;
            count++;
        }

    for(int i = 0; i < cpus.size(); i++){
        cpusQ.push_back(cpus[i]);
        done.push_back(0);
       // cout << cpusQ[i].algo << endl;
        //cout << cpusQ[i].pcbQueue[0].processID << endl;
    }

    int *ind = (int*)malloc(sizeof(int));  

        for(int i = 0; i < cpus.size(); i++){
            *ind = i;
            if(cpus[i].algo == 1){
                pthread_t t;
                pthread_create(&t, NULL, priority, ind);
                //pthread_join (t, NULL);
               // sleep(1);

            }
            else if(cpus[i].algo == 2){
                pthread_t t1;
               pthread_create(&t1, NULL, shortestJob, ind);
              //pthread_join (t1, NULL);
              //sleep(1);
            }
            else if(cpus[i].algo == 3){
              pthread_t t2;
                pthread_create(&t2, NULL, roundRobin, ind);
              // pthread_join (t2, NULL);
             // sleep(1);
            }
            sleep(1);
        }
         bool run = true;
        while(run){
            for(int i = 0; i < done.size(); i++){
                if(done[i] == 0) break;
                else if(i == done.size() - 1 && done[i] == 1) run = false;
            }
        }



    return 0;
}

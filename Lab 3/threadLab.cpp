#include <iostream>
#include <limits>
#include<stdio.h>
#include<stdlib.h>
#include <vector>
#include <iterator>
#include <fstream>
#include <ctime>
#include <random>
#include<pthread.h>

int M, n;


bool isSorted(std::vector<int> arr) {
    for (int i = 1; i < arr.size(); i++) if (arr[i] < arr[i - 1]) return false;
    return true;
}


//All sorting functions sourced from geeksforgeeks.com!

void insertionSort(std::vector<int> &arr) {
    int n = arr.size();
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void swap(int *xp, int *yp) {
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void bubbleSort(std::vector<int> &arr) {
    int n = arr.size();
    int i, j;
    for (i = 0; i < n - 1; i++) for (j = 0; j < n - i - 1; j++) if (arr[j] > arr[j + 1]) swap(&arr[j], &arr[j + 1]);

}

int partition(std::vector<int> &arr, int low, int high) {
    int pivot = arr[high];    // pivot
    int i = (low - 1);  // Index of smaller element

    for (int j = low; j <= high - 1; j++) {
        // If current element is smaller than or
        // equal to pivot
        if (arr[j] <= pivot) {
            i++;    // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}


void quickSort(std::vector<int> &arr, int low, int high) {
    if (low < high) {
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partition(arr, low, high);

        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }

}

void * problem2Solver(void * threadID) {
    using namespace ::std;
    string input, inputFile, output;
    pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
    vector<int> nums, temp;
    int index1, index2, sort, a, iter;
    inputFile = "numbers.txt";
    iter = 0;
    int tid = (int)(long)threadID;

    while (1) {
        //pthread_mutex_lock(&myMutex);
        std::fstream myfile(inputFile, std::ios_base::in);
        while (myfile >> a) nums.push_back(a);
        myfile.close();


        if (isSorted(nums)) {
            //output = "\n\nThread ID: " + to_string(tid) + "\n\nNumber of iterations: " + to_string(iter);
            //output = "\n\nNumber of iterations: " + to_string(iter);
            //printf("%s",output.data());
            //pthread_mutex_unlock(&myMutex);
           // pthread_exit(NULL);
            break;
            //return (void *)0;
        }

        iter++;
        index1 = (rand() % (nums.size() + 5) - 2);
        index2 = (rand() % (nums.size() + 5) - 2);
        sort = rand() % 3 + 1;

        if (index1 < 0)index1 = 0;
        else if(index1 > nums.size() - 1) index1 = nums.size() -1;
        if (index2 < 0)index2 = 0;
        else if(index2 > nums.size() - 1) index2 = nums.size() -1;

        if (index1 < index2) {
            for (int i = index1; i <= index2; i++) {
                temp.push_back(nums[i]);
            }
        } else if (index1 > index2) {
            for (int i = index2; i <= index1; i++) {
                temp.push_back(nums[i]);
            }
        } else {
            temp.clear();
            nums.clear();
            continue;
        }

        if (sort == 1) insertionSort(temp);
        else if (sort == 2) bubbleSort(temp);
        else if (sort == 3) quickSort(temp, 0, temp.size() - 1);

        std::ofstream ofs(inputFile, std::ofstream::trunc);

        if (index1 < index2) {
            for (int i = 0; i < index1; i++){
                ofs << std::to_string(nums[i]) + "\t";
                output = to_string(nums[i]) + " ";
                printf("%s",output.data());
            }
            for (int i = 0; i <= (index2 - index1); i++){
                ofs << std::to_string(temp[i]) + "\t";
                output = to_string(temp[i]) + " ";
                printf("%s",output.data());
            }
            for (int i = (index2 + 1); i < nums.size(); i++) {
                ofs << std::to_string(nums[i]) + "\t";
                output = to_string(nums[i]) + " ";
                printf("%s",output.data());
            }
            printf("\n\n");
        } else if (index2 < index1) {
            for (int i = 0; i < index2; i++){
                ofs << std::to_string(nums[i]) + "\t";
                output = to_string(nums[i]) + " ";
                printf("%s",output.data());
            }
            for (int i = 0; i <= (index1 - index2); i++) {
                ofs << std::to_string(temp[i]) + "\t";
                output = to_string(temp[i]) + " ";
                printf("%s",output.data());
            }
            for (int i = (index1 + 1); i < nums.size(); i++) {
                ofs << std::to_string(nums[i]) + "\t";
                output = to_string(nums[i]) + " ";
                printf("%s",output.data());
            }
            printf("\n\n");
        }
        ofs.close();
        temp.clear();
        nums.clear();
        //pthread_mutex_unlock(&myMutex);
    }

    //pthread_mutex_unlock(&myMutex);

}

void * problem2() {

    using namespace ::std;
   int a = 0;

    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    srand((time_t)ts.tv_nsec);


    std::ofstream outfile ("numbers.txt");
    for(int i = 0; i < n*n; i++){
        a = (rand() % (500) - 200);
        outfile << to_string(a) + "\t";
    }
    outfile.close();


    pthread_t threads[M];
    for (int i = 0; i < M; i++) {
        pthread_create(&threads[i], NULL, problem2Solver, (void *)&i);
    }
    for (int i = 0; i < M; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\n\nProblem 2 finished!\n\n");
    //pthread_exit(NULL);
    //return (void *)0;
}

bool allZeroesOrOnes(std::vector<uint64_t> fileData){
    int zc = 0;
    int oc = 0;
    for(int i = 0; i < n*n; i++){
        if(fileData[i] == 0) zc++;
        else if(fileData[i] == 1) oc++;
    }
    if(oc == n*n || zc == n*n) return true;
    else return false;
}

void * problem1Solver(void * threadID) {
    using namespace::std;
    int a, index1, index2, cz, co, iter;
    iter = 0;
    string output;
    std::vector<uint64_t> temp;
    string filename("matrix.dat");

    while(1){
        //pthread_mutex_lock(&myMutex);
        std::streampos fileSize;
        std::ifstream file(filename, std::ios::binary);

        // get its size:
        file.seekg(0, std::ios::end);
        fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        // read the data:
        std::vector<uint64_t> fileData(fileSize);
        file.read((char*) &fileData[0], fileSize);

        for(int i = 0; i < n*n; i++) temp.push_back(fileData[i]);
        //cout << "\n\n";

        if(allZeroesOrOnes(temp)){
            temp.clear();
            fileData.clear();
            //output = "\n\nNumber of iterations: " + to_string(iter);
           // printf("%s",output.data());
            //cout << "\n\nProblem 1 finished!\n\n";
            break;
        }

        iter++;
        std::vector<std::vector<int>> vect(n, std::vector<int>(n));

        for(int i = 0; i < n; i++){
            for(int j = 0; j< n; j++){
                vect[i][j] = fileData.front();
                fileData.erase(fileData.begin());
            }
        }
        index1 = rand() % n;
        index2 = rand() % n;
        co = 0;
        cz = 0;

        if(index1 == 0 && index2==0){
            if(vect[0][1] == 0) cz++;
            else co++;
            if(vect[1][1] == 0) cz++;
            else co++;
            if(vect[1][0] == 0) cz++;
            else co++;
            if(co > cz) vect[index1][index2] = 0x1;
            else vect[index1][index2] = 0x0;
        }
        else if(index1 == 0 && index2==n-1){
            if(vect[0][n-2] == 0) cz++;
            else co++;
            if(vect[1][n-2] == 0) cz++;
            else co++;
            if(vect[1][n-1] == 0) cz++;
            else co++;
            if(co > cz) vect[index1][index2] = 0x1;
            else vect[index1][index2] = 0x0;
        }
        else if(index1 == n-1 && index2==0){
            if(vect[n-2][0] == 0) cz++;
            else co++;
            if(vect[n-2][1] == 0) cz++;
            else co++;
            if(vect[n-1][1] == 0) cz++;
            else co++;
            if(co > cz) vect[index1][index2] = 0x1;
            else vect[index1][index2] = 0x0;
        }
        else if(index1 == n-1 && index2==n-1){
            if(vect[n-1][n-2] == 0) cz++;
            else co++;
            if(vect[n-2][n-1] == 0) cz++;
            else co++;
            if(vect[n-2][n-2] == 0) cz++;
            else co++;
            if(co > cz) vect[index1][index2] = 0x1;
            else vect[index1][index2] = 0x0;
        }
        else if(index1 == 0){
            if(vect[index1][index2 - 1] == 0) cz++;
            else co++;
            if(vect[index1][index2 + 1] == 0) cz++;
            else co++;
            if(vect[index1 + 1][index2 + 1] == 0) cz++;
            else co++;
            if(vect[index1 + 1][index2] == 0) cz++;
            else co++;
            if(vect[index1 + 1][index2 - 1] == 0) cz++;
            else co++;
            if(co > cz) vect[index1][index2] = 0x1;
            else vect[index1][index2] = 0x0;

        }
        else if(index2 == 0){
            if(vect[index1 - 1][index2] == 0) cz++;
            else co++;
            if(vect[index1 + 1][index2] == 0) cz++;
            else co++;
            if(vect[index1 + 1][index2 + 1] == 0) cz++;
            else co++;
            if(vect[index1][index2 + 1] == 0) cz++;
            else co++;
            if(vect[index1 - 1][index2 + 1] == 0) cz++;
            else co++;
            if(co > cz) vect[index1][index2] = 0x1;
            else vect[index1][index2] = 0x0;
        }
        else if(index1 == n-1){
            if(vect[index1][index2 - 1] == 0) cz++;
            else co++;
            if(vect[index1 - 1][index2 - 1] == 0) cz++;
            else co++;
            if(vect[index1 - 1][index2] == 0) cz++;
            else co++;
            if(vect[index1 - 1][index2 + 1] == 0) cz++;
            else co++;
            if(vect[index1][index2 + 1] == 0) cz++;
            else co++;
            if(co > cz) vect[index1][index2] = 0x1;
            else vect[index1][index2] = 0x0;
        }
        else if(index2 == n-1){
            if(vect[index1 + 1][index2] == 0) cz++;
            else co++;
            if(vect[index1 + 1][index2 - 1] == 0) cz++;
            else co++;
            if(vect[index1][index2 - 1] == 0) cz++;
            else co++;
            if(vect[index1 - 1][index2 - 1] == 0) cz++;
            else co++;
            if(vect[index1 - 1][index2] == 0) cz++;
            else co++;
            if(co > cz) vect[index1][index2] = 0x1;
            else vect[index1][index2] = 0x0;
        }
        else{
            if(vect[index1 - 1][index2] == 0) cz++;
            else co++;
            if(vect[index1 - 1][index2 + 1] == 0) cz++;
            else co++;
            if(vect[index1][index2 + 1] == 0) cz++;
            else co++;
            if(vect[index1 + 1][index2 + 1] == 0) cz++;
            else co++;
            if(vect[index1 + 1][index2] == 0) cz++;
            else co++;
            if(vect[index1 + 1][index2 - 1] == 0) cz++;
            else co++;
            if(vect[index1][index2 - 1] == 0) cz++;
            else co++;
            if(vect[index1 - 1][index2 - 1] == 0) cz++;
            else co++;
            if(co > cz) vect[index1][index2] = 0x1;
            else vect[index1][index2] = 0x0;
        }

        fileData.clear();
        temp.clear();

        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                fileData.push_back(vect[i][j]);
                output = to_string(vect[i][j])+" ";
                printf("%s",output.data());
            }
            printf("\n");
        }
        printf("\n");

        std::ofstream out;
        uint64_t hex;
        std::ofstream ofs("matrix.dat", std::ofstream::trunc);
        ofs << "";
        ofs.close();
        out.open("matrix.dat", std::ios::app | std::ios::binary);
        for(int i = 0; i < n*n; i++){
            hex = fileData[i];
            out.write(reinterpret_cast<char*>(&hex), sizeof(hex));
        }
        //pthread_mutex_unlock(&myMutex);
        out.close();
    }


}

void problem1(){
    using namespace::std;
    int a, index1, index2, cz, co, iter;
    iter = 0;
    string output;
    std::vector<uint64_t> temp;

    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    srand((time_t)ts.tv_nsec);

    //srand( (unsigned)time( NULL ) );
    std::fstream outfile;
    uint64_t hex;

    std::ofstream ofs("matrix.dat", std::ofstream::trunc);
    ofs << "";
    ofs.close();

    outfile.open("matrix.dat", std::ios::app | std::ios::binary);
    for(int i = 0; i < n*n; i++){
        a = rand() % 2;
        if( a == 0){
            hex = 0x0;
            outfile.write(reinterpret_cast<char*>(&hex), sizeof(hex));
            //cout << hex;
        }
        else if( a== 1){
            hex = 0x1;
            outfile.write(reinterpret_cast<char*>(&hex), sizeof(hex));
            //cout << to_string(hex);
        }
    }

    //cout << "\n\n";
    outfile.close();

    pthread_t threads[M];
    for (int i = 0; i < 1; i++) {
        pthread_create(&threads[i], NULL, problem1Solver, (void *)&i);
    }
    for (int i = 0; i < 1; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\n\nProblem 1 finished! \n\n");

   // problem1Solver((void* )2);




}

int main(int argc, char* argv[]) {
    using namespace ::std;
    int input, error;
    string inputM;
    int exitVal;

    try {
        n = stoi(argv[1]);
    } catch (exception &e) {
        //cout << e.what() << " error... ";
        cout << "Invalid command line argument for n, aborting program...\n\n";
        return -1;
    }



    while (1) {
        while (1) {
            cout << "Enter an integer value for M, representing number of threads:  ";
            cin >> inputM;
            cout << "\n";
            try {
                M = stoi(inputM);
                break;
            } catch (exception &e) {
                cout << e.what() << " error... ";
                cout << "Incorrect input for M, try again...\n\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        cout << "Enter the number 1 to run problem #1, 2 to run problem #2, or 3 to exit:  ";
        cin >> input;
        cout << "\n";
        if (input == 3) break;
        else if (input == 2) {
            cout << "Running problem 2...\n\n";
            problem2();
        } else if (input == 1) {
            cout << "Running problem 1...\n\n";
            problem1();
        } else {
            cout << "\nIncorrect input... Please enter 1, 2, or 3 next time\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return 0;
}

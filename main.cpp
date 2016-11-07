#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <sstream>
#include <fstream>

#define PTHREAD 0
#if PTHREAD
#include <pthread.h>
#include <thread>
#endif // threaded test execution and output

#define DEBUG 0
#define TYPE int
#define SRAND 1
#define LIMIT 100000
#define INCREMENT 250

using namespace std;

TYPE Median(TYPE *arr);
TYPE Select(TYPE *arr, int l, int m, int h);
int Partition(TYPE *arr, int l, int h);
TYPE bruteForceMedian(TYPE *A);
TYPE* generate_array(int seed);

typedef struct {
    TYPE* A;
    TYPE median;
    unsigned long long basicOps;
} median_data;

void CalculateBruteForce(median_data* data);
void CalculateMedian(median_data* data);

int N=0;
median_data* bruteForce;
median_data* medianData;

int main()
{
    ofstream opCSV("operations.csv");
    ofstream timeCSV("times.csv");
    opCSV << "N,Bruteforce,Median" << endl;
    timeCSV << "N,Bruteforce,Median" << endl;
    for (N=1;N<LIMIT;N+=INCREMENT) {
        //do thing and output result
        cout << "Performing "<< N << endl;
        double bruteForceTime;
        double medianTime;
        unsigned long long basicBOps = 0;
        unsigned long long basicMOps = 0;

        for (int j = 0; j<10;j++) {
            TYPE* mArr = generate_array(SRAND+j);
            TYPE* bArr = generate_array(SRAND+j);
            medianData = new median_data();
            medianData->A = mArr;
            bruteForce = new median_data();
            bruteForce->A = bArr;
            clock_t timer;
            timer = clock();
            Median(medianData->A);
            timer = clock()-timer;
            medianTime = (double)timer/CLOCKS_PER_SEC;
            timer = clock();
            bruteForceMedian(bruteForce->A);
            timer = clock()-timer;
            bruteForceTime += (double)timer/CLOCKS_PER_SEC;
            basicBOps += bruteForce->basicOps;
            basicMOps += medianData->basicOps;
        }
        bruteForceTime /= 10;
        medianTime /= 10;
        basicBOps /= 10;
        basicMOps /= 10;
        opCSV << N <<","<< basicBOps <<","<< basicMOps << endl;
        timeCSV << N << "," << bruteForceTime << "," << medianTime << endl;
    }
    opCSV.close();
    timeCSV.close();
    return 0;
}

TYPE* generate_array(int seed) {
    srand(seed);
    TYPE *returnArray = new TYPE[N];
    for (int i=0;i<N;i++) {
        returnArray[i] = rand();
    }
    return returnArray;
}

TYPE Median(TYPE *arr) {
    if (N == 1) {
        return arr[0];
    } else {
        return Select(arr, 0, (int)N/2,N-1);
    }
}

TYPE Select(TYPE *arr, int l, int m, int h){
    int pos = Partition(arr,l,h);
    if (pos == m) {
        return arr[pos];
    }
    if (pos > m) {
        return Select(arr,l,m,pos-1);
    }
    if (pos < m) {
        return Select(arr,pos+1,m,h);
    }
}

int Partition(TYPE *arr, int l, int h) {
    int pivotval = arr[l];
    int pivotloc = l;
    for (int j = l+1; j <= h; j++) {
        if (arr[j] < pivotval) {
            medianData->basicOps++;
            pivotloc = pivotloc + 1;
            swap(arr[pivotloc],arr[j]);
        }
    }
    swap(arr[l],arr[pivotloc]);
    return pivotloc;
}

// Brute force median
// inputs: array and its length
TYPE bruteForceMedian(TYPE *A) {

    int i, j, numsmaller, numequal = 0;
    int k = (N+1) / 2;
    int basicOperation = 0;

    if (DEBUG) {
        cout << "k = " << k << endl;
    }

    for(i = 0; i < N; i++){
        numsmaller = 0;
        numequal = 0;
        for(j = 0; j < N; j++) {
            // Count the basic operation here
            basicOperation++;
            bruteForce->basicOps++;

            if(A[j] < A[i]) {
                numsmaller++;
            }
            else {
                 if(A[j] == A[i]) {
                        numequal++;
                    }
                 }
            }
            if(numsmaller < k && k <= (numsmaller+numequal)) {
                if (DEBUG) {
                    cout << "The basic operation count is: " << basicOperation << "smaller " << numsmaller << "equal " << numequal << endl;
                }
               return A[i];
               }
    }
}

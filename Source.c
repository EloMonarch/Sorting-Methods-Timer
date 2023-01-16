#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "MTgen.h"
#define _CRT_SECURE_NO_WARNINGS //for visual studio to run the code

unsigned int* createArray(int n);
void fillArray(unsigned int* p, int n);
void initArrASC(unsigned int* p, int n);
void initArrDESC(unsigned int* p, int n);
void printArray(unsigned int* p, int n);
void copyArray(unsigned int* p, unsigned int* q, int n);
void selectionSort(unsigned int* p, int n);
void insertionSort(unsigned int* p, int n);
void shellSort(unsigned int* p, int n);
void quickSort(unsigned int* p, int left, int right);
int sortAndPartition(int p[], int left, int right);
void writeResultsToFile(FILE* f, char arrTypeLabels[3][7], char sortNames[4][11], float times[4][3], int n);


void sort(unsigned int* givenArr, int n, char type[10], float *selTime, float *insTime, float *shellTime, float *quickTime);

int main()
{
    unsigned int* initArr, * Asc, * Desc;
    int n;
    float randSelTime = 0, randInsTime = 0, randShellTime = 0, randQuickTime = 0;
    float ascSelTime = 0, ascInsTime = 0, ascShellTime = 0, ascQuickTime = 0;
    float descSelTime = 0, descInsTime = 0, descShellTime = 0, descQuickTime = 0;
    char arrTypeLabels[3][7], sortNames[4][11];
    float times[4][3];
    FILE* f;

    sMTrand(time(NULL));

    printf("Dose megethos pinaka: ");
    scanf("%d", &n);

    initArr = createArray(n); //Create an initial array
    fillArray(initArr, n); //fill the initial array

    Asc = createArray(n);
    copyArray(initArr, Asc, n); //Temporary set the array same as the initial one
    initArrASC(Asc, n); //Ascent the array

    Desc = createArray(n);
    copyArray(Asc, Desc, n); //Temporary set the array same as the ascenting one
    initArrDESC(Desc, n); //Descent the already ascenting array

    //printf("Init Peinakas: \n");
    //printArray(initArr, n);
    //printf("\n");


    sort(initArr, n, "Random", &times[0][0], &times[1][0], &times[2][0], &times[3][0]);

    sort(Asc, n, "ASC", &times[0][1], &times[1][1], &times[2][1], &times[3][1]);

    sort(Desc, n, "DESC", &times[0][2], &times[1][2], &times[2][2], &times[3][2]);

    strncpy(arrTypeLabels[0], "Random", 6);
    strncpy(arrTypeLabels[1], "ASC", 3);
    strncpy(arrTypeLabels[2], "DESC", 4);

    strncpy(sortNames[0], "Selection", 9);
    strncpy(sortNames[1], "Insertion", 9);
    strncpy(sortNames[2], "Shell", 5);
    strncpy(sortNames[3], "Quick", 5);


    f = fopen("TIMES.txt", "w");

    if (f == NULL) {
        printf("file can't be opened\n");
    }
    else {
        writeResultsToFile(f, arrTypeLabels, sortNames, times, n);

        fclose(f);
    }


    return 0;
}


void sort(unsigned int* givenArr, int n, char type[10], float* selTime, float *insTime, float *shellTime, float *quickTime) { //Sorts the array with 4 different sorting methods and times them individually
    int* SelArray, * InsArray, * ShellArray, * QuickArray;
    clock_t  ct0, ct1;

    printf("         Array type: %s \n", type);

    //START: Selection Sort 

    SelArray = createArray(n);
    copyArray(givenArr, SelArray, n);

    printf("Sorting ==> Selection...");
    ct0 = clock();
    selectionSort(SelArray, n);
    ct1 = clock();
    *selTime = (float)(ct1 - ct0) / CLOCKS_PER_SEC;


    //END: Selection Sort


    //START: Insertion Sort

    InsArray = createArray(n);
    copyArray(givenArr, InsArray, n);

    printf("Insertion...");
    ct0 = clock();
    insertionSort(InsArray, n);
    ct1 = clock();
    *insTime = (float)(ct1 - ct0) / CLOCKS_PER_SEC;


    //END: Insertion Sort


    //START: Shell Sort

    ShellArray = createArray(n);
    copyArray(givenArr, ShellArray, n);

    printf("Shell...");
    ct0 = clock();
    shellSort(ShellArray, n);
    ct1 = clock();
    *shellTime = (float)(ct1 - ct0) / CLOCKS_PER_SEC;


    //END: Shell Sort


    //START: Quick Sort

    QuickArray = createArray(n);
    copyArray(givenArr, QuickArray, n);

    printf("Quick...");
    ct0 = clock();
    quickSort(QuickArray, 0, n - 1);
    ct1 = clock();
    *quickTime = (float)(ct1 - ct0) / CLOCKS_PER_SEC;


    //END: Quick Sort

    printf("DONE!\n\n");
}


unsigned int* createArray(int n){
    unsigned int* p;

    p = (unsigned int*)malloc(n * sizeof(unsigned int));

    return p;
}

void fillArray(unsigned int* p, int n){
    int i;

    for (i = 0; i < n; i++)
        p[i] = MTrand();
}

void printArray(unsigned int* p, int n){
    int i;

    for (i = 0; i < n; i++)
        printf("%u ", p[i]);
    printf("\n");
}

void initArrASC(unsigned int* p, int n) { //Sort with ascenting order with quicksort
    int i, j, pivot, temp, first, last;

    first = 0;
    last = n - 1;

    if (first < last) {
        pivot = first;
        i = first;
        j = last;

        while (i < j) {
            while (p[i] <= p[pivot] && i < last)
                i++;
            while (p[j] > p[pivot])
                j--;
            if (i < j) {
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }

        temp = p[pivot];
        p[pivot] = p[j];
        p[j] = temp;
        quickSort(p, first, j - 1);
        quickSort(p, j + 1, last);

    }
}

void initArrDESC(unsigned int* p, int n) { //Descent an ascenting array, swapping the first with the last, second with last-1....
    int i, temp, current;

    current = n - 1;
    for (i = 0; i < n / 2; i++) {
        temp = p[current];

        p[current] = p[i];
        p[i] = temp;
        current--;
    }
}


void copyArray(unsigned int* p, unsigned int* q, int n)
{
    int i;

    for (i = 0; i < n; i++)
        q[i] = p[i];
}


void quickSort(unsigned int* p, int left, int right)
{
    int partPos;
    if (left < right)
    {
        partPos = sortAndPartition(p, left, right);
        quickSort(p, left, partPos);
        quickSort(p, partPos + 1, right);
    }
}

int sortAndPartition(int p[], int left, int right)
{
    int mid, i, j, pivot, temp;
    mid = (left + right) / 2;
    pivot = p[mid];
    i = left - 1;
    j = right + 1;
    while (i < j)
    {
        do
        {
            i = i + 1;
        } while (p[i] < pivot);
        do
        {
            j = j - 1;
        } while (p[j] > pivot);
        if (i < j)
        {
            temp = p[i];
            p[i] = p[j];
            p[j] = temp;
        }
    } // end of while (i<j)
    return j;
}

void selectionSort(unsigned int* p, int n) {
    int i, k, j, min;

    for (i = 0; i < n - 1; i++)
    {
        k = i;
        min = p[i];
        for (j = i + 1; j < n; j++)
        {
            if (p[j] < min)
            {
                k = j;
                min = p[j];
            }
        }
        p[k] = p[i];
        p[i] = min;
    }

}

void insertionSort(unsigned int* p, int n) {
    int step, key, j;

    for (step = 1; step < n; step++) {
        key = p[step];
        j = step - 1;

        while (key < p[j] && j >= 0) {
            p[j + 1] = p[j];
            --j;
        }
        p[j + 1] = key;
    }
}

void shellSort(unsigned int* p, int n) {
    int i, j, interval, temp;

    for (interval = n / 2; interval > 0; interval /= 2) {
        for (i = interval; i < n; i += 1) {
            temp = p[i];
            j;
            for (j = i; j >= interval && p[j - interval] > temp; j -= interval) {
                p[j] = p[j - interval];
            }
            p[j] = temp;
        }
    }
}

void writeResultsToFile(FILE* f, char arrTypeLabels[3][7], char sortNames[4][11], float times[4][3], int n) {
    fprintf(f, "Array size: %d\n", n);
    fprintf(f, "==================\n");
    fprintf(f, "            Random         ASC        DESC\n");
    fprintf(f, "Selection  %7.3f       %7.3f    %7.3f\n", times[0][0], times[0][1], times[0][2]);
    fprintf(f, "Insertion  %7.3f       %7.3f    %7.3f\n", times[1][0], times[1][1], times[1][2]);
    fprintf(f, "Shell      %7.3f       %7.3f    %7.3f\n", times[2][0], times[2][1], times[2][2]);
    fprintf(f, "Quick      %7.3f       %7.3f    %7.3f\n", times[3][0], times[3][1], times[3][2]);

}
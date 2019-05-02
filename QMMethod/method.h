//load modules
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

// macros
#define TRUE 1
#define FALSE 0
#define NO_TERM -1
#define MAX_VARS 4
#define START_VAR 'a'
#define NEGATIVE '`'
#define MAX_TERMS (int)pow(2, MAX_VARS)

typedef int bool;

// global variables
int a[8];

// structures
typedef struct passStruct {
	int size, sizeOfMinTerm, sizeOfDontCare;
	int* minTerm;
	int* dontCare;
} passStruct;


// funcitons
void input(passStruct, bool);
void intToBinCharArray(int , char[]);
bool CheckSummariable(char[], char[], char[]);
bool CheckPowerOfTwo(int);
int countOneOfBinary(int);
int static compare(const void*, const void*);
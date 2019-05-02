// load modules
#include <stdio.h>
#include <stdlib.h>

#include "method.h"

// program constants
#define TRUE 1
#define FALSE 0

// debugging constants
#define IS_DEBUG_MODE TRUE
#define consolePause(isDebugMode) {if (isDebugMode) {printf("\n"); system("pause");}}

#define SIZE_OF_MINTERM 3
#define SIZE_OF_DONTCARE 2

void main()
{
	// TODO 정해진 입력값 대신 사용자가 입력할 수 있게끔 변경
	passStruct passing;

	passing.size = 11;

	passing.sizeOfMinTerm = SIZE_OF_MINTERM;
	passing.minTerm = malloc(SIZE_OF_MINTERM * sizeof(int));
	for (int i = 0; i < SIZE_OF_MINTERM; i++) 
	{
		passing.minTerm[i] = i;
	}

	passing.sizeOfDontCare = SIZE_OF_DONTCARE;
	passing.dontCare = malloc(SIZE_OF_DONTCARE * sizeof(int));
	passing.dontCare[0] = 4;
	passing.dontCare[1] = 5;

	input(passing, IS_DEBUG_MODE);

	//free(passing.minTerm);
	//free(passing.dontCare);

	consolePause(IS_DEBUG_MODE);
}
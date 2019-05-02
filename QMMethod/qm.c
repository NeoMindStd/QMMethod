// load modules
#include <stdio.h>
#include <stdlib.h>

#include "method.h"

// program constants
#define TRUE 1
#define FALSE 0
#define scanf scanf_s

// debugging constants
#define IS_DEBUG_MODE TRUE
#define consolePause(isDebugMode) {if (isDebugMode) {printf("\n"); system("pause");}}

//#define SIZE_OF_MINTERM 3
//#define SIZE_OF_DONTCARE 2

void main()
{
	// TODO ������ �Է°� ��� ����ڰ� �Է��� �� �ְԲ� ����
	passStruct passing;
	bool inputCounter = FALSE;
	/*
	do {
		printf("���� ������ ��Դϱ�? (2�� �������� �ƴ� ��� ���� ���� Don't Care Condition���� ���) ");
		scanf("%d", &passing.size);
	} while (passing.size < 0 && passing.size >= MAX_TERMS);
	*/
	passing.size = MAX_TERMS;

	do {
		printf("F(");
		for (int i = 0; i < MAX_VARS; i++) {
			printf("%c, ", 'a' + i);
		}
		printf("\b\b) =\n��m : ");
		int minTerms[16];
		passing.minTerm = minTerms;
		for (int i = 0; TRUE; i++)
		{
			scanf("%d", &minTerms[i]);
			if (minTerms[i] == -1) {
				passing.sizeOfMinTerm = i;
				minTerms[i] = 0;
				break;
			}
			bool inputtedFlag = FALSE;
			if (minTerms[i] < 0 || minTerms[i] >= MAX_TERMS || inputtedFlag) i--;
		}

		printf("+\n��d : ");
		int dontCare[16];
		passing.dontCare = dontCare;
		for (int i = 0; TRUE; i++)
		{
			scanf("%d", &dontCare[i]);
			if (dontCare[i] == -1) {
				passing.sizeOfDontCare = i;
				dontCare[i] = 0;
				break;
			}
			bool inputtedFlag = FALSE;
			if (dontCare[i] < 0 || dontCare[i] >= MAX_TERMS || inputtedFlag) i--;
		}
	} while (passing.sizeOfMinTerm + passing.sizeOfDontCare >= passing.size);

	input(passing, IS_DEBUG_MODE);

	//free(passing.minTerm);
	//free(passing.dontCare);

	consolePause(IS_DEBUG_MODE);
}
#include "method.h"

// 현재 입력 size가 16임을 가정
void input(passStruct passed, bool isDebugMode)
{
	if (isDebugMode) printf("input() running. size: %d\n", passed.size);

	// 만약 주어진 항 크기가 충분치 않다면 그 이후 항은 모두 돈케어로 간주
	while (!CheckPowerOfTwo(passed.size))
	{
		if (isDebugMode)
		{
			printf("before - last dontCare: %d, current size: %d\n",
				passed.dontCare[passed.sizeOfDontCare - 1], passed.sizeOfDontCare);
		}

		//realloc(passed.dontCare, sizeof(int) * passed.sizeOfDontCare+1);	TODO 메모리 충돌 방지를 위해 realloc을 미리계산해서 할것(여기서 하면 쓰레기값 들어감)
		passed.dontCare[passed.sizeOfDontCare++] = passed.size++;

		if (isDebugMode) {
			printf("after - last dontCare: %d, current size: %d\n",
				passed.dontCare[passed.sizeOfDontCare - 1], passed.sizeOfDontCare);
		}
	}


	// 주항과 돈케어를 합쳐서 
	int* total;
	total = malloc(passed.size * sizeof(int));
	for (int i = 0; i < passed.sizeOfMinTerm; i++)
		total[i] = passed.minTerm[i];
	for (int i = 0; i < passed.sizeOfDontCare; i++)
		total[passed.sizeOfMinTerm + i] = passed.dontCare[i];

	qsort(total, passed.sizeOfMinTerm + passed.sizeOfDontCare, sizeof(int), compare);
	for (int i = 0; i < passed.sizeOfMinTerm + passed.sizeOfDontCare; i++)
		printf("%d ", total[i]);

	// 그룹별로 구분
	int first[5][6];
	int counters[MAX_VARS + 1][5] = { { 0, }, };
	int tag[MAX_VARS + 1][MAX_VARS + 1][10][MAX_VARS+1] = { { { {NO_TERM,}, }, }, };
	for (int i = 0; i < MAX_VARS+1; i++)
		for (int j = 0; j < MAX_VARS+1; j++)
			for (int k = 0; k < 10; k++)
				for (int l = 0; l < MAX_VARS; l++)
					tag[i][j][k][l] = -1;
	printf("\n");

	for (int i = 0; i < passed.sizeOfMinTerm + passed.sizeOfDontCare; i++)
	{
		int index = countOneOfBinary(total[i]);
		tag[0][index][counters[0][index]][0] = total[i];
		first[index][counters[0][index]++] = total[i];
	}
	for (int i = 0; i < MAX_VARS+1; i++)
		for (int j = 0; j < MAX_VARS+1; j++)
			for (int k = 0; k < 10; k++)
				for (int l = 0; l < MAX_VARS; l++)
					if (tag[i][j][k][l] != -1) printf("%d ", tag[i][j][k][l]);
	printf("\n");

	printf("\n\n");

	// TODO 주항과 돈케어를 이진수로 변환
	char terms[MAX_VARS + 1][MAX_VARS + 1][10][MAX_VARS] = { { { {0,}, },}, };
	bool used[MAX_VARS + 1][MAX_VARS + 1][10] = { { {FALSE,}, }, };
	for (int i = 0; i < 5; i++) {
		printf("\n-------%d-------\n", i);
		for (int j = 0; j < counters[0][i]; j++) {
			printf("%d ", tag[0][i][j][0]);
		}
	}
	printf("\n\n");

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < counters[0][i]; j++) {
			intToBinCharArray(first[i][j], terms[0][i][j]);
		}
	}

	for (int count = 0; count < 5; count++) {
		for (int i = 0; i < 5-count; i++) {
			for (int j = 0; j < counters[count][i]; j++) {
				for (int k = 0; k < counters[count][i + 1]; k++) {
					bool check = CheckSummariable(terms[count][i][j], terms[count][i + 1][k], terms[count+1][i][counters[count+1][i]]);
					if (check) {
						used[count][i][j] = TRUE;
						used[count][i + 1][k] = TRUE;

						int indexTag, index1, index2;
						indexTag = index1 = index2 = 0;
						while (TRUE) {
							if (index1 >= MAX_VARS || indexTag >= MAX_VARS) {
								//printf("\n중단점\n");
								break;
							}
							int tmp = tag[count][i][j][index1++];
							//if (tmp != -1) printf("\ncount = %d, i = %d, j = %d, index1 = %d, indexTag = %d, counters[count + 1][i] = %d, tmp = %d\n", count, i, j, index1-1, indexTag, counters[count + 1][i], tmp);
							if (tmp != -1) tag[count + 1][i][counters[count + 1][i]][indexTag++] = tmp;
						}
						while (TRUE) {
							if (index2 >= MAX_VARS || indexTag >= MAX_VARS) {
								//printf("\n중단점\n"); 
								break;
							}
							int tmp = tag[count][i + 1][k][index2++];
							//if (tmp != -1) printf("\ncount = %d, i = %d, j = %d, index2 = %d, indexTag = %d, counters[count + 1][i] = %d, tmp = %d\n", count, i, j, index2-1, indexTag, counters[count + 1][i], tmp);
							if (tmp != -1) tag[count + 1][i][counters[count + 1][i]][indexTag++] = tmp;
						}

						counters[count + 1][i]++;
					}
				}
			}
		}
	}
	bool table[30][16] = { {FALSE,}, };
	int tableRowSize = 0;
	char answer[30][MAX_VARS];
	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < MAX_VARS; j++) {
			answer[i][j] = " ";
		}
	}

	for (int count = 0; count < 5; count++) {
		printf("===================%d회차===================\n", count+1);
		for (int i = 0; i < 5-count; i++) {
			printf("-------%d-------\n", i);
			for (int j = 0; j < counters[count][i]; j++) {
				if (j > 0) {
					bool overlap = TRUE;
					for (int k = 0; k < MAX_VARS; k++) {
						if (terms[count][i][j][k] != terms[count][i][j - 1][k]) overlap = FALSE;
					}
					if (overlap) break;
				}
				for (int k = 0; k < MAX_VARS; k++) {
					printf("%c", terms[count][i][j][k]);
				}
				int l = 0;
				while (TRUE) {
					int tmp = tag[count][i][j][l++];
					if (tmp == -1 || l > MAX_VARS) break;
					if(l == 1) printf(" (");
					printf("%d, ", tmp);
					if (!used[count][i][j]) table[tableRowSize][tmp] = TRUE;
				}
				if (l > 1) {
					printf("\b\b) ");
					if (!used[count][i][j]) {
						printf("unused");
						for (int ii = 0; ii < MAX_VARS; ii++) {
							answer[tableRowSize][ii] = terms[count][i][j][ii];
						}
						tableRowSize++;
					}
					printf("\n");
				}
				else printf("\n");
			}
		}
	}

	printf("\n\n");
	for (int i = 0; i < MAX_VARS + 1; i++) {
		for (int j = 0; j < MAX_VARS + 1; j++) {
			printf("%d ", counters[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	printf("\n================");
	printf("\n==Table Figure==\n");
	printf("================\n");
	for (int i = 0; i < tableRowSize; i++) {
		for (int j = 0; j < MAX_TERMS; j++) {
			printf("%c", table[i][j] ? 'X' : '_');
		}
		printf("\n");
	}

	// 통합항에서 필수항을 선정하고 간소화. 돈케어에 대해선 나중에 생각. 일단 모두 주항취급
	// 테이블의 세로 합이 1인 열에서 값이 있는부분 선택 후 해당 행 모두 0처리, 해당 행의 값이 있는 부분의 열을 모두 0처리
	// 즉, 현재 주항차트에서 모든 열의 합이 1보다 큰 경우는 처리하지 못함

	// TODO 돈케어는 간소화 과정에서 따로 처리하기
	// TODO 이후 남은 것에 대해 다시 반복. 만약 열의 합이 모두 1보다 큰 경우는 따로 처리
	int primeRows[30] = { 0, };
	int primeIndex = 0;
	int primeIndexBefore = 0;
	while (TRUE) {
		for (int j = 0; j < MAX_TERMS; j++) {
			int columnSum = 0;
			int tmp;
			for (int i = 0; i < tableRowSize; i++) {
				columnSum += table[i][j];
				if (columnSum == 1 && table[i][j]) {
					tmp = i;
				}
			}
			//printf("%d", columnSum);
			if (columnSum == 1) {
				primeRows[primeIndex++] = tmp;
			}
		}
		if (primeIndex == primeIndexBefore) break;
		for (int i = primeIndexBefore; i < primeIndex; i++) {
			for (int j = 0; j < MAX_TERMS; j++) {
				if (table[primeRows[i]][j]) {
					for (int k = 0; k < tableRowSize; k++) {
						table[k][j] = FALSE;
					}
				}
			}/*
			printf("\n================");
			printf("\n==Table Figure==\n");
			printf("================\n");
			for (int iii = 0; iii < tableRowSize; iii++) {
				for (int jjj = 0; jjj < MAX_TERMS; jjj++) {
					printf("%c", table[iii][jjj] ? 'X' : '_');
				}
				printf("\n");
			}*/
		}
		primeIndexBefore = primeIndex;

		printf("\n\n================");
		printf("\n==Table Figure==\n");
		printf("================\n");
		for (int i = 0; i < tableRowSize; i++) {
			for (int j = 0; j < MAX_TERMS; j++) {
				printf("%c", table[i][j] ? 'X' : '_');
			}
			printf("\n");
		}
	}
	printf("\n");
	for (int i = 0; i < primeIndex; i++) {
		printf("%d ", primeRows[i]);
	}
	printf("\n");

	// 간소화 결과 출력
	printf("\n간소화 된 논리식 : ");
	for (int i = 0; i < primeIndex; i++) {
		char terms[MAX_VARS * 2];
		int index = 0;
		for (int j = 0; j < MAX_VARS; j++) {
			char tmp = answer[primeRows[i]][j];
			switch (tmp) {
			case '0' :
				terms[index++] = START_VAR + j;
				terms[index++] = NEGATIVE;
				break;
			case '1' :
				terms[index++] = START_VAR + j;
				break;
			}
		}
		terms[index] = '\0';
		printf("%s + ", terms);
	}
	printf("\b\b  \n");
}

void intToBinCharArray(int a, char b[]) {
	int count = MAX_VARS;

	do {
		b[--count] = '0' + (char)(a & 1);
		a = a >> 1;
	} while (count);

}

bool CheckSummariable(char a[MAX_VARS], char b[MAX_VARS], char result[MAX_VARS]) {
	int diffCount = 2;


	for (int i = 0; i < MAX_VARS; i++) {
		if (a[i] != b[i]) {
			if (a[i] == '_' || b[i] == '_') {
				diffCount = -1;
			}
			else {
				diffCount--;
			}
		}
	}

	bool isSummarizable = diffCount == 1;
	if (isSummarizable) {
		for (int i = 0; i < MAX_VARS; i++) {
			if (a[i] != b[i]) {
				result[i] = '_';
			}
			else {
				result[i] = a[i];
			}
		}
	}
	return isSummarizable;
}

bool CheckPowerOfTwo(int nNum)
{
	int nResult = nNum & (nNum - 1);

	if (nResult == 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

int countOneOfBinary(int n) {
	int i;
	for (i = 0; n != 0; i++) {
		n &= (n - 1);
	}
	return i;
}

int static compare(const void* first, const void* second)
{
	if (*(int*)first > *(int*)second)
		return 1;
	else if (*(int*)first < *(int*)second)
		return -1;
	else
		return 0;
}
// load module
#include <stdio.h>

// compiler constants
#define bool int

// program constants
#define TRUE 1
#define FALSE 0

// debugging constants
#define IS_DEBUGGING_MODE TRUE
#define consolePause(isDebuggingMode) {if (isDebuggingMode) {printf("\n"); system("pause");}}

void main() {
	printf("Hello World!!");

	consolePause(IS_DEBUGGING_MODE);
}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Tetris.h"


int main(int argc, char const *argv[]) {
	time_t t;
	srand((unsigned) time(&t));
	if(init()) {
		run();
	}

	return 0;
}
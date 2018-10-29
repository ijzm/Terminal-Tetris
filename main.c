#include <stdlib.h>
#include <time.h>

#include "Tetris.h"


int main(int argc, char const *argv[]) {
	srand((unsigned) time(NULL));
	if(init()) {
		run();
	}

	return 0;
}
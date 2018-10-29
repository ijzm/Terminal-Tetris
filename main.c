#include <stdlib.h>
#include <time.h>

#include "Tetris.h"


int main(int argc, char const *argv[]) {
	srand((unsigned) time(NULL)); //Sets up the Random Number Generator Seed
	if(init()) {
		run();                    //Start the game
	}

	return 0;
}
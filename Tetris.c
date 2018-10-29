#ifdef _WIN32
#include <curses.h>
#endif

#ifdef linux
#include <ncurses.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Tetris.h"
#include "Tetrominos.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 24


//Global Variables
WINDOW* win;
int score;
short board[BOARD_WIDTH][BOARD_HEIGHT];
clock_t t;

static struct piece CurrentPiece;
static int SavedPiece = -1;


int init() {
	win = initscr();
	
	cbreak();               //
	curs_set(0);            //Removes Mouse Cursor
	noecho();               //
	keypad(stdscr, TRUE);   //
	nodelay(stdscr, TRUE);  //

	setupColors();

	clear();
	refresh();

	return 1;
}

void setupColors() {
	start_color();
	//If terminal allows:
	init_color(COLOR_BLACK, 0, 0, 0);
	init_color(COLOR_CYAN, 0, 1000, 1000);
	init_color(COLOR_YELLOW, 1000, 1000, 0);
	init_color(COLOR_BLUE, 0, 0, 1000);
	init_color(COLOR_RED, 1000, 650, 0); //ORANGE
	init_color(COLOR_GREEN, 0, 1000, 0);
	init_color(COLOR_WHITE, 1000, 1000, 1000);
	init_color(COLOR_MAGENTA, 1000, 0, 1000);
	init_color(11, 1000, 0, 0); //RED

	init_pair(0, COLOR_WHITE, COLOR_BLACK);
	init_pair(1, COLOR_BLACK, COLOR_CYAN);    // I
	init_pair(2, COLOR_BLACK, COLOR_YELLOW);  // O
	init_pair(3, COLOR_BLACK, COLOR_BLUE);    // L  
	init_pair(4, COLOR_BLACK, COLOR_RED);     // J TODO: Orange
	init_pair(5, COLOR_BLACK, COLOR_GREEN);   // S
	init_pair(6, COLOR_BLACK, 11);   // Z TODO: RED
	init_pair(7, COLOR_BLACK, COLOR_MAGENTA); // T
}


void run() {
	CurrentPiece = getRandomPiece();
	addPiece();

	t = clock();
	while(1) {
		if(handleInput()) {
			break;
		}
		printBoard();
		printNext();
		
		if(((float)(clock() - t))/CLOCKS_PER_SEC > 0.7) { //Calculate Delta Time
			t = clock();
			if(moveDown()) {
				break;
			}
		}

		refresh();
	}
	close();
}

int handleInput() {
	switch(getch()) {
		case KEY_UP:
		case 'w':
		rotatePiece();
		break;
		case KEY_DOWN:
		case 's':
		moveDown();
		break;
		case KEY_LEFT:
		case 'a':
		moveHorizontal(-1);
		break;
		case KEY_RIGHT:
		case 'd':
		moveHorizontal(1);
		break;

		case 'q':
		return 1;
		break;

		case 'z': {
			if(SavedPiece == -1) {
				removePiece();
				SavedPiece = CurrentPiece.piece;
				CurrentPiece = getRandomPiece();
				addPiece();
			} else {
				removePiece();
				int temp = SavedPiece;
				SavedPiece = CurrentPiece.piece;
				CurrentPiece = getRandomPiece();
				CurrentPiece.piece = temp;
				addPiece();
			}
		}
		break;

		case ' ':
		moveBottom();

		break;

		default:
		break;
	}
	
	return 0;
}

void printBoard() {
	int anchor = COLS/2 - BOARD_WIDTH;
	for (int x = -1; x < BOARD_WIDTH+1; ++x) {
		for (int y = -1; y < BOARD_HEIGHT+1; ++y) {
			if(x == -1 || y == -1 || x == BOARD_WIDTH || y == BOARD_HEIGHT){
				mvaddch(y, anchor + 2 * x    , ' ' | A_REVERSE | COLOR_PAIR(0));
				mvaddch(y, anchor + 2 * x +1 , ' ' | A_REVERSE | COLOR_PAIR(0));
				//CHANGE ACS_BLOCK
				continue;
			}


			mvaddch(y, anchor + 2 * x    , ' ' | COLOR_PAIR(board[x][y]));	
			mvaddch(y, anchor + 2 * x + 1, ' ' | COLOR_PAIR(board[x][y]));
		}
	}
	mvprintw(8, COLS/2 + BOARD_WIDTH + 4, "Score: %d", score);
}

void printNext() {
	int anchor = COLS/2 + BOARD_WIDTH + 6;
	for (int x = -1; x < 4+1; ++x) {
		for (int y = -1; y < 4+1; ++y) {
			if(x == -1 || y == -1 || x == 4 || y == 4){
				mvaddch(y + 2, anchor + 2 * x    , ' ' | A_REVERSE | COLOR_PAIR(0));
				mvaddch(y + 2, anchor + 2 * x +1 , ' ' | A_REVERSE | COLOR_PAIR(0));
				continue;
			}
			if(SavedPiece != -1) {
					mvaddch(y + 2, anchor + 2 * x    , ' ' | COLOR_PAIR(tetris[SavedPiece][0][x][y] * (SavedPiece+1)));	
					mvaddch(y + 2, anchor + 2 * x + 1, ' ' | COLOR_PAIR(tetris[SavedPiece][0][x][y] * (SavedPiece+1)));
			}
		}
	}

	//mvprintw(0, COLS/2 + BOARD_WIDTH + 3, "%d", CurrentPiece.piece);
	//mvprintw(1, COLS/2 + BOARD_WIDTH + 3, "%d", score);
}

void close() {
	endwin();
	printf("Game Over\n");
	printf("Score: %d \n", score);
}

void addPiece() {
	for (int y = 0; y < 4; ++y)	{
		for (int x = 0; x < 4; ++x) {
			int newX = CurrentPiece.position.x - x, newY = CurrentPiece.position.y - y;
			if(newX >= 0 && newY >= 0 && board[newX][newY] == 0) {
				board[newX][newY] = tetris[CurrentPiece.piece][CurrentPiece.rotation][x][y] * (CurrentPiece.piece + 1);
			}
		}
	}
}

void removePiece() {
	for (int y = 0; y < 4; ++y)	{
		for (int x = 0; x < 4; ++x) {
			int newX = CurrentPiece.position.x - x, newY = CurrentPiece.position.y - y;
			if(newX >= 0 && newY >= 0 && tetris[CurrentPiece.piece][CurrentPiece.rotation][x][y]) {
				board[newX][newY] = 0;
			}
		}
	}
}

void rotatePiece() {
	removePiece();
	CurrentPiece.rotation = wrap(0, 3, CurrentPiece.rotation, 1);
	if(checkOverlap()) {
		CurrentPiece.rotation = wrap(0, 3, CurrentPiece.rotation, -1);
	}
	addPiece();
}

void moveHorizontal(short dir) {
	removePiece();
	CurrentPiece.position.x += dir;
	if(checkOverlap()) {
		CurrentPiece.position.x -= dir;
	}
	addPiece();
}

bool moveDown() {
		removePiece();
		CurrentPiece.position.y++;
		if(checkOverlap()) {
			CurrentPiece.position.y--;
			addPiece();
			score += removeLines();

			CurrentPiece = getRandomPiece();
			if(checkOverlap()) {
				return 1;
			}
		}
		addPiece();
		return 0;
}

void moveBottom() {
	while(1) {
		removePiece();
		CurrentPiece.position.y++;
		if(checkOverlap()) {
			CurrentPiece.position.y--;
			break;
		}
		addPiece();
	}
	addPiece();
}

bool checkDown() {
	bool isBottom = false;
	return isBottom;
}

int removeLines() {
	int lines = 0;
	for (int y = 0; y < BOARD_HEIGHT; ++y) {
		bool isCleared = true;

		for (int x = 0; x < BOARD_WIDTH; ++x) {
			if(board[x][y] == 0) {
				isCleared = false;
			}
		}

		if(isCleared) {
			for (int tempY = y; tempY > 0; --tempY)	{
				for (int x = 0; x < BOARD_WIDTH; ++x) {
					board[x][tempY] = board[x][tempY - 1];
				}
			}
			lines++;
		}
	}

	return lines;
}

bool checkOverlap() {

	for (int x = 0; x < 4; ++x)	{
		for (int y = 0; y < 4; ++y)	{
			int newX = CurrentPiece.position.x - x, newY = CurrentPiece.position.y - y;
			if(
				(
					board[newX][newY] ||
					newY + 1 > BOARD_HEIGHT ||
					newX + 1 > BOARD_WIDTH ||
					newX < 0 ||
					newY < 0) && 
				tetris[CurrentPiece.piece][CurrentPiece.rotation][x][y]
				) {
				return true;
			}
		}
	}

	return false;
}

int wrap(int min, int max, int current, int amount) {
	if(current + amount > max) {
		return min;
	} else if(current + amount < min) {
		return max;
	} else {
		return current + amount;
	}
}
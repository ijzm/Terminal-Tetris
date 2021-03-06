#include <stdbool.h>

#ifndef TETRIS_H
#define TETRIS_H 

int init();
void setupColors();
void run();
int handleInput();
void printBoard();
void printNext();
void close();
void addPiece();
void removePiece();
int removeLines();
void moveHorizontal(short dir);
bool moveDown();
void moveBottom();
void rotatePiece();
bool checkOverlap();
void savePiece();
int wrap(int, int, int, int);

#endif
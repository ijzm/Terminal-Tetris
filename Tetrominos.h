#ifndef TETROMINOS_H
#define TETROMINOS_H 

enum pieces { 
	PIECE_I,
	PIECE_O,
	PIECE_L,
	PIECE_J,
	PIECE_S,
	PIECE_Z,
	PIECE_T
};

enum rotation {
	ROTATION_0,
	ROTATION_90,
	ROTATION_180,
	ROTATION_270
};

struct coordinate { 
	short x, y;
};

struct piece {
	enum pieces piece;
	enum rotation rotation;
	struct coordinate position;
};

extern const char tetris[7][4][4][4];

struct piece getRandomPiece();

#endif
#ifndef GLOBAL_VARIABLES_H_
#define GLOBAL_VARIABLES_H_

#define SIZE 8 // size of the board

// #define ADDR_IP "127.0.0.1" // IP adress used for server purposes
#define ADDR_PORT 6969 // Adress port we agreed upon beforehand

// integers representing the direction of the movement
#define MOVE_UP -1
#define MOVE_RIGHT 2
#define MOVE_DOWN 1
#define MOVE_LEFT -2

// global variables
extern int player;
extern int finished;

// main board
extern int board[SIZE][SIZE];
extern int tmp_board[SIZE][SIZE];
extern char ai_move[40];

#endif

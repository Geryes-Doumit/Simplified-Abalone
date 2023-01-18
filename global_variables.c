/**
 * \file global_variables.c
 * \author Group 4
 * \brief Global variables
 * \include "global_variables.h"
 * \include "game_basics.h"
*/
#include <stdio.h>
#include <stdlib.h>
#include "global_variables.h"
#include "game_basics.h"

#define SIZE 8 /* Size of the board (= 8) */

/* Integers representing the direction of the movement */
#define MOVE_UP -1
#define MOVE_RIGHT 2
#define MOVE_DOWN 1
#define MOVE_LEFT -2

/* global variables */
int board[SIZE][SIZE];
int tmp_board[SIZE][SIZE];
char ai_move[40];
int player = 1;
int finished = 0;
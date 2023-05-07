/**
 * \file game_basics.c
 * \author Group 4
 * \brief Basic functions for the board, to initialize it and to print it in the terminal.
 * \include "global_variables.h"
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../global/global_variables.h"

void initialize_matrix();
void print_ball(int x);
void print_board();

/**
 * \fn void initialize_matrix()
 * \brief Initializes the board
 */
void initialize_matrix()
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            board[i][j] = 0;
        }
    }
    for (int j = 0; j < SIZE; j++)
    {
        board[0][j] = 2;
        board[SIZE - 1][j] = 1;
    }
    for (int j = 1; j < SIZE - 1; j++)
    {
        board[1][j] = 2;
        board[SIZE - 2][j] = 1;
    }
}

/**
 * \fn void print_ball(int x)
 * \brief Prints a cell
 * \details Depends on the ball presents in each cell
 * 
 * \param x the value of the cell (0/1/2)
 */
void print_ball(int x)
{
    if (x == 0)
        printf(" - ");
    else
        printf(" %i ", x);
}

/**
 * \fn void print_board()
 * \brief Prints the current board
 */
void print_board()
{
    printf("\n");
    printf("     ");
    for (int i = 0; i < SIZE; i++)
    {
        printf(" %i ", i + 1);
    }
    printf("\n");
    printf("\n");
    for (int i = 65; i < (65 + SIZE); i++)
    {
        printf("%c    ", i);
        for (int j = 0; j < SIZE; j++)
        {
            print_ball(board[i - 65][j]);
        }
        printf("\n");
    }
    printf("\n");
}
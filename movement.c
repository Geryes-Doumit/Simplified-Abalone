/**
 * \file movement.c
 * \author Group 4
 * \brief Movement of the balls on the board.
 * \include "global_variables.h"
 * \include "game_basics.h"
 * \include "validity_check.h"
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "global_variables.h"
#include "game_basics.h"
#include "validity_check.h"

int minimum(int a, int b);
int maximum(int a, int b);
void switch_box(int i1, int j1, int i2, int j2);
void shift_piece(int i1, int j1, int i2, int j2);

/**
 * \fn int minimum(int a, int b)
 * \brief Returns the smallest value between two integers.
 * \details Used to see minimum between i1 (or j1) and i2 (or j2).
 * 
 * \param a first integer to compare
 * \param b second integer to compare
 * \return minimum integer
 */
int minimum(int a, int b)
{
    return a < b ? a : b;
}

/**
 * \fn int maximum(int a, int b)
 * \brief Returns the biggest value between two integers.
 * \details Used to see maximum between i1 (or j1) and i2 (or j2).
 * 
 * \param a first integer to compare.
 * \param b second integer to compare.
 * \return maximum integer.
 */
int maximum(int a, int b)
{
    return a > b ? a : b;
}

/**
 * \fn void switch_box(int i1, int j1, int i2, int j2)
 * \brief Switches two cells on the board.
 * 
 * \param i1 line index of first cell.
 * \param j1 column index of first cell.
 * \param i2 line index of second cell.
 * \param j2 column index  of second cell.
 */
void switch_box(int i1, int j1, int i2, int j2)
{
    board[i2][j2] = board[i1][j1];
    board[i1][j1] = 0;
}

/**
 * \fn void shift_piece(int i1, int j1, int i2, int j2)
 * \brief Moves a ball if the movement is allowed.
 * \details Uses other functions to know how many balls have to be moved and if a ball needs to be pushed.
 * 
 * \param i1 line index of departure.
 * \param j1 column index of departure.
 * \param i2 line index of arrival.
 * \param j2 column index  of arrival.
 */
void shift_piece(int i1, int j1, int i2, int j2)
{ 
    /*temporary board*/
    for(int i = 0;i<SIZE;i++){
        for(int j = 0;j<SIZE;j++){
            tmp_board[i][j]=board[i][j];        
        }
    }

    if (is_move_allowed(i1, j1, i2, j2))
    {
        if (is_in_line(i1, j1, i2, j2))
        {
            if (number_of_balls(i1, j1, i2, j2) == 1)
            {
                switch_box(i1, j1, i2, j2);
            }
            else
            {
                int direction = get_direction(i1, j1, i2, j2);
                int sign = direction / abs(direction);

                if (is_in_line(i1, j1, i2, j2))
                {
                    int numToPush = count_balls_behind(i1, j1, i2, j2);
                    if (numToPush > 0 && is_move_allowed(i1, j1, i2, j2) != 2)
                    {
                        if (abs(direction) > 1)
                        {
                            switch_box(i2, j2, i2, j2 + sign * numToPush);
                        }
                        else
                        {
                            switch_box(i2, j2, i2 + sign * numToPush, j2);
                        }
                    }
                    switch_box(i1, j1, i2, j2);
                }
            }
        }
        else
        {
            for (int i = minimum(i1, i2); i <= maximum(i1, i2); i++)
            {
                for (int j = minimum(j1, j2); j <= maximum(j1, j2); j++)
                {
                    if (board[i][j] == 0)
                    {
                        board[i][j] = player;
                    }
                    else
                    {
                        board[i][j] = 0;
                    }
                }
            }
        }
    }
}
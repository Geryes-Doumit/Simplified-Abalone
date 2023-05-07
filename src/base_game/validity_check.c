/**
 * \file validity_check.c
 * \author Group 4
 * \brief Validity of a move 
 * \details Checks if the move is allowed 
 * \include "global_variables.h"
 * \include "game_basics.h"
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../global/global_variables.h"
#include "../base_game/game_basics.h"

int is_move_allowed(int i1, int j1, int i2, int j2);
int is_move_allowed_lateral(int i1, int j1, int i2, int j2);
int get_direction(int i1, int j1, int i2, int j2);
int is_in_line(int i1, int j1, int i2, int j2);
int number_of_balls(int i1, int j1, int i2, int j2);
int count_balls_for_lateral_in_direction(int i1, int j1, int i2, int j2);
int count_balls_behind(int i1, int j1, int i2, int j2);
int border(int i1, int i2, int j1, int j2);
char is_next_cell_outside(int i, int j, int direction);

/**
 * \fn is_in_line(int i1, int j1, int i2, int j2)
 * \brief Checks if the move is of the 'en ligne' type (two or three balls aligned on a vertical or horizontal axis).
 * 
 * \param i1 line index of departure.
 * \param j1 column index of departure.
 * \param i2 line index of arrival.
 * \param j2 column index  of arrival.
 * \return 1 : Move type 'en ligne'.
 * \return 0 : Move type 'transversal'.
 */
int is_in_line(int i1, int j1, int i2, int j2)
{
    return (i1 == i2 || j1 == j2) ? 1 : 0;
}

/**
 * \fn int get_direction()
 * \brief Get the direction of the move.
 *      -1 if it goes up.
 *      +1 if it goes down.
 *      -2 if it goes left.
 *      +2 if it goes right.
 * \param i1 line index of departure.
 * \param j1 column index of departure.
 * \param i2 line index of arrival.
 * \param j2 column index  of arrival.
 * \return direction as described.
 */
int get_direction(int i1, int j1, int i2, int j2)
{
    if ((j1 == j2) || (abs(i1 - i2) == 1 && abs(j1 - j2) == 2))
    {
        if (i1 > i2)
        {
            return MOVE_UP;
        }
        else if (i1 < i2)
        {
            return MOVE_DOWN;
        }
    }
    else if ((i1 == i2) || (abs(i1 - i2) == 2 && abs(j1 - j2) == 1))
    {
        if (j1 > j2)
        {
            return MOVE_LEFT;
        }
        else if (j1 < j2)
        {
            return MOVE_RIGHT;
        }
    }
    else if (count_balls_for_lateral_in_direction(i1, j1, i1, j2) == 1 && count_balls_for_lateral_in_direction(i1, j1, i2, j1) == 2)
    {
        return (get_direction(i1, j1, i1, j2));
    }
    else if (count_balls_for_lateral_in_direction(i1, j1, i2, j1) == 1 && count_balls_for_lateral_in_direction(i1, j1, i1, j2) == 2)
    {
        return (get_direction(i1, j1, i2, j1));
    }
    return 0;
}

/**
 * \fn int number_of_balls(int i1, int j1, int i2, int j2)
 * \brief Get the number of balls that will be moved and that belong to the current player.
 * 
 * \param i1 line index of departure.
 * \param j1 column index of departure.
 * \param i2 line index of arrival.
 * \param j2 column index  of arrival.
 * \return number of ball's player moved.
 */
int number_of_balls(int i1, int j1, int i2, int j2)
{
    int direction = get_direction(i1, j1, i2, j2);
    int sign = direction / abs(direction);
    int count = 0;

    if (abs(direction) == 1)
    {
        for (int i = 0; i1 + i != i2; i = i + sign)
        {
            if (board[i1 + i][j1] != player)
            {
                return 0;
            }
            count++;
        }
    }
    else
    {
        for (int j = 0; j1 + j != j2; j = j + sign)
        {
            if (board[i1][j1 + j] != player)
            {
                return 0;
            }
            count++;
        }
    }
    return count;
}

/**
 * \fn int count_balls_for_lateral_in_direction(int i1, int j1, int i2, int j2)
 * \brief Only used to count the number of balls for a lateral move of two balls.
 * 
 * \param i1 line index of departure.
 * \param j1 column index of departure.
 * \param i2 line index of arrival.
 * \param j2 column index  of arrival.
 * \return number of ball's player moved according to line or column index (when used returns 1 or 2).
 */
int count_balls_for_lateral_in_direction(int i1, int j1, int i2, int j2)
{
    int direction = get_direction(i1, j1, i2, j2);
    int sign = direction / abs(direction);
    int count = 0;

    if (abs(direction) == 1)
    {
        for (int i = 0; i1 + i != i2 + sign; i = i + sign)
        {
            if (board[i1 + i][j1] == player)
            {
                count++;
            }
        }
    }
    else
    {
        for (int j = 0; j1 + j != j2 + sign; j = j + sign)
        {
            if (board[i1][j1 + j] == player)
            {
                count++;
            }
        }
    }
    return count;
}

/**
 * \fn int count_balls_behind(int i1, int j1, int i2, int j2)
 * \brief Get the number of balls to push.
 * \details Counts the ball(s) behind player's balls.
 * 
 * \param i1 line index of departure.
 * \param j1 column index of departure.
 * \param i2 line index of arrival.
 * \param j2 column index  of arrival.
 * \return number of balls that will be pushed.
 * \return -1 if too many opposing balls or any of the current player's balls are in the way.
 */
int count_balls_behind(int i1, int j1, int i2, int j2)
{
    int direction = get_direction(i1, j1, i2, j2);
    int sign = direction / abs(direction);
    int numBall = number_of_balls(i1, j1, i2, j2);
    int count = 0;

    if (abs(direction) == 1)
    {
        for (int i = 0; (abs(i) <= numBall) && (i2 + i >= 0) && (i2 + i < SIZE) && (board[i2 + i][j2] != 0); i = i + sign)
        {
            if (board[i2 + i][j2] == player)
            {
                return -1;
            }
            count++;
            if (is_next_cell_outside(i2 + i, j2, direction) && count < numBall)
            {
                return count;
            }
        }
    }
    else
    {
        for (int j = 0; (abs(j) <= numBall) && (j2 + j >= 0) && (j2 + j < SIZE) && (board[i2][j2 + j] != 0); j = j + sign)
        {
            if (board[i2][j2 + j] == player)
            {
                return -1;
            }
            count++;
            if (is_next_cell_outside(i2, j2 + j, direction) && count < numBall)
            {
                return count;
            }
        }
    }
    if (count >= numBall)
    {
        return -1;
    }
    return count;
}

/**
 * \fn char is_next_cell_outside(int i, int j, int direction)
 * \brief Check if the next cell is inside of the board.
 * 
 * \param i line index of cell.
 * \param j column index of cell.
 * \param direction move direction.
 * \return 1 next cell is outside.
 * \return 0 next cell is in the board.
 */
char is_next_cell_outside(int i, int j, int direction)
{
    char isOutside;
    int sign = direction/abs(direction);
    switch (abs(direction))
    {
    case 1:
        isOutside = i + sign < 0 || i + sign > SIZE - 1;
        break;
    case 2:
        isOutside = j + sign < 0 || j + sign > SIZE - 1;
        break;
    default:
        perror("Calling is_next_cell_outside with incorrect direction");
        break;
    }
    return isOutside;
}

/**
 * \fn int is_move_allowed(int i1, int j1, int i2, int j2)
 * \brief checks if a move is allowed.
 * \details Sees if the move is out of bound, if there are too many ball to push, etc...
 * 
 * \param i1 line index of departure.
 * \param j1 column index of departure.
 * \param i2 line index of arrival.
 * \param j2 column index  of arrival. 
 * \return 0 movement not allowed => Other player wins.
 * \return 1 movement allowed.
 * \return 2 movemennt is a winning move => Current player wins.
 */
int is_move_allowed(int i1, int j1, int i2, int j2)
{
    if (is_in_line(i1, j1, i2, j2) && (abs(i1 - i2) > 3 || abs(j1 - j2) > 3))
    {
        return 0;
    }
    if (!(is_in_line(i1, j1, i2, j2)) && !((abs(i1 - i2) < 3 && abs(j1 - j2) == 1)) && !((abs(j1 - j2) < 3 && abs(i1 - i2) == 1)))
    {
        return 0;
    }
    else if (i1 < 0 || i2 >= SIZE || j1 < 0 || j2 >= SIZE || i2 < 0 || i1 >= SIZE || j2 < 0 || j1 >= SIZE)
    {
        return 0;
    }
    else if (board[i1][j1] != player)
        return 0;

    else if (is_in_line(i1, j1, i2, j2))
    {
        if (abs(i1 - i2) + abs(j1 - j2) > number_of_balls(i1, j1, i2, j2))
        {
            return 0;
        }
        else if (count_balls_behind(i1, j1, i2, j2) > 0)
        {
            if (border(i1, j1, i2, j2))
            {
                return 2;
            }
            
        }
        else if (count_balls_behind(i1, j1, i2, j2) == -1)
        {
            return 0;
        }
        return 1;
    }
    else if (is_move_allowed_lateral(i1, j1, i2, j2))
    {
        return 1;
    }
    return 0;
}

/**
 * \fn int is_move_allowed_lateral(int i1, int j1, int i2, int j2)
 * \brief Checks if a not 'en ligne' move is allowed.
 * \details Checks for move out of bound or too many balls to push (used in is_move_allowed()).
 * 
 * \param i1 line index of departure.
 * \param j1 column index of departure.
 * \param i2 line index of arrival.
 * \param j2 column index  of arrival. 
 * \return 0 : movement not allowed.
 * \return 1 : movement allowed.
 * \return 2 : movement is a winning move.
 */
int is_move_allowed_lateral(int i1, int j1, int i2, int j2)
{
    int abs_dir = abs(get_direction(i1, j1, i2, j2));
    if (abs_dir == 0)
    {
        return 0;
    }
    else if (abs_dir == 1)
    {
        int lateralMove = get_direction(i1, j1, i1, j2);
        int sign = lateralMove / abs(lateralMove);
        for (int j = 0; j1 + j != j2 + sign; j = j + sign)
        {
            if (!is_move_allowed(i1, j1 + j, i2, j1 + j))
            {
                return 0;
            }
        }
    }
    else if (abs_dir == 2)
    {
        int lateralMove = get_direction(i1, j1, i2, j1);
        int sign = lateralMove / abs(lateralMove);
        for (int i = 0; i1 + i != i2 + sign; i = i + sign)
        {
            if (!is_move_allowed(i1 + i, j1, i1 + i, j2))
            {
                return 0;
            }
        }
    }
    return 1;
}

/**
 * \fn int border(int i1, int j1, int i2, int j2)
 * \brief Checks if a ball is pushed outside of the board.
 * 
 * \param i1 line index of departure.
 * \param j1 column index of departure.
 * \param i2 line index of arrival.
 * \param j2 column index  of arrival. 
 * \return 1 if the move pushes the ball outside.
 * \return 0 if not.
 */
int border(int i1, int j1, int i2, int j2)
{
    int direction = get_direction(i1, j1, i2, j2);
    int sign = direction / abs(direction);
    if (abs(direction) == 1)
    {
        int testBorder = i2 + sign * count_balls_behind(i1, j1, i2, j2);
        if (testBorder < 0 || testBorder >= SIZE)
        {
            return 1;
        }
    }
    else
    {
        int testBorder = j2 + sign * count_balls_behind(i1, j1, i2, j2);
        if (testBorder < 0 || testBorder >= SIZE)
        {
            return 1;
        }
    }
    return 0;
}
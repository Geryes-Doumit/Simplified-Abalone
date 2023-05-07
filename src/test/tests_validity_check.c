#include "../base_game/game_basics.h"
#include "../global/global_variables.h"
#include "../base_game/validity_check.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

void run_tests_validity_check();
void test_is_move_allowed();
void test_is_in_line();
void test_count_balls_behind();
void test_border();
void test_count_balls_for_lateral_in_direction();
void remove_all();
void test_get_direction();
void test_number_of_balls();
void test_is_move_allowed_lateral();
void test_is_next_cell_outside();


void run_tests_validity_check()
{
    test_border();
    test_is_move_allowed();
    test_is_in_line();
    test_count_balls_behind();
    test_is_next_cell_outside();
    test_get_direction();
    test_number_of_balls();
    test_count_balls_for_lateral_in_direction();
    test_is_move_allowed_lateral();
}



void test_get_direction()
{
    initialize_matrix();

    // simple movement for one pawn

    assert(get_direction(7, 0, 6, 0)==MOVE_UP);
    printf("Validated : direction UP one pawn \n");
    assert(get_direction(5, 0, 6, 0)==MOVE_DOWN);
    printf("Validated : direction DOWN one pawn\n");
    assert(get_direction(6, 1, 6, 0)==MOVE_LEFT);
    printf("Validated : direction LEFT one pawn\n");
    assert(get_direction(6, 6, 6, 7)==MOVE_RIGHT);
    printf("Validated : direction RIGHT one pawn\n");

    // simple movement for two pawn

    assert(get_direction(6, 1, 5, 2)==MOVE_UP);
    printf("Validated : direction UP two pawns\n");
    board[3][4]=1;
    board[4][4]=1;
    assert(get_direction(3, 4, 5, 4)==MOVE_DOWN);
    printf("Validated : direction DOWN two pawns\n");
    board[5][1]=1;
    assert(get_direction(6, 1, 6, 0)==MOVE_LEFT);
    printf("Validated : direction LEFT two pawns\n");
    board[5][6]=1;
    assert(get_direction(5, 6, 6, 7)==MOVE_RIGHT);
    printf("Validated : direction RIGHT two pans\n");

    remove_all();

    // simple movement for three pawn
    board[6][1]=1;
    board[6][2]=1;
    board[6][3]=1;
    assert(get_direction(6, 1, 5, 3)==MOVE_UP);
    printf("Validated : direction UP three pawns\n");
    assert(get_direction(6, 1, 7, 3)==MOVE_DOWN);
    printf("Validated :direction DOWN three pawns\n");
    assert(get_direction(6, 3, 6, 0)==MOVE_LEFT);
    printf("Validated : direction LEFT three pawns\n");
    assert(get_direction(6, 1, 6, 4)==MOVE_RIGHT);
    printf("Validated : direction RIGHT three pawns\n");

}

void test_is_move_allowed()
{
    initialize_matrix();

    // simple movement

    assert(is_move_allowed(7, 0, 6, 0));
    printf("Validated : move 1 ball top\n");
    assert(is_move_allowed(6, 1, 6, 0));
    printf("Validated : move 1 ball left\n");
    assert(is_move_allowed(6, 6, 6, 7));
    printf("Validated : move 1 ball right\n");
    board[5][0]=1;
    assert(is_move_allowed(5, 0, 6, 0));
    printf("Validated : move 1 ball down\n");

    // impossible movement

    assert(is_move_allowed(6, 1, 7, 4)==0);
    printf("Validated : impossible : move to long in lateral\n");
    assert(is_move_allowed(6, 3, 6, 7)==0);
    printf("Validated : impossible : move to long in line\n");
    assert(is_move_allowed(8, 0, 6, 0)==0);
    printf("Validated : impossible : out of board\n");
    assert(is_move_allowed(0,0,1,0)==0);
    printf("Validated : impossible : not your pawns\n");
    assert(is_move_allowed(6,1,4,1)==0);
    printf("Validated : impossible : nb of balls < wanted distance voulue\n");

    // push 

    board[6][7]=2;
    assert(is_move_allowed(6,4,6,7)==2);
    printf("Validated : player victory by push\n");
    board[5][2]=2;
    board[4][2]=2;
    assert(is_move_allowed(7,2,5,2)==0);
    printf("Validated : opponent player => impossible push\n");

}

void test_is_in_line()
{
    initialize_matrix();

    // en tant que joueur 1

    assert(is_in_line(7, 1, 5, 1));
    printf("Validated : in line for 2 balls\n");
    assert(is_in_line(7, 2, 4, 2));
    printf("Validated : in line for 3 balls\n");
    board[4][1] = 2;
    assert(!is_in_line(6, 1, 4, 0));
    printf("Validated : not in line for 3 balls\n");
    board[5][2] = 0;
    board[6][2] = 0;
    assert(!is_in_line(6, 1, 4, 2));
    printf("Validated : not in line for 2 balls\n");
}

void test_count_balls_behind()
{
    initialize_matrix();
    player = 1;

    // Pour une boule

    assert(count_balls_behind(7, 3, 5, 3) == 0);
    printf("Validated : no ball behind\n");
    board[6][0] = 2;
    assert(count_balls_behind(7, 0, 6, 0) == -1);
    printf("Validated : one ball behind the one\n");
    board[5][0] = 2;
    assert(count_balls_behind(7, 0, 6, 0) == -1);
    printf("Validated : two balls behind the one\n");
    board[4][0] = 2;
    assert(count_balls_behind(7, 0, 6, 0) == -1);
    printf("Validated : three balls behind the one\n");

    // Pour deux boules

    assert(count_balls_behind(7, 1, 5, 1) == 0);
    printf("Validated : no ball behind two\n");
    board[5][1] = 2;
    assert(count_balls_behind(7, 1, 5, 1) == 1);
    printf("Validated : one ball behind two\n");
    board[4][1] = 2;
    assert(count_balls_behind(7, 1, 5, 1) == -1);
    printf("Validated : two balls behind two\n");
    board[3][1] = 2;
    assert(count_balls_behind(7, 1, 5, 1) == -1);
    printf("Validated : three balls behind two\n");

    // Pour trois boules

    board[5][2] = 1;
    assert(count_balls_behind(7, 2, 4, 2) == 0);
    printf("Validated : no balls behind three\n");
    board[4][2] = 2;
    assert(count_balls_behind(7, 2, 4, 2) == 1);
    printf("Validated : one balls behind three\n");
    board[3][2] = 2;
    assert(count_balls_behind(7, 2, 4, 2) == 2);
    printf("Validated : two balls behind three\n");
    board[2][2] = 2;
    assert(count_balls_behind(7, 2, 4, 2) == -1);
    printf("Validated : three balls behind three\n");
}

void test_is_next_cell_outside()
{
    initialize_matrix();

    // Test si la cellule n'est pas en dehors

    board[4][3] = 1;
    assert(is_next_cell_outside(4, 3, -1) == 0);
    printf("Validated : up cell not outside\n");
    assert(is_next_cell_outside(4, 3, 1) == 0);
    printf("Validated : down cell not outside\n");
    assert(is_next_cell_outside(4, 3, 2) == 0);
    printf("Validated : right cell not outside\n");
    assert(is_next_cell_outside(4, 3, -2) == 0);
    printf("Validated : left cell not outside\n");

    // Test si la cellule est en dehors

    assert(is_next_cell_outside(7, 0, 1) == 1);
    printf("Validated : down cell outside\n");
    assert(is_next_cell_outside(7, 0, -2) == 1);
    printf("Validated : left cell outside\n");
    player = 2;
    assert(is_next_cell_outside(0, 7, 2) == 1);
    printf("Validated : right cell outside\n");
    assert(is_next_cell_outside(0, 7, -1) == 1);
    printf("Validated : up cell outside\n");
    player = 1;
}

void test_border()
{
    initialize_matrix();
    
    // trois boules face à une, collée à la barrière : ooox|
    board[1][7] = 1;
    board[2][7] = 1;
    board[3][7] = 1;
    assert(border(3, 7, 0, 7) == 1);
    printf("Validated : border after the opponent ball\n");

    // trois boules face à deux, collée à la barrière : oooxx|
    board[4][7] = 1;
    board[1][7] = 2;
    assert(border(4, 7, 1, 7) == 1);
    printf("Validated : border after the two opponent balls\n");

    initialize_matrix();
    // trois boules face à une,pas collée à la barrière : ooox_|
    board[0][7] = 0;
    board[1][7] = 2;
    board[2][7] = 1;
    board[3][7] = 1;
    board[4][7] = 1;
    assert(border(4, 7, 1, 7) == 0);
    printf("Validated : no border after opponent ball\n");

    // trois boules face à deux, pas collées à la barrière : oooxx_|
    board[0][7] = 0;
    board[1][7] = 2;
    board[2][7] = 2;
    board[3][7] = 1;
    board[4][7] = 1;
    board[5][7] = 1;
    assert(border(5, 7, 2, 7) == 0);
    printf("Validated : no border after the two opponent balls\n");
}

void test_number_of_balls()
{
    // count balls in line
    initialize_matrix();
    player = 1;
    remove_all();
    board[6][1]=1;
    assert(number_of_balls(6,1,5,1) == 1);
    printf("Validated : there is one ball\n");
    board[6][2]=1;
    assert(number_of_balls(6,1,6,3) == 2);
    printf("Validated : there is two balls\n");
    board[6][3]=1;
    assert(number_of_balls(6,1,6,4) == 3);
    printf("Validated : there is three balls\n");
}

void test_count_balls_for_lateral_in_direction()
{
    //particular case for two balls in lateral
    board[6][1]=0;
    assert(count_balls_for_lateral_in_direction(6,2,6,3) == 2);
    printf("Validated : two balls are on a row and goes down\n");
    assert(number_of_balls(6,2,7,2) == 1);
    printf("Validated : no balls on the bottom \n");
    board[6][3]=0;
    board[7][2]=1;
    assert(count_balls_for_lateral_in_direction(6,2,6,3) == 1);
    printf("Validated : no balls on the right\n");
    assert(number_of_balls(6,2,7,2) == 1);
    printf("Validated : two balls are on a column and goes downright\n");
}


void test_is_move_allowed_lateral()
{
    initialize_matrix();
    remove_all();
    board[4][2] = 1;
    board[4][3] = 1;
    board[4][4] = 1;
    assert(is_move_allowed_lateral(4,2,3,4) == 1);
    printf("Validated : lateral move possible\n");
    board[3][3] = 2;
    assert(is_move_allowed_lateral(4,2,3,4) == 0);
    printf("Validated : lateral move impossible\n");

}

void remove_all()
{
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            board[i][j] = 0;
}
#include "../base_game/movement.h"
#include "../global/global_variables.h"
#include "../base_game/game_basics.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

void run_tests_movements();
void test_switch_box();


void run_tests_movements()
{
    test_switch_box();
}

void test_switch_box()
{
    initialize_matrix();
    switch_box(6, 1, 5, 1);
    int tab[8][8];
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            tab[i][j]=board[i][j];   
        }
    }
    initialize_matrix();
    board[6][1]=0;
    board[5][1]=1;
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            assert(tab[i][j]==board[i][j]);
        }
    }
    
    printf("Validated : switch box works\n");
}
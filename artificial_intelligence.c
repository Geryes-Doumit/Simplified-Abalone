/**
 * \file artificial_intelligence.c
 * \author Group 4
 * \brief Artificial intelligence
 * \include "interpret_input.h"
 * \include "global_variables.h"
 * \include "game_basics.h"
 * \include "validity_check.h"
 * \include "movement.h"
 * \include "ihm.h"
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <gtk/gtk.h>
#include <math.h>
#include "interpret_input.h"
#include "global_variables.h"
#include "game_basics.h"
#include "validity_check.h"
#include "movement.h"
#include "ihm.h"

int test_finished(int tab[8][8]);
int packed(int tab[SIZE][SIZE]);
void coups_ia();
int possible_move(int tab[8][8],char possible_tab[300][40]);
int min_max(int board[8][8], int depth, int maximizingPlayer,int alpha, int beta,int current_player, int player_ai);
void copy_tab(int tab[8][8], int tab2[8][8]);
void add_possible_move(int i1, int j1, int i2, int j2,int index,char possible_tab[300][40]);
void convert_coup_tab(char tab[300][40], int size_tab, int board_compare[8][8],int child_board[300][8][8]);
int min(int i, int j);
int max(int i, int j);
int evaluation(int board[8][8], int player_ai);

/**
 * \fn void copy_tab(int tab[8][8], int tab2[8][8])
 * \brief Creates a copy of the board into tab2
 * 
 * \param tab board to copy
 * \param tab2 board copied
 */
void copy_tab(int tab[8][8], int tab2[8][8])
{
	for(int i=0; i<8;i++)
	{
		for(int j=0; j<8; j++)
		{
			tab[i][j] = tab2[i][j];
		}
	}
}

/**
 * \fn void add_possible_move(int i1, int j1, int i2, int j2,int index,char possible_tab[300][40])
 * \brief Adds to possible_tab a move at the position:index
 * 
 * \param i1 first line index
 * \param j1 first column index
 * \param i2 second line index
 * \param j2 second column index
 * \param index index of tab
 * \param possible_tab list of possible tab
 */
void add_possible_move(int i1, int j1, int i2, int j2, int index, char possible_tab[300][40])
{
	char buff[40];
	sprintf(buff, "%d%d:%d%d", i1, j1,i2, j2);
	for(int ind=0; ind<5; ind++)
	{
		possible_tab[index][ind] = buff[ind];
	}
}

/**
 * \fn int possible_move(int tab[8][8], char possible_tab[300][40])
 * \brief find all possible move in tab and put it in possible_tab in the follow format : ??:??
 * 
 * \param tab tab in wich move are tested
 * \param possible_tab list of possible tab
 * \return index of last move possible
 */
int possible_move(int tab[8][8], char possible_tab[300][40])
{
	int index = 0;
	int current_board[8][8];
	copy_tab(current_board,board);
	copy_tab(board,tab);
	for(int i=0;i<SIZE;i++)
	{
		for(int j=0;j<SIZE;j++)
		{
			if(board[i][j] == player )
			{
				if(i<(SIZE-1) && is_move_allowed(i,j,i+1,j))
				{
					add_possible_move(i,j,i+1,j,index,possible_tab);
					index++;
                }
				if(j<(SIZE-1) && is_move_allowed(i,j,i,j+1))
				{
					add_possible_move(i,j,i,j+1,index,possible_tab);
					index++;
				}
				if(j>0 && is_move_allowed(i,j,i,j-1))
				{
					add_possible_move(i,j,i,j-1,index,possible_tab);
					index++;
				}
				if(i>0 && is_move_allowed(i,j,i-1,j))
				{
					add_possible_move(i,j,i-1,j,index,possible_tab);
					index++;
				}
			}

			if(j<(SIZE-1) && board[i][j] == player && board[i][j+1] == player)
			{
				if(j<(SIZE-2) && is_move_allowed(i,j,i,j+2))
				{
					add_possible_move(i,j,i,j+2,index,possible_tab);
					index++;
				}
				if(j>0 && is_move_allowed(i,j+1,i,j-1))
				{
					add_possible_move(i,j+1,i,j-1,index,possible_tab);
					index++;
				}
				if(i<(SIZE-1) && is_move_allowed(i,j,i+1,j+1))
				{
					add_possible_move(i,j,i+1,j+1,index,possible_tab);
					index++;
				}
				if(i>0 && is_move_allowed(i,j,i-1,j+1))
				{
					add_possible_move(i,j,i-1,j+1,index,possible_tab);
					index++;
				}
			}
			if(i<(SIZE-1) && board[i][j] == player && board[i+1][j] == player)
			{
				if(i<(SIZE-2) && is_move_allowed(i,j,i+2,j))
				{
					add_possible_move(i,j,i+2,j,index,possible_tab);
					index++;
				}
				if(i>0 && is_move_allowed(i+1,j,i-1,j))
				{ 
					add_possible_move(i+1,j,i-1,j,index,possible_tab);
					index++;
                }
				if(j<(SIZE-1) && is_move_allowed(i,j,i+1,j+1))
				{
					add_possible_move(i,j,i+1,j+1,index,possible_tab);
					index++;
				}
				if(j>0 && is_move_allowed(i,j,i+1,j-1))
				{
					add_possible_move(i,j,i+1,j-1,index,possible_tab);
					index++;
				}
			}
			
			if(j<(SIZE-2) && board[i][j] == player && board[i][j+1]==player && board[i][j+2] ==player)
			{
				if(j<(SIZE-3) && is_move_allowed(i,j,i,j+3))
				{
					add_possible_move(i,j,i,j+3,index,possible_tab);
					index++;
				}
				if(j>0 && is_move_allowed(i,j+2,i,j-1))
				{
					add_possible_move(i,j+2,i,j-1,index,possible_tab);
					index++;
				}
				if(i<SIZE-1 && is_move_allowed(i,j,i+1,j+2))
				{
					add_possible_move(i,j,i+1,j+2,index,possible_tab);
					index++;
				}
				if(i>0 && is_move_allowed(i,j,i-1,j+2))
				{
					add_possible_move(i,j,i-1,j+2,index,possible_tab);
					index++;
				}
			}
			if(i<(SIZE-2) && board[i][j] == player && board[i+1][j]==player && board[i+2][j] == player)
			{
				if(i<(SIZE-3) && is_move_allowed(i,j,i+3,j))
				{
					add_possible_move(i,j,i+3,j,index,possible_tab);
					index++;
				}
				if(i>0 && is_move_allowed(i+2,j,i-1,j))
				{
					add_possible_move(i+2,j,i-1,j,index,possible_tab);
					index++;
				}
				if(j<(SIZE-1) && is_move_allowed(i,j,i+2,j+1))
				{
					add_possible_move(i,j,i+2,j+1,index,possible_tab);
					index++;
				}
				if(j>0 && is_move_allowed(i,j,i+2,j-1))
				{
					add_possible_move(i,j,i+2,j-1,index,possible_tab);
					index++;
				}
			}	
		}
	}
	copy_tab(board,current_board);
	return(index);
}

/**
 * \fn void convert_coup_tab(char tab[300][40], int size_tab, int tab_to_compare[8][8],int child_board[300][8][8])
 * \brief put in child_board all the child of board_compare from the moves stocked in tab
 * 
 * \param tab 
 * \param size_tab length of tab
 * \param tab_to_compare board tested
 * \param child_board 
 */
void convert_coup_tab(char tab[300][40], int size_tab, int tab_to_compare[8][8],int child_board[300][8][8])
{
	int current_board[8][8];
	int compare[8][8];
	copy_tab(compare,tab_to_compare);
	copy_tab(current_board,board);
	for(int l=0; l<size_tab;l++)
	{
		copy_tab(board,compare);
		shift_piece(tab[l][0]-48,  tab[l][1]-48,  tab[l][3]-48, tab[l][4]-48);
		for(int i=0; i<8;i++)
		{
			for(int j=0; j<8;j++)
			{
				child_board[l][i][j] = board[i][j];
			}
		}
	}
	copy_tab(board,current_board);
}

/**
 * \fn void ai_make_move()
 * \brief chooses a possible move in the state of the game and return it in ai_move
 */
void ai_make_move(){
	int player_ai = player;
	char possible_tab1[300][40];
    int size_tab = possible_move(board,possible_tab1);
	int max_value = -1000;
	int child_board[300][8][8];
	int index_of_max=0;
	convert_coup_tab(possible_tab1, size_tab, board, child_board);
	int min_max_value;
    for(int i=0; i<size_tab;i++)
	{
		min_max_value = min_max(child_board[i], 2, 0, -1000, 1000,player,player_ai);
		player = player_ai;
		if(min_max_value >max_value)
		{
			max_value = min_max_value;
			index_of_max = i;
		}
	}
	for(int j=0; j<5; j++)
	{
		ai_move[j] = possible_tab1[index_of_max][j];
	}
	player = player_ai;
}

/**
 * \fn int min(int i, int j)
 * \brief returns the smallest integer between two
 * 
 * \param i integer to compare
 * \param j integer to compare
 * \return smallest integer between i and j
 */
int min(int i, int j)
{
	if (i<j){return i;}
	else{return j;}
}

/**
 * \fn int min(int i, int j)
 * \brief returns the highest integer between two
 * 
 * \param i integer to compare
 * \param j integer to compare
 * \return highest integer between i and j
 */
int max(int i, int j)
{
	if (i>j){return i;}
	else{return j;}
}

/**
 * \fn int test_finished(int tab[8][8])
 * \brief determines if the game is finished
 * 
 * \param tab Board to test
 * \return 1 game finished
 * \return 0 game not finished
 */
int test_finished(int tab[8][8])
{
	int index = 0;
	for(int i=0; i<8; i++){
		for(int j=0; j<8; j++)
		{
			if(tab[i][j] != 0)
			{
				index++;
			}
		}
	}
	if(index<28)
	{
		return 1;
	}
	return 0;
}

/**
 * \fn int min_max(int tab[8][8], int depth, int maximizingPlayer,int alpha, int beta, int current_player, int player_ai)
 * \brief returns the min-max value of a tab
 * 
 * \param tab 
 * \param depth 
 * \param maximizingPlayer 
 * \param alpha 
 * \param beta 
 * \param current_player 
 * \param player_ai 
 * \return maxComparaison
 * \return minComparaison
 * \return 0
 * \return 1000 wins by IA
 * \return -1000 wins by Oppenant
 */
int min_max(int tab[8][8], int depth, int maximizingPlayer,int alpha, int beta, int current_player, int player_ai){
	int current_depth = depth;
	int focused_player = current_player;
	player = focused_player;
	int tab2[8][8];
	copy_tab(tab2,tab);
	int is_finished= test_finished(tab2);
	int alpha2 = alpha;
	int beta2 = beta;
	if(is_finished ==1 && player == player_ai)
	{
		return 1000;
	}
	if(is_finished == 1 && player != player_ai)
	{
		return -1000;
	}
	if(depth==0)
	{
		return evaluation(tab,player_ai);
	}
	player = focused_player%2+1;
	if( maximizingPlayer == 1)
	{
		int maxComparaison = -1000;
		char possible_tab[300][40];
		int size_tab = possible_move(tab2,possible_tab);
		int child_board[300][8][8];
		convert_coup_tab(possible_tab, size_tab, tab2, child_board);
		for(int i=0; i< size_tab; i++){
			int eval= min_max(child_board[i],current_depth-1,0,alpha2, beta2,player,player_ai);
			player = focused_player%2+1;
			maxComparaison = max(maxComparaison, eval);
			alpha2 = max(alpha2, eval);
			if(beta2<=alpha2)
			{
				break;
			}
		}
		return maxComparaison;
	}
	else if (maximizingPlayer == 0)
	{
		int minComparaison = 1000;
		char possible_tab[300][40];
		int size_tab = possible_move(tab2,possible_tab);
		int child_board[300][8][8];
		convert_coup_tab(possible_tab, 300, tab2, child_board);
		for(int i=0; i< size_tab; i++)
		{
			int eval= min_max(child_board[i], current_depth-1,1, alpha2, beta2,player,player_ai);
			player = focused_player%2+1;
			minComparaison = min(minComparaison, eval);
			beta2 = min(beta2, eval);
			if(beta2<=alpha2)
			{
				break;
			}
		}
		return minComparaison;
	}
	return 0;
}

/**
 * \fn int packed(int tab[8][8])
 * \brief checks if balls are packed
 * 
 * \param tab Board tested
 * \return value of packs
 */
int packed(int tab[8][8])
{
	int index = 0;
	for(int i=0;i<SIZE;i++)
	{
		for(int j=0;j<SIZE;j++)
		{
			if(tab[i][j] == player )
			{
				if(j<(SIZE-1) && tab[i][j] == player && tab[i][j+1] == player)
				{
					index++;
				}
				if(i<(SIZE-1) && tab[i][j] == player && tab[i+1][j] == player)
				{
					index+=5;
				}
				if(j<(SIZE-2) && tab[i][j] == player && tab[i][j+1]==player && tab[i][j+2] ==player)
				{
					index++;
				}
				if(i<(SIZE-2) && tab[i][j] == player && tab[i+1][j]==player && tab[i+2][j] == player)
				{
					index+=10;
				}	
			}
		}
	}
	return(index);
}

/**
 * \fn int aggressif(int tab[8][8], int player_ia)
 * \brief Gives aggressiveness to AI
 * 
 * \param tab Board tested
 * \param player_ia Color of IA
 * \return index of aggressiveness 
 */
int aggressif(int tab[8][8], int player_ia)
{
	int index=0;
	if(player == player_ia)
	{
		for(int i=0;i<8;i++)
		{
			for(int j=0;j<8;j++)
			{
				if(tab[i][j] == player)
				{
					if(player_ia == 2)
					{
						index += i;
					}
					else
					{
						index += 8-i;
					}
				} 
			}
		}
	}
	return index;
}

/**
 * \fn return the evaluation of tab
 * \brief Gives a rating to a board
 * 
 * \param tab Board tested
 * \param player_ai Color of IA
 * \return value as an integer
 */
int evaluation(int tab[8][8], int player_ai)
{
	int value = 0;
	value += aggressif(tab,player_ai);
	value += 5*packed(tab);
	return value;
}

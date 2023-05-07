#ifndef ARTIFICIAL_INTELLIGENCE_H_
#define ARTIFICIAL_INTELLIGENCE_H_

int test_finished(int tab[8][8]);
int packed(int tab[8][8]);
void ai_make_move();
int possible_move(int tab[8][8],char possible_tab[300][40]);
int min_max(int board[8][8], int depth, int maximizingPlayer,int alpha, int beta,int current_player, int player_ai);
void copy_tab(int tab[8][8], int tab2[8][8]);
void add_possible_move(int i1, int j1, int i2, int j2,int index,char possible_tab[300][40]);
void convert_coup_tab(char tab[300][40], int size_tab, int board_compare[8][8],int child_board[300][8][8]);
int min(int i, int j);
int max(int i, int j);
int evaluation(int board[8][8], int player_ai);
int near_bound(int tab[8][8]);

#endif
#ifndef VALIDITY_CHECK_H
#define VALIDITY_CHECK_H

// functions used for validity check
int is_move_allowed(int i1, int j1, int i2, int j2);
int is_move_allowed_lateral(int i1, int j1, int i2, int j2);
int get_direction(int i1, int j1, int i2, int j2);
int is_in_line(int i1, int j1, int i2, int j2);
int count_balls_for_lateral_in_direction(int i1, int j1, int i2, int j2);
int number_of_balls(int i1, int j1, int i2, int j2);
int count_balls_behind(int i1, int j1, int i2, int j2);
int border(int i1, int i2, int j1, int j2);
char is_next_cell_outside(int i, int j, int direction);

#endif
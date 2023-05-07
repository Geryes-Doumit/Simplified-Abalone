#include "../base_game/interpret_input.h"
#include "../global/global_variables.h"
#include "../base_game/game_basics.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void run_tests_interpret_input();
void test_ai_to_string_input();
void test_interpret_input();


void run_tests_interpret_input()
{
    test_interpret_input();
    test_ai_to_string_input();
    
}

void test_interpret_input()
{
    char* s = "H1:G1";
    int buffer[4];
    input_as_array(s, buffer);
    assert(buffer[0]==7 && buffer[1]==0 && buffer[2]==6 && buffer[3]==0);
    printf("Validated : Input as array\n");

}

void test_ai_to_string_input()
{
    char* s= "70:60";
    char output[6];
    ai_to_string_input(s,output);
    assert(strcmp(output,"H1:G1\0") == 0);
    printf("Validated : AI to string\n");
}

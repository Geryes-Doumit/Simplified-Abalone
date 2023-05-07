/**
 * \file interpret_input.c
 * \author Group 4
 * \brief Converts input to usable date for other functions.
 * \details test.
 * \include "interpret_input.h"
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../base_game/interpret_input.h"

/**
 * \fn int *input_as_array(char *s, int input_array[], int length)
 * \brief Converts a string input into four board coordinates.
 * \details Converts "A-H" to integer between 0 and 7 using ASCII code.
 * \details Converts "1-8" to integer between 0 and 7 using ASCII code.
 * 
 * \param s Atring to convert, syntax accepted: "A1:C2".
 * \param out Array that will contain the output; out[n] contains the nth coordinate.
 * \return Array described beforehand.
 */
int *input_as_array(char *s, int out[])
{
    int i1, j1, i2, j2;

    if (s[2] != ':')
    {
        return NULL;
    }
    else
    {
        i1 = s[0] - 'A';
        i2 = s[3] - 'A';

        j1 = s[1] - '0' - 1;
        j2 = s[4] - '0' - 1;

        out[0] = i1;
        out[1] = j1;
        out[2] = i2;
        out[3] = j2;

        return out;
    }
}

/**
 * \fn char *ai_to_string_input(char *s, char output_string[], int length)
 * \brief Converts AI input into the accepted string format.
 * \details Checks syntax
 * \details Converts "A-H" to integer between 0 and 7 using ASCII code
 * \details Converts "1-8" to integer between 0 and 7 using ASCII code
 * 
 * \param s input to convert
 * \param out char* that will contain the correct string format.
 * \return The char* described beforehand (or NULL for an invalid syntax).
 */
char *ai_to_string_input(char *s, char out[])
{
    char l1, l2;
    int i1, i2;

    if (s[2] != ':')
    {
        printf("invalid syntax (ai_to_string_input())\n");
        return NULL;
    }
    else
    {
        l1 = ((int)s[0]) + 'A' - '0';
        l2 = ((int)s[3]) + 'A' - '0';
        
        i1 = ((int)s[1]) + 1;
        i2 = ((int)s[4]) + 1;
        
        out[0] = l1;
        out[1] = i1;
        out[2] = ':';
        out[3] = l2;
        out[4] = i2;
        out[5] = '\0';

        return out;
    }
}
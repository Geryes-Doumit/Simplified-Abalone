#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <netdb.h>
#include <gtk/gtk.h>
#include "interpret_input.h"
#include "global_variables.h"
#include "game_basics.h"
#include "validity_check.h"
#include "movement.h"
#include "ihm.h"
#include "artificial_intelligence.h"
#include "network.h"
#include "tests_validity_check.h"
#include "test_movement.h"
#include "test_interpret_input.h"

int main(int argc, char **argv)
{

    if (argc == 1)
    {
        printf("Too few arguments:\n");
        printf("\t-c <IP_Address:Port> to be the client\n");
        printf("\t-s <Port> to be the server\n");
        printf("\t-pvp to play against another player (black starts)\n");
        printf("\t-l <blanc/noir> to play against the computer (local)\n");
        printf("\t-tests to run the test functions \n");

        return 1;
    }

    if (argc > 1)
    {
        if ((strcmp(argv[1], "-pvp") == 0))
        {
            init(argc, argv);
        }
        else if (strcmp(argv[1], "-tests") == 0)
        {
            printf("Tests of evaluation functions :\n");
            run_tests_validity_check();
            printf("Tests of movements functions :\n");
            run_tests_movements();
            printf("Tests of interpret functions :\n");
            run_tests_interpret_input();
        }
        else if (argc > 2) 
        {
            if (strcmp(argv[1], "-s") == 0 || strcmp(argv[1], "-c") == 0)
            {
                setup_comm(strcmp(argv[1], "-s") == 0, argv[2]);
            }
            if ( (strcmp(argv[1], "-l") == 0 && strcmp(argv[2], "blanc") == 0) || (strcmp(argv[1], "-l") == 0 && strcmp(argv[2], "noir") == 0))
            {
                /* if ( strcmp(argv[2], "blanc") == 0 )
                {
                    player = 2;
                } */
                init(argc, argv);
            }
        }
        else
        {
            printf("Too few arguments:\n");
            printf("\t-c <IP_Address:Port> to be the client\n");
            printf("\t-s <Port> to be the server\n");
            printf("\t-pvp to play against another player (black starts)\n");
            printf("\t-l <blanc/noir> to play against the computer (local)\n");
            printf("\t-tests to run the test functions \n");

            return 1;
        }

    }

    return 0;
}
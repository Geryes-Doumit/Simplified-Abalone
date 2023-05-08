/**
 * \file ihm.c
 * \author Group 4
 * \brief Graphic display
 * \details show the GUI for the game
 * \include "ihm.h"
 * \include "interpret_input.h"
 * \include "global_variables.h"
 * \include "game_basics.h"
 * \include "validity_check.h"
 * \include "movement.h"
 * \include "artificial_intelligence.h"
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../gui/ihm.h"
#include "../base_game/interpret_input.h"
#include "../global/global_variables.h"
#include "../base_game/game_basics.h"
#include "../base_game/validity_check.h"
#include "../base_game/movement.h"
#include "../base_game/artificial_intelligence.h"

// Constants used in GTK functions
#define WSIZE 600

// Variables used in GTK functions
char buffer[5],last_pos[5];
const gchar *text;
static GtkWidget *window, *grid, *layout, *image, *darea, *vBox2, *label2, *label3, *label4, *vBox3, *vBox4;
GtkWidget *entry;
int next_turn = 0;

/**
 * \fn draw_arc_black(cairo_t *cr)
 * \brief Display a black circle
 * \param cr Cairo object
 */

static void draw_arc_black(cairo_t *cr)
{
    cairo_arc(cr, 50, 50, 20, 0, 360);
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0); /* set fill color */
    cairo_fill(cr);                          /* fill arc */
}

/**
 * \fn draw_arc_white(cairo_t *cr)
 * \brief Display a black circle
 * \param cr Cairo object
 */

static void draw_arc_white(cairo_t *cr)
{
    cairo_arc(cr, 50, 50, 20, 0, 360);
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); /* set fill color */
    cairo_fill(cr);                          /* fill arc */
}

/**
 * \fn draw_arc_white(cairo_t *cr)
 * \brief Display a transparent circle
 * \param cr Cairo object
 */

static void draw_arc_none(cairo_t *cr)
{
    cairo_arc(cr, 0, 0, 0, 0, 0);
    cairo_fill(cr); /* fill arc */
}

/**
 * \fn on_draw_event_*(GtkWidget *widget, cairo_t *cr, gpointer user_data)
 * \brief Event handler which calls fonctions to draw circles
 * \param cr Cairo object
 * \param widget GTK widget
 * \param user_data
 */

static gboolean on_draw_event_1(GtkWidget *widget, cairo_t *cr,
                                gpointer user_data)
{

    draw_arc_white(cr); /* draw circle white in window */

    return FALSE;
    (void)user_data, (void)widget; /* suppress -Wunused warning */
}

static gboolean on_draw_event_2(GtkWidget *widget, cairo_t *cr,
                                gpointer user_data)
{

    draw_arc_black(cr); /* draw circle black in window */
    return FALSE;
    (void)user_data, (void)widget; /* suppress -Wunused warning */
}

static gboolean on_draw_event_3(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
    draw_arc_none(cr); /* draw circle NONE in window */
    return FALSE;
    (void)user_data, (void)widget; /* suppress -Wunused warning */
}

/**
 * \fn invalid_move_dialog(void)
 * \brief Display a message if the move entered isn't valid
 */
static void invalid_move_dialog(void)
{
    // This creates (but does not yet display) a message dialog with
    // the given text as the title.
    GtkWidget* hello = gtk_message_dialog_new(
        NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
        "Mouvement invalide !");

    // The (optional) secondary text shows up in the "body" of the
    // dialog. Note that printf-style formatting is available.
    gtk_message_dialog_format_secondary_text(
        GTK_MESSAGE_DIALOG(hello),
        "Assurez d'entrer un mouvement qui est permis,\net ayant la bonne syntaxe.\nExemple de syntaxe correcte: A1:B1");

    // This displays our message dialog as a modal dialog, waiting for
    // the user to click a button before moving on. The return value
    // comes from the :response signal emitted by the dialog. By
    // default, the dialog only has an OK button, so we'll get a
    // GTK_RESPONSE_OK if the user clicked the button. But if the user
    // destroys the window, we'll get a GTK_RESPONSE_DELETE_EVENT.
    int response = gtk_dialog_run(GTK_DIALOG(hello));

    printf("response was %d (OK=%d, DELETE_EVENT=%d)\n",
           response, GTK_RESPONSE_OK, GTK_RESPONSE_DELETE_EVENT);

    // If we don't destroy the dialog here, it will still be displayed
    // (in back) when the second dialog below is run.
    gtk_widget_destroy(hello);
}

/**
 * \fn grid_display(GtkWidget *grid)
 * \brief Refresh the grid display with the board
 * \param grid GTK widget grid
 */

void grid_display(GtkWidget *grid)
{


    /*Adding widget to grid*/
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (board[i][j] != tmp_board[i][j])
            {
                darea = gtk_drawing_area_new();
                GtkWidget *child = GTK_WIDGET(gtk_grid_get_child_at(GTK_GRID(grid), j + 1, i + 1));
                gtk_container_remove(GTK_CONTAINER(grid), child);
                gtk_widget_set_size_request(darea, WSIZE / 8, WSIZE / 8);
                /*white pawns*/
                if (board[i][j] == 2)
                {
                    g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event_1), NULL);
                }
                /*black pawns*/
                if (board[i][j] == 1)
                {
                    g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event_2), NULL);
                }
                /*no pawns*/
                else
                {
                    g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event_3), NULL);
                }
                gtk_grid_attach(GTK_GRID(grid), darea, j + 1, i + 1, 1, 1);
            }
        }
    }

    /*Display the player color*/
    vBox2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_container_add(GTK_CONTAINER(window), vBox2);
    gchar *str2;
    if (player == 1)
    {
        str2 = g_locale_to_utf8(" <b> <span face=\"Bookman\"> <span foreground=\"#FFFFFF\"> Joueur </span> noir </span> </b>",
                                -1, NULL, NULL, NULL);
    }
    else
    {
        str2 = g_locale_to_utf8(" <span face=\"Bookman\" foreground=\"#FFFFFF\"><b> Joueur blanc </b></span>",
                                -1, NULL, NULL, NULL);
    }
    gtk_label_set_markup(GTK_LABEL(label2), str2);
    gtk_grid_attach(GTK_GRID(grid), vBox2, 0, 12, 5, 3);
    g_free(str2);
    
    /*Display the last position*/

    vBox4 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    gtk_container_add(GTK_CONTAINER(window), vBox4);

    gchar* str4 = g_strconcat("<b> <span face=\"Bookman\"> <span foreground=\"#FFFFFF\"> ",last_pos," </span></span> </b>",NULL);

    gtk_label_set_markup(GTK_LABEL(label4),str4);

    gtk_grid_attach(GTK_GRID(grid), vBox4, 5, 12, 5, 3);

    g_free(str4);

    /*Update widgets*/
    gtk_widget_show_all(grid);
}

/**
 * \fn grid_init(GtkWidget *grid)
 * \brief Initialize the grid display with the board
 * \param grid GTK widget grid
 */

void grid_init(GtkWidget *grid)
{

    /*decalage pour l'affichage des chiffres et lettres*/
    for (int i = 0; i < SIZE + 1; i++)
    {
        darea = gtk_drawing_area_new();
        gtk_widget_set_size_request(darea, WSIZE / 8, WSIZE / 8);
        g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event_3), NULL);
        gtk_grid_attach(GTK_GRID(grid), darea, 0, i, 1, 1);
    }
    for (int j = 1; j < SIZE + 1; j++)
    {
        darea = gtk_drawing_area_new();
        gtk_widget_set_size_request(darea, WSIZE / 8, WSIZE / 8);
        g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event_3), NULL);
        gtk_grid_attach(GTK_GRID(grid), darea, j, 0, 1, 1);
    }

    /*Adding widget to grid*/
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            darea = gtk_drawing_area_new();
            gtk_widget_set_size_request(darea, WSIZE / 8, WSIZE / 8);
            /*white pawns*/
            if (board[i][j] == 2)
            {
                g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event_1), NULL);
            }
            /*black pawns*/
            if (board[i][j] == 1)
            {
                g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event_2), NULL);
            }
            /*no pawns*/
            else
            {
                g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event_3), NULL);
            }
            gtk_grid_attach(GTK_GRID(grid), darea, j + 1, i + 1, 1, 1);
        }
    }

    /*Init display player*/
    vBox2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vBox2);
    label2 = gtk_label_new(NULL);
    gchar *str2 = g_locale_to_utf8(" <b> <span face=\"Bookman\"> <span foreground=\"#FFFFFF\">Joueur</span> noir </span> </b>", -1, NULL, NULL, NULL);
    gtk_label_set_markup(GTK_LABEL(label2), str2);
    gtk_box_pack_start(GTK_BOX(vBox2), label2, FALSE, TRUE, 1);
    gtk_grid_attach(GTK_GRID(grid), vBox2, 0, 12, 5, 3);
    g_free(str2);
    
    /*Display the last position*/
    vBox3 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    vBox4 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vBox3);
    gtk_container_add(GTK_CONTAINER(window), vBox4);
    label3 = gtk_label_new(NULL);
    label4 = gtk_label_new(NULL);

    gchar *str3 = g_locale_to_utf8(" <b> <span face=\"Bookman\"> <span foreground=\"#FFFFFF\"> Dernier mouvement :  </span></span> </b>", -1, NULL, NULL, NULL);
    gchar* str4 = g_strconcat("<b> <span face=\"Bookman\"> <span foreground=\"#FFFFFF\"> ",last_pos," </span></span> </b>",NULL);
    gtk_label_set_markup(GTK_LABEL(label3),str3);
    gtk_label_set_markup(GTK_LABEL(label4),str4);
    gtk_box_pack_start(GTK_BOX(vBox3), label3, FALSE, TRUE, 1);
    gtk_box_pack_start(GTK_BOX(vBox4), label4, FALSE, TRUE, 1);
    gtk_grid_attach(GTK_GRID(grid), vBox3, 3, 12, 5, 2);
    gtk_grid_attach(GTK_GRID(grid), vBox4, 5, 12, 5, 3);
    g_free(str3);
    g_free(str4);

    gtk_widget_show_all(grid);
}

/**
 * \fn final_window()
 * \brief Display the final window when the game is finished
 */

void final_window()
{
    // gtk_window_close (GTK_WINDOW(window));
    GtkWidget *window2, *image2;
    /* Creation of main window */
    window2 = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    /* Center window on screen */
    gtk_window_set_position(GTK_WINDOW(window2), GTK_WIN_POS_CENTER);

    /* Default window size */
    gtk_window_set_default_size(GTK_WINDOW(window2), 1200, 675);

    /* Set window title */
    gtk_window_set_title(GTK_WINDOW(window2), "GAME OVER");
    g_signal_connect(window2, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_handlers_disconnect_by_func(G_OBJECT(entry), on_activate_entry_pvp, NULL);
    g_signal_handlers_disconnect_by_func(G_OBJECT(entry), on_activate_entry_ai, NULL);
    /*Background image final*/
    layout = gtk_layout_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(window2), layout);
    if (player == 1)
    {
        image2 = gtk_image_new_from_file("src/gui/black_win_screen.png");
    }
    else
    {
        image2 = gtk_image_new_from_file("src/gui/white_win_screen.png");
    }

    gtk_layout_put(GTK_LAYOUT(layout), image2, 0, 0);

    gtk_widget_show_all(window2);
    /* Start main event loop */
    gtk_main();
}

/**
 * \fn on_activate_entry_ai(gpointer data)
 * \brief Event handler, when entry is used in ai mode, run the main code
 * \param data
 */

void on_activate_entry_ai(gpointer data)
{
    next_turn = 0;
    int i = 0;

    /* Recuperation du texte contenu dans le GtkEntry */
    text = gtk_entry_get_text(GTK_ENTRY(entry));

    

    int i1, j1, i2, j2;
    
    for (i = 0; i < 5; i++)
    {
        buffer[i] = toupper(text[i]);
        last_pos[i]=buffer[i];
    }

    gtk_entry_set_text(GTK_ENTRY(entry), "");
    text = "";

    int input[4] = {0};

    if (!finished)
    {
        printf("Joueur %i :\n", player);
        printf(" %s :\n", buffer);

        input_as_array(buffer, input);

        i1 = input[0];
        j1 = input[1];
        i2 = input[2];
        j2 = input[3];
        if (!is_move_allowed(i1, j1, i2, j2))
        {
            // finished = 1;
            // //printf("Move not allowed");
            // player = player % 2 + 1;
            // final_window();
            invalid_move_dialog();
        }
        else if (is_move_allowed(i1, j1, i2, j2) == 2)
        {
            shift_piece(i1, j1, i2, j2);
            print_board();
            finished = 1;
            grid_display(grid);
            gtk_widget_show(grid);
            final_window();
        }
        else
        {
            shift_piece(i1, j1, i2, j2);
            print_board();
            next_turn = 1;
            player = player % 2 + 1;
        }

        grid_display(grid);
        gtk_widget_show(grid);
    }

    g_timeout_add(10, ia_turn, NULL);

}

/**
 * \fn on_activate_entry_ai(gpointer data)
 * \brief Event handler, when entry is used in pvp mode, run the main code
 * \param data
 */

void on_activate_entry_pvp(gpointer data)
{
    int i = 0;

    /* Recuperation du texte contenu dans le GtkEntry */
    text = gtk_entry_get_text(GTK_ENTRY(entry));



    int i1, j1, i2, j2;

    for (i = 0; i < 5; i++)
    {
        buffer[i] = toupper(text[i]);
        last_pos[i]=buffer[i];
    }

    gtk_entry_set_text(GTK_ENTRY(entry), "");
    text = "";

    int input[4] = {0};

    if (!finished)
    {
        printf("Joueur %i :\n", player);
        printf(" %s :\n", buffer);

        input_as_array(buffer, input);

        i1 = input[0];
        j1 = input[1];
        i2 = input[2];
        j2 = input[3];
        if (!is_move_allowed(i1, j1, i2, j2))
        {
            // finished = 1;
            // //printf("Move notallowed");
            // player = player % 2 + 1;
            // final_window();
            invalid_move_dialog();
        }
        else if (is_move_allowed(i1, j1, i2, j2) == 2)
        {

            shift_piece(i1, j1, i2, j2);
            print_board();
            finished = 1;
            grid_display(grid);
            gtk_widget_show(grid);
            final_window();
        }
        else
        {
            shift_piece(i1, j1, i2, j2);
            print_board();
            player = player % 2 + 1;
        }

        grid_display(grid);
        gtk_widget_queue_draw(grid);
    }
}

/**
 * \fn ia_turn()
 * \brief Run the main code but for the ia, used in on_activate_entry_ai
 */

void ia_turn()
{
    if (next_turn == 1) {
        ai_make_move();
    }
    next_turn = 0;
    // recupere le coup*
    for (int i = 0; i < 5; i++)
    {
        buffer[i] = ai_move[i];
        ai_to_string_input(buffer,last_pos);
    }

    grid_display(grid);
    if (!finished)
    {
        printf("Joueur %i :\n", player);
        printf(" %s :\n", buffer);

        int i1, j1, i2, j2;
        i1 = buffer[0];
        j1 = buffer[1];
        i2 = buffer[3];
        j2 = buffer[4];

        i1 -= 48;
        j1 -= 48;
        i2 -= 48;
        j2 -= 48;

        if (!is_move_allowed(i1, j1, i2, j2))
        {
            // finished = 1;
            // //printf("Move not allowed");
            // player = player % 2 + 1;
            // final_window();
        }
        else if (is_move_allowed(i1, j1, i2, j2) == 2)
        {
            shift_piece(i1, j1, i2, j2);
            print_board();
            finished = 1;
            grid_display(grid);
            gtk_widget_show(grid);
            final_window();
        }
        else
        {
            next_turn = 1;
            shift_piece(i1, j1, i2, j2);
            print_board();
            player = player % 2 + 1;
        }
        grid_display(grid); 
        gtk_widget_queue_draw(grid);
    }
}

/**
 * \fn init(int argc, char **argv)
 * \brief Initialize all the GTK widgets
 * \param argc
 * \param argv
 */

void init(int argc, char **argv)
{
    initialize_matrix(); // Fonction qui initialise le board, met des 0 partout, puis des 1 pour les pions noirs et 2 pour les pions blancs (configuration initiale)
    int i1, j1, i2, j2;

    GtkWidget *message, *label1, *vBox1, *vBox;
    GdkColor color = {0, 0x28F5, 0xAB86, 0x851E};

    /*Init widget and gtk*/
    gtk_init(&argc, &argv);

    /* Creation of main window */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    /* Center window on screen */
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    /* Default window size */
    gtk_window_set_default_size(GTK_WINDOW(window), WSIZE + 75, WSIZE + 175);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_modify_bg(window, GTK_STATE_NORMAL, &color); // 145,82,61

    /*Background image*/
    layout = gtk_layout_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(window), layout);
    gtk_widget_show(layout);
    image = gtk_image_new_from_file("src/gui/fond.png");
    gtk_layout_put(GTK_LAYOUT(layout), image, 0, 0);

    /*Grid widget*/
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);
    gtk_layout_put(GTK_LAYOUT(layout), grid, -10, -10);

    /* Creation of a vertical box */
    vBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    /* Insertion of the box in the window */
    gtk_container_add(GTK_CONTAINER(window), vBox);
    /* Creation of a text zone */
    entry = gtk_entry_new();


    /* Connection to the signal named "activate" */
    if (argc > 1 && !strcmp("-l", argv[1]))
    {
        g_signal_connect(G_OBJECT(entry), "activate", G_CALLBACK(on_activate_entry_ai), NULL);
    }
    else
    {
        g_signal_connect(G_OBJECT(entry), "activate", G_CALLBACK(on_activate_entry_pvp), NULL);
    }

    /* Insertion in vBox */
    gtk_box_pack_start(GTK_BOX(vBox), entry, FALSE, TRUE, 0);

    /* Creation of a vertical box */
    vBox1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    /* Insertion of the box in the window */
    gtk_container_add(GTK_CONTAINER(window), vBox1);
    /* Creation of the label */
    label1 = gtk_label_new(NULL);
    gchar *str1 = g_locale_to_utf8(" <span face=\"Bookman\"> <span foreground=\"#FFFFFF\"><b>Entrez les coordonnées :        </b> </span> </span>", -1, NULL, NULL, NULL);
    gtk_label_set_markup(GTK_LABEL(label1), str1);

    /* Insertion in vBox */
    gtk_box_pack_start(GTK_BOX(vBox1), label1, TRUE, TRUE, 0);

    GtkWidget *space = gtk_label_new("  ");
    /*Pawns table call*/
    grid_init(grid);
    gtk_widget_set_size_request(space, 80, 60);
    gtk_grid_attach(GTK_GRID(grid), space, 0, 9, 7, 5);
    gtk_grid_attach(GTK_GRID(grid), vBox1, 0, 15, 5, 1);
    gtk_grid_attach(GTK_GRID(grid), vBox, 4, 15, 3, 1); // Centrer le txt
    
    if(argc>2 && !strcmp("blanc", argv[2]))
    {
        g_print("AI is making a move, please wait...");
        next_turn = 1;
        ia_turn();
    }

    /*Display widget*/
    gtk_widget_show_all(window);
    gtk_main();
}

/**
 * \fn init_ai_vs_ai(int argc, char **argv)
 * \brief Initialize all the GTK widgets in ai vs ai mode
 * \param argc
 * \param argv
 */

void init_ai_vs_ai(int argc, char **argv)
{
    GtkWidget *message;
    GdkColor color = {0, 0x28F5, 0xAB86, 0x851E};

    /*Init widget and gtk*/
    gtk_init(&argc, &argv);

    /* Creation of main window */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    /* Center window on screen */
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    /* Default window size */
    gtk_window_set_default_size(GTK_WINDOW(window), WSIZE + 75, WSIZE + 175);
    
    /*Background color*/
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_modify_bg(window, GTK_STATE_NORMAL, &color); 

    /*Background image*/
    layout = gtk_layout_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(window), layout);
    gtk_widget_show(layout);
    image = gtk_image_new_from_file("src/gui/fond.png");
    gtk_layout_put(GTK_LAYOUT(layout), image, 0, 0);

    /*Grid widget*/
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);
    gtk_layout_put(GTK_LAYOUT(layout), grid, -10, -10);

    GtkWidget *space = gtk_label_new("  ");
    /*Pawns table call*/
    grid_init(grid);
    gtk_widget_set_size_request(space, 80, 60);
    gtk_grid_attach(GTK_GRID(grid), space, 0, 9, 7, 5);

    /*Display widget*/
    gtk_widget_show_all(window);
    gtk_main();
}

void refresh_display()
{
    grid_display(grid);
}
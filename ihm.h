#ifndef IHM_H_
#define IHM_H_


// Constants used in GTK functions
#define WSIZE 600
#include <gtk/gtk.h>

// Variables used in GTK functions
extern char buffer[5],last_pos[5];
extern const gchar *text;
static GtkWidget *window, *grid, *layout, *image, *darea, *vBox2, *label2, *label3, *label4, *vBox3, *vBox4;

// GTK functions
static void draw_arc_black(cairo_t *cr);
static void draw_arc_white(cairo_t *cr);
static void draw_arc_none(cairo_t *cr);
static gboolean on_draw_event_1(GtkWidget *widget, cairo_t *cr, gpointer user_data);
static gboolean on_draw_event_2(GtkWidget *widget, cairo_t *cr, gpointer user_data);
static gboolean on_draw_event_3(GtkWidget *widget, cairo_t *cr, gpointer user_data);
void on_activate_entry_ai(gpointer data);
void init(int argc, char **argv);
void on_activate_entry_pvp(gpointer data);
void grid_init(GtkWidget *grid);
void grid_display(GtkWidget *grid);
void final_window();
void init_ai_vs_ai(int argc, char **argv);
void refresh_display();
void ia_turn();

#endif

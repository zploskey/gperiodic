/*
 
gperiodic - a program for browsing the periodic table and looking up
            information on elements.

Copyright (C) 2000 Jonas Frantz
 
    This file is part of gperiodic.
 
    gdpc is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
 
    gdpc is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
 
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 
Authors email : jonas.frantz@helsinki.fi
 
*/

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

  gsize bytes_read, bytes_written;

#include "gperiodic.h"
#include "table_data.h"

#define VERSION "2.0.10"

#define PACKAGE "gperiodic"
#define GPERIODIC_LOCALE_DIR "/usr/share/locale"

#define HEADERTEXTLEN 40

  char *optstr            = "c:hDv";		/* cmdline option string... */
  int  enable_colorscale  = 0;
  int  debug = 0;

  GtkWidget *window;

int gpdata_get_atomic_number_for_symbol(const char*sym)
{
  int i;

    for(i = 0; i < ELEMENTNUM; ++i) {
	if(!strcasecmp(table[i].info[SYMBOL],sym)) {
	    return (i+1);
	}
    }
  return -1;
}  


void gpparse_print_element_data_for_num(int num)
{
    int row, n, i;
    char fillout[HEADERTEXTLEN];

    printf("\n");
    for (row = 0; row < MAX_INFO_NR; row++) {
	n = strlen(header.info[row]);
	n = HEADERTEXTLEN - n;
	for (i=0;i<HEADERTEXTLEN;i++) fillout[i] = ' ';
	fillout[n] = (char) NULL;
	printf(" %s%s%s\n", header.info[row], fillout,  table[num-1].info[row]);
    }
    printf("\n");
}


void main_window_exit( GtkWidget *w, gpointer d ) 
{
    gtk_main_quit();
}


void dialog_destroy( GtkWidget *w, gpointer d ) 
{
    gtk_grab_remove(GTK_WIDGET(w));
}


void dialog_close( GtkWidget *w, gpointer d ) 
{
    gtk_widget_destroy(GTK_WIDGET(d));
}


void display_element_dialog( GtkWidget *w, gpointer d ) 
{
  struct table_entry *entry = (struct table_entry *)d;
  GtkWidget *dialog, *vbox, *tbl, *ok_button, *hbox_space, *hbox_ok, *sound_button, *hbox_sound;
  int col, x1, x2, y1, y2;
  char *buff, tmpbuf[128];

    _DBG(g_message("%s(%d) display_table_element() w=0x%08x d=0x%08x",__FILE__, __LINE__, w, d ) );

/* display this in a dialog box... */
    dialog = gtk_dialog_new();
    g_signal_connect(GTK_OBJECT(dialog),"destroy", G_CALLBACK(dialog_destroy), dialog );
   
    gtk_window_set_title(GTK_WINDOW(dialog), _(entry->info[NAME]));

    gtk_container_set_border_width( GTK_CONTAINER(dialog), 5 );

    gtk_window_set_resizable(GTK_WINDOW(dialog),0);

    vbox = gtk_vbox_new(FALSE,0);

    gtk_box_pack_start(GTK_BOX( GTK_DIALOG(dialog)->action_area),
			vbox, TRUE, TRUE, 0 );

    tbl = gtk_table_new(MAX_INFO_NR, 2,FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), tbl, FALSE, FALSE, 0 );

/* add the labels to the dialog box verticaly... */
    for (col = 0; col < 2; col++) {
	int row;

	for (row = 0; row < MAX_INFO_NR; row++) {
	    GtkWidget *label;

            label = gtk_label_new(NULL);
	    if (col == 0) gtk_label_set_markup (GTK_LABEL (label), _(header.info[row]));
            else gtk_label_set_markup (GTK_LABEL (label), _(entry->info[row]));

	    gtk_misc_set_alignment(GTK_MISC(label), 0, 0);
	    gtk_table_attach_defaults(GTK_TABLE(tbl), label, col, col + 1, row, row + 1);
	}
    }


    ok_button = gtk_button_new_from_stock (GTK_STOCK_OK);
    g_signal_connect( GTK_OBJECT(ok_button), "clicked",
          G_CALLBACK(dialog_close), dialog );

    hbox_space = gtk_hbox_new(FALSE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(hbox_space),5);
    gtk_box_pack_start( GTK_BOX(vbox), hbox_space, TRUE, TRUE, 0 );

    hbox_ok = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_ok), ok_button, TRUE, TRUE, 0 );
    gtk_box_pack_end(GTK_BOX(vbox), hbox_ok, TRUE, TRUE, 0 );

    gtk_grab_add(dialog);
    gtk_widget_show_all(dialog);
}


GCallback menu_file_exit(void)
{
    main_window_exit(NULL,NULL);
}


GCallback menu_help_about(void)
{
  gchar *authors[] = AUTHORS;

    gtk_show_about_dialog((GtkWindow *) window, 
	"authors", authors, 
	"comments", COMMENTS,
	"copyright", COPYRIGHT,
	"license", LICENSE,
	"name", PROGNAME,
	"version", VERSION,
	"website", HOMEPAGEURL,
	"website-label", HOMEPAGELABEL,
	NULL);

  return NULL;
}


void show_usage( void ) 
{
    printf(N_("GPeriodic [opts]\n"
	   "Options are:\n"
	   "    -h         show this help screen.\n"
	   "    -c <Temp>  color the elements according to their phase at\n"
	   "               temperature <Temp> in Kelvins.\n"
	   "    -D         dump the periodic table.\n"
	   "    -v         be verbose\n\n"));
}


int criterion2scale(double r, int maxcolor) 
{
  int result;

    if (r < 0) return 0;
    result = 1 + (maxcolor-1) * r;
    if (result >= maxcolor) result = maxcolor - 1;
    return result;
}

void colorize_element(
  int i,
  struct colorscale * cs1, criterion c1,
  struct colorscale * cs2, criterion c2,
  struct colorscale * cs3, criterion c3)
{
  int n, c;
  double r;
  GdkColor col, col_;

  c = cs1->maxcolor;
  r = c1(table+i);
  n = criterion2scale(r,c);
  col = cs1->scale[n];

  if(cs2) {
    c = cs2->maxcolor;
    r = c2(table+i);
    n = criterion2scale(r,c);
    col_ = cs2->scale[n];
    col.red += col_.red;
    col.green += col_.green;
    col.blue += col_.blue;
    if(cs3) {
      c = cs3->maxcolor;
      r = c3(table+i);
      n = criterion2scale(r,c);
      col_ = cs3->scale[n];
      col.red += col_.red;
      col.green += col_.green;
      col.blue += col_.blue;
    }
  }

  table[i].style->bg[0].red = col.red;
  table[i].style->bg[0].green = col.green;
  table[i].style->bg[0].blue = col.blue;

  gtk_widget_set_style( table[i].button, table[i].style );
}

void main_prog(void)
{
  GtkWidget *vbox;
  GtkWidget *periodic_table;
  int menu_num_items;
  GdkColor tooltip_color_bg, tooltip_color_fg;
  int red, gree, blue;
  char *buff;
  int i;

  GtkWidget *menubar;
  GtkActionGroup *action_group;
  GtkUIManager *ui_manager;
  GtkAccelGroup *accel_group;
  GError *error;

  GtkActionEntry entries[] = {
	{ "FileMenu", NULL, _("_File") },
	{ "HelpMenu", NULL, _("_Help") },
	{ "Exit", GTK_STOCK_OPEN, _("E_xit"), "<control>Q", _("Exit the program"), (GCallback) menu_file_exit },
	{ "About", GTK_STOCK_HELP, _("_About"), "<control>H", _("About gperiodic"), (GCallback) menu_help_about },
  };

  char *ui_description =
	"<ui>"
	"  <menubar name='MainMenu'>"
	"    <menu action='FileMenu'>"
	"      <menuitem action='Exit'/>"
	"    </menu>"
	"    <menu action='HelpMenu'>"
	"      <menuitem action='About'/>"
	"    </menu>"
	"  </menubar>"
	"</ui>";

/* create the main window */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),_("GPeriodic"));
    gtk_container_set_border_width(GTK_CONTAINER(window),0);
    gtk_window_set_resizable(GTK_WINDOW(window),0);
    
/* connect the close event... */
    g_signal_connect(GTK_OBJECT(window), "delete-event", G_CALLBACK(main_window_exit), &window );
    g_signal_connect(GTK_OBJECT(window), "destroy", G_CALLBACK(main_window_exit), &window );

/* use a vbox for the menubar and the table of elements... */
    vbox = gtk_vbox_new(FALSE,0);
    gtk_container_set_border_width(GTK_CONTAINER(vbox),2);
    gtk_container_add( GTK_CONTAINER(window), vbox );

/* Create menues */
    action_group = gtk_action_group_new ("MenuActions");
    gtk_action_group_add_actions (action_group, entries, G_N_ELEMENTS (entries), window);

    ui_manager = gtk_ui_manager_new ();
    gtk_ui_manager_insert_action_group (ui_manager, action_group, 0);

    accel_group = gtk_ui_manager_get_accel_group (ui_manager);
    gtk_window_add_accel_group (GTK_WINDOW (window), accel_group);

    error = NULL;
    if (!gtk_ui_manager_add_ui_from_string (ui_manager, ui_description, -1, &error)) {
	g_message ("building menus failed: %s", error->message);
	g_error_free (error);
	exit (EXIT_FAILURE);
    }

    menubar = gtk_ui_manager_get_widget (ui_manager, "/MainMenu");
    gtk_box_pack_start (GTK_BOX (vbox), menubar, FALSE, FALSE, 0);

/* create the table widget to hold the periodic table */
    periodic_table = gtk_table_new(1,1,TRUE);
    gtk_box_pack_end( GTK_BOX(vbox), periodic_table, FALSE, FALSE, 0 );

/* now for each element in the table of elements, create a display */
/* item for it, and add it to the table... */
    for(i = 0; i < sizeof(table); ++i ) {
	if( !table[i].info[NAME] ) {
	    /* stop if no data... */
	    _DBG(g_message("%s(%d) %s : %d",__FILE__,__LINE__,
		 _("No symbol for index"), i));
	    break;
	}

/* create the button */
	table[i].button = gtk_button_new_with_label(_(table[i].info[SYMBOL]));
	if( !table[i].button ) {
	    g_warning("%s(%d) %s", __FILE__, __LINE__, _("Error button was null?"));
	    break;
	}

/******************************************************************************/
    {
      int j;
      GtkStyle *button_style = gtk_widget_get_style(window);

      table[i].style = gtk_style_copy(button_style);

      _DBG( g_message(N_("%s(%d) Setting color:  %d %d %d"),__FILE__,__LINE__,
        table[i].color.red,
        table[i].color.green,
        table[i].color.blue ) );

      
      if( enable_colorscale ) {
        colorize_element(i,
          &solid_fluid_cs, solid_fluid_crid,
          &condensed_gas_cs, condensed_gas_crit,
          0, 0
        );
      }
      else {
        /* set for 0, 1, and 2 */
        table[i].style->bg[0].red   = table[i].color.red;
        table[i].style->bg[0].green = table[i].color.green;
        table[i].style->bg[0].blue  = table[i].color.blue;
      }

      table[i].style->bg[1].red   = table[i].color.red;
      table[i].style->bg[1].green = table[i].color.green;
      table[i].style->bg[1].blue  = table[i].color.blue;

      table[i].style->bg[2].red   = table[i].color.red;
      table[i].style->bg[2].green = table[i].color.green;
      table[i].style->bg[2].blue  = table[i].color.blue;

      gtk_widget_set_style( table[i].button, table[i].style );
    }
/******************************************************************************/

/* connect the destroy method to it */
	g_signal_connect(GTK_OBJECT(table[i].button), "clicked",
			 G_CALLBACK(display_element_dialog), &table[i] );

/* set up a string for the tooltips */
	buff = g_strdup_printf(N_("%s  n:%s w:%s"), _(table[i].info[NAME]),
			       table[i].info[NUMBER], table[i].info[WEIGHT]);

/* create a new tooltips object... */
	table[i].tooltip = gtk_tooltips_new();
	gtk_tooltips_set_delay(table[i].tooltip,100);
	gtk_tooltips_set_tip(table[i].tooltip,table[i].button,buff,NULL);

/* attach the button to the table */
	gtk_table_attach(GTK_TABLE(periodic_table), table[i].button,
         		 table[i].x - 1, table[i].x, table[i].y - 1, table[i].y,
			 GTK_FILL, GTK_FILL, 2, 2);

    }

/* show all the widgets */
    gtk_widget_show_all(window);

/* give up controll till they exit the application */
    gtk_main();

    free(buff);
}


int main(int argc, char** argv) {
  signed char ch;

    textdomain (PACKAGE);
    bindtextdomain(PACKAGE, GPERIODIC_LOCALE_DIR);

    gtk_init(&argc,&argv);

/* parse the command line with getopt(3) */
    while( -1 != (ch=getopt(argc,argv,optstr)) ) {
	switch(ch) {
	    case 'c':
	    if (sscanf(optarg, "%lf", &ambientTemperature) != 1) {
		printf(N_(" Option -c error : %s is not a number.\n"), optarg);
		return -1;
	    }
	    if (ambientTemperature < 0) {
		printf(N_(" Option -c error : %s is not a valid number (must be > 0).\n"), optarg);
		return -1;
	    }
	    enable_colorscale = 1;
	    break;
	case 'h':
	    show_usage();
	    exit(0);
	    break;
	case 'D':
	    dump_table();
	    exit(0);
	    break;
	case 'v':
	    debug = 1;
	    break;
	default:
	    break;
	}
    }

    if(optind < argc) {
	while(optind < argc) {
	    int atomicNumber = atoi(argv[optind]);
	    if(!atomicNumber) { 
		atomicNumber = gpdata_get_atomic_number_for_symbol(argv[optind]);
	    }
	    if (atomicNumber == -1) {
		printf(N_("\n Unknown element : %s. Exiting.\n\n"),argv[optind]);
		return -1;
	    }
	    else {
		if (atomicNumber > ELEMENTNUM) {
		    printf(N_("\n Element number exceeds existing elements. Exiting.\n\n"));
		    return -1;
		}
		gpparse_print_element_data_for_num(atomicNumber);
		++optind;
	    }
	}
	return 0;
    }

    main_prog();

  return 0;
}


/*
                                                                                                                                                             
gperiodic - a program for browsing the periodic table and looking up
            information on elements.

Copyright (C) 1999-2002 Kyle R. Burton, 2003-2007 Jonas Frantz, 2014 Alexey Loginov
                                                                                                                                                             
    This file is part of gperiodic.
                                                                                                                                                             
    gperiodic is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
                                                                                                                                                             
    gperiodic is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
       
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
       
*/

#include <libintl.h>

#define _(String) g_locale_to_utf8(gettext(String), -1, &bytes_read, &bytes_written, NULL)
#define N_(String) (String)

/* structure to hold element data, as initialized from the static
 * table_data.c. */
enum info_types {
  NAME = 0,
  SYMBOL,
  NUMBER,
  WEIGHT,
  DENSITY,
  MELTING,
  BOILING,
  ATOMIC_RADIUS,
  COVALENT_RADIUS,
  IONIC_RADIUS,
  ATOMIC_VOLUME,
  SPECIFIC_HEAT,
  FUSION_HEAT,
  EVAPORATION_HEAT,
  TERMAL_CONDUCTIVITY,
  DEBYE_TEMP,
  PAULING,
  ENERGY,
  OXIDATION,
  CONFIGURATION,
  LATTICE,
  LATTICE_CONSTANT,
  LATTICE_CA_RATIO,
  APPEARANCE,
  DISCOVERY_DATE,
  DISCOVERED_BY,
  NAMED_AFTER,
  MAX_INFO_NR      /* Has to be the last element */
};

struct table_entry {
  char *info[MAX_INFO_NR];
  int   x;      /* x position in the table */
  int   y;      /* y position in the table */
  GdkColor    color;
  GtkStyle    *style;
  GtkWidget   *button;    /* associated gtk button */
#if !GTK_CHECK_VERSION(2,12,0)
  GtkTooltips *tooltip;    /* associated gtk tooltip */
#endif /* GTK_CHECK_VERSION */
};

struct colorscale {       /* table of maxcolor GkdColors */
  int maxcolor;
  GdkColor *scale;
};

typedef double criterion( struct table_entry *entry );

void dump_table(void);

int parse_table(void);

int interactive_shell(void);

/* debugging macro, does nothing unless _DEBUG is defined... */
#if defined(_DEBUG)
#  define _DBG(a) (a)
#else
#  define _DBG(a)
#endif

/************************************************************************************************/
/* 					ABOUT INFO 						*/
/************************************************************************************************/

#define PROGNAME "gperiodic"
#define COPYRIGHT "Copyright (C) 1999-2002 Kyle R. Burton, 2003-2007 Jonas Frantz, 2014 Alexey Loginov"
#define COMMENTS gettext("This program shows the periodic table of elements and information about these elements")
#define LICENSE gettext("gperiodic is free software; you can redistribute it and/or modify\nit under the terms of the GNU General Public License as published by\nthe Free Software Foundation; either version 2 of the License, or\n(at your option) any later version.\n\ngperiodic is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the implied warranty of\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\nGNU General Public License for more details.\n\nYou should have received a copy of the GNU General Public License\nalong with this program; if not, write to the Free Software\nFoundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA\n")
#define HOMEPAGEURL "http://code.google.com/p/gperiodic"
#define HOMEPAGELABEL gettext("gperiodic homepage")
#define AUTHORS {"Kyle R. Burton", "Jonas Frantz", "Alexey Loginov", "Piotr Najman", "Arno W. Peters", "Georges Khaznadar", "Mauricio Rivera", "Lalo Martins", "Sven Neumann", "Christian Klein", "Koen Kooi", "Ambrogio Oliva", "Felix Natter", "Michael Shigorin", "Costantino Ceoldo", "Arkadiusz Lipiec", "Sebastien Bacher", "Daniel Leidert", "Theppitak Karoonboonyanan", "Zach Ploskey", "Jon Abbott", "Andrew Dalke", "Gunner Poulsen", "Gonzalo Regueiro Iglesias", "Leandro Regueiro", "Andrea Tasso", "Osman Köşeli", "Frederic Gobry", "Ireneusz Imiolek", "Oleksandr Mishchenko", "Juan Rafael Fernández", "Fırat Kutlu", NULL}



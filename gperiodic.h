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
  GtkTooltips *tooltip;    /* associated gtk tooltip */
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
#define COPYRIGHT N_("Copyright (C) 2000 Jonas Frantz")
#define COMMENTS N_("This program shows the periodic table of elements and information about these elements")
#define LICENSE N_("gperiodic is free software; you can redistribute it and/or modify\nit under the terms of the GNU General Public License as published by\nthe Free Software Foundation; either version 2 of the License, or\n(at your option) any later version.\n\ngperiodic is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the implied warranty of\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\nGNU General Public License for more details.\n\nYou should have received a copy of the GNU General Public License\nalong with this program; if not, write to the Free Software\nFoundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA\n")
#define HOMEPAGEURL "http://www.frantz.fi/software/gperiodic.php"
#define HOMEPAGELABEL "gperiodic homepage"
#define AUTHORS {"Jonas Frantz", "Kyle R. Burton", "Arno W. Peters", "Costantino Ceoldo", NULL}



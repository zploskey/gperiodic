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

#include <errno.h>

#define ELEMENTNUM 118

struct table_entry header = {
  N_("Name: "),
  N_("Symbol: "),
  N_("Atomic Number: "),
  N_("Atomic Weight (u.m): "),
  N_("Density (g/cm&#179;): "),
  N_("Melting Point (K): "),
  N_("Boiling Point (K): "),
  N_("Atomic Radius (pm): "),
  N_("Covalent Radius (pm): "),
  N_("Ionic Radius (pm): "),
  N_("Atomic Volume (cm&#179;/mol): "),
  N_("Specific Heat (@20&#176;C J/(K*mol)): "),
  N_("Fusion Heat (kJ/mol): "),
  N_("Evaporation Heat (kJ/mol): "),
  N_("Thermal Conductivity (@25&#176;C W/(m*K)): "),
  N_("Debye temperature (K): "),
  N_("Pauling Negativity Number: "),
  N_("First Ionizing Energy (kJ/mol): "),
  N_("Oxidation states: "),
  N_("Electronic Configuration: "),
  N_("Lattice structure: "),
  N_("Lattice constant (&#x212B;): "),
  N_("Lattice c/a ratio: "),
  N_("Appearance: "),
  N_("Discovery date: "),
  N_("Discovered by: "),
  N_("Named after: ")
};

double temperature = 298.15;
GdkColor red_yellow2[3] = {
  {0x7FFFFFFF, 0x0000, 0x3000, 0x0000},
  {0x7FFFFFFF, 0x0000, 0xC000, 0x0000},
  {0x7FFFFFFF, 0x0000, 0x3000, 0xC000}
};

GdkColor yellow_white2[3]= {
  {0x7FFFFFFF, 0x0000, 0x3000, 0x0000},
	{0x7FFFFFFF, 0xC000, 0xC000, 0x0000},
	{0x7FFFFFFF, 0xC000, 0xC000, 0xC000}
};

struct colorscale solid_fluid_cs = { 3, red_yellow2 };

double ambientTemperature = 273.0;

static double solid_fluid_crid( struct table_entry *entry ) 
{
  double tm;
  char buffer[20];
  
  if( entry && entry->info[MELTING] && strcmp(entry->info[MELTING], _("n/a")) ) {
    strcpy(buffer, entry->info[MELTING]);
    tm = strtod(buffer,0);
    if(errno) tm = 0;
    if( ambientTemperature < tm ) return 0.0;
    else return 1.0;
  }
  
  return - 1.0;
}

struct colorscale condensed_gas_cs = { 3, yellow_white2 };

static double condensed_gas_crit(struct table_entry *entry)
{
  double tb;
  if(entry && entry->info[BOILING] && strcmp(entry->info[BOILING],_("n/a"))) {
    tb = strtod(entry->info[BOILING],0);
    if (errno) tb = 0;
    if (ambientTemperature < tb) return 0.0;
    else return 1.0;
  }
  else return -1.0;
}

/*
struct colorscale solid_fluid_cs = { 3, red_yellow2 };

double solid_fluid_crid( struct table_entry *entry ) 
{
  double tm;
	if( entry && entry->info[MELTING] && strcmp(entry->info[MELTING], _("n/a")) ) {
	  tm = strtod(entry->info[MELTING],0);
		if(!errno) tm = 0;
		if( temperature < tm ) return 0.0;
		else return 1.0;
	}

	return - 1.0;
}

struct colorscale condensed_gas_cs = { 3, yellow_white2 };

double condensed_gas_crit(struct table_entry *entry)
{
  double tb;
  if(entry && entry->info[BOILING] && strcmp(entry->info[BOILING],_("n/a"))) {
    tb = strtod(entry->info[BOILING],0);
    if (!errno) tb = 0;
    if (temperature < tb) return 0.0;
    else return 1.0;
  }
  else return -1.0;
}
*/

/*
 * NAME  SYMBOL  NUMBER  WEIGHT  DENSITY MELTING  BOILING  ATOMIC_RADIUS COVALENT_RADIUS
 * IONIC_RADIUS ATOMIC_VOLUME SPECIFIC_HEAT FUSION_HEAT EVAPORATION_HEAT TERMAL_CONDUCTIVITY DEBYE_TEMP
 * PAULING  ENERGY OXIDATION CONFIGURATION LATTICE LATTICE_CONSTANT LATTICE_CA_RATIO APPEARANCE DISCOVERY_DATE
 * DISCOVERED_BY NAMED_AFTER MAX_INFO_NR X_POS  Y_POS  PIXEL_DEPTH  RED  GREEN  BLUE
 *
 */

struct table_entry table [ELEMENTNUM+1] = {
  {N_("Hydrogen"), "H", "1", "1.00794", N_("0.0708 (@ -253&#176;C)"), "14.01", "20.28", "53-79", "32", "54 (-1e)", "14.1", "14.267 (H-H)", "0.117 (H-H)", "0.904 (H-H)", "0.1815", "110.00", "2.20", "1311.3", "1, 0, -1", "1s&#185;", N_("HEX"), "3.750", "1.731", N_("Colorless, odorless, tasteless gas"), N_("1766 (England)"), N_("Henry Cavendish"), N_("Greek: hydro (water) and genes (generate)"), 1, 1, 0x7FFFFFFF, 0xAAAA, 0xFFFF, 0xAAAA},
  {N_("Helium"), "He", "2", "4.002602", N_("0.147 (@ -270&#176;C)"), "0.95", "4.216", "28-31", "28-140", "93", "31.8", "5.188", N_("n/a"), "0.08", "0.152", N_("n/a"), "4.5", "2361.3", "0", "1s&#178;", N_("HEX"), "3.570", "1.633", N_("Inert, colorless, odorless, tasteless gas"), N_("1895 (Scotland/Sweden)"), N_("Sir William Ramsey, Nils Langet, P.T.Cleve"), N_("Greek: helios (sun)."), 18, 1, 0x7FFFFFFF, 0xC000, 0x7000, 0x0},

  {N_("Lithium"), "Li", "3", "6.941", "0.534", "453.69-553.69", "1118.5-1613", "145-155", "134-163", N_("68 (+1e) or 76 (+1e)"), "13.1", "3.489", "2.89", "148", "84.8", "400.00", "0.98", "519.9", "1", "[He] 2s&#185;", N_("BCC"), "3.490", N_("n/a"), N_("Soft, silvery-white metal"), N_("1817 (Sweden)"), N_("Johann Arfwedson"), N_("Greek: lithos (stone)."), 1, 2, 0x7FFFFFFF, 0x4444, 0x4444, 0xFFFF},
  {N_("Beryllium"), "Be", "4", "9.01218", "1.848", "1551", "3243", "112", "90", "35 (+2e)", "5.0", "1.824", "12.21", "309", "201", "1000.00", "1.57", "898.8", "2, 1", "[He] 2s&#178;", N_("HEX"), "2.290", "1.567", N_("Hard, brittle, steel-gray metal"), N_("1798 (France)"), N_("Louis-Nicolas Vauquelin"), N_("Greek: beryllos, 'beryl' (a mineral)."), 2, 2, 0x7FFFFFFF, 0xFFFF, 0x4444, 0x4444},
  {N_("Boron"), "B", "5", "10.811", "2.34", "2573", "3931", "98", "82", "23 (+3e)", "4.6", "1.025", "23.60", "504.5", "27.4", "1250.00", "2.04", "800.2", "3", "[He] 2s&#178; 2p&#185;", N_("RHL"), "8.730", "0.576", N_("Hard, brittle, lustrous black semimetal"), N_("1808 (England/France)"), N_("Sir H. Davy, J.L. Gay-Lussac, L.J. Thenard"), N_("The Arabic and Persian words for borax."), 13, 2, 0x7FFFFFFF, 0xFFFF, 0x4444, 0xFFFF},
  {N_("Carbon"), "C", "6", "12.011", N_("2.25 (graphite)"), "3820", "5100", "91", "77", "16 (+4e) 260 (-4e)", "5.3", "0.711", N_("n/a"), N_("n/a"), "1.59", N_("1860.00 (diamond)"), "2.55", "1085.7", "4, 3, 2, 1, 0, -1, -2, -3, -4", "[He] 2s&#178; 2p&#178;", N_("HEX (graphite), DIA (diamond)"), "3.570", N_("n/a"), N_("Dense, Black (graphite)"), N_("n/a (Unknown)"), N_("Known to the ancients."), N_("Latin: carbo (charcoal)."), 14, 2, 0x7FFFFFFF, 0xAAAA, 0xFFFF, 0xAAAA},
  {N_("Nitrogen"), "N", "7", "14.00674", N_("0.808 (@ -195.8&#176;C)"), "63.29", "77.4", "92", "75", "13 (+5e) 171 (-3e)", "17.3", "1.042 (N-N)", "0.720 (N-N)", "5.57 (N-N)", "0.026", N_("n/a"), "3.04", "1401.5", "5, 4, 3, 2, 1, 0, -1, -2, -3", "[He] 2s&#178; 2p&#179;", N_("HEX or CUB"), N_("4.039 (HEX)"), N_("1.651 (HEX)"), N_("Colorless, odorless, tasteless, and generally inert gas"), N_("1772 (Scotland)"), N_("Daniel Rutherford"), N_("Greek: nitron and genes (soda forming)."), 15, 2, 0x7FFFFFFF, 0xAAAA, 0xFFFF, 0xAAAA},
  {N_("Oxygen"), "O", "8", "15.9994", N_("1.149 (@ -183&#176;C)"), "54.8", "90.19", "60 (48)", "73", "132 (-2e)", "14.0", "0.916 (O-O)" , "0.444", "3.4099", "0.027", "155", "3.44", "1313.1", "-2, -1, -1/2, -1/3, 0, 1/2, 1, 2", "[He] 2s&#178; 2p&#8308;", N_(N_("MCL or CUB")), N_("6.830 (CUB)"), N_("n/a"), N_("Colorless, odorless, tasteless gas; pale blue liquid"), N_("1774 (England/Sweden)"), N_("Joseph Priestly, Carl Wilhelm Scheele"), N_("Greek: oxys and genes (acid former)."), 16, 2, 0x7FFFFFFF, 0xAAAA, 0xFFFF, 0xAAAA},
  {N_("Fluorine"), "F", "9", "18.998403", N_("1.108 (@ -189&#176;C)"), "53.53", "85.03", "71", "72", "(-1e) 133", "17.1", "0.824 (F-F)", "0.51 (F-F)", "6.54 (F-F)", "0.028", N_("n/a"), "3.98", "1680.0", "-1, 0", "[He] 2s&#178; 2p&#8309;", N_(N_("MCL")), N_("n/a"), N_("n/a"), N_("Greenish-yellow, pungent, corrosive gas"), N_("1886 (France)"), N_("Henri Moissan"), N_("Latin: fluere (flow)."), 17, 2, 0x7FFFFFFF, 0xFFFF, 0x4444, 0xFFFF},
  {N_("Neon"), "Ne", "10", "20.1797", N_("1.204 (@ -246&#176;C)"), "24.55", "27.1", "(38)", "58-71", "112", "16.8", "1.029", N_("n/a"), "1.74", "(0.0493)", "63.00", "4.4", "2079.4", N_("n/a"), "[He] 2s&#178; 2p&#8310;", N_("FCC"), "4.430", N_("n/a"), N_("Colorless, odorless, tasteless gas"), N_("1898 (England)"), N_("Sir William Ramsey, M.W. Travers"), N_("Greek: neos (new)."), 18, 2, 0x7FFFFFFF, 0xC000, 0x7000, 0x0},

  {N_("Sodium"), "Na", "11", "22.989769", "0.971", "370.96", "1156.1", "190", "154", "97 (+1e)", "23.7", "1.222", "2.64", "97.9", "142.0", "150.00", "0.93", "495.6", "1", "[Ne] 3s&#185;", N_("BCC"), "4.230", N_("n/a"), N_("Soft, silvery-white metal"), N_("1807 (England)"), N_("Sir Humphrey Davy"), N_("Medieval Latin: sodanum (headache remedy); symbol from Latin natrium (sodium carbonate)."), 1, 3, 0x7FFFFFFF, 0x4444, 0x4444, 0xFFFF},
  {N_("Magnesium"), "Mg", "12", "24.305", "1.738", "923", "1363", "160", "136", "66 (+2e)", "14.0", "1.025", "9.20", "131.8", "156", "318.00", "1.31", "737.3", "2", "[Ne] 3s&#178;", N_("HEX"), "3.210", "1.624", N_("Lightweight, malleable, silvery-white metal"), N_("1808 (England)"), N_("Sir Humphrey Davy"), N_("Magnesia, ancient city in district of Thessaly, Greece."), 2, 3, 0x7FFFFFFF, 0xFFFF, 0x4444, 0x4444},
  {N_("Aluminum"), "Al", "13", "26.981539", "2.6989", "933.5", "2740-2792", "143", "121&#177;4", "51 (+3e)", "10.0", "0.900", "10.75", "284.1", "237", "394.00", "1.61", "577.5; 1816.7", "3", "[Ne] 3s&#178; 3p&#185;", N_("FCC"), "4.050", N_("n/a"), N_("Soft, lightweight, silvery-white metal"), N_("1825 (Denmark)"), N_("Hans Christian Oersted"), N_("Latin: alumen, aluminis (alum)."), 13, 3, 0x7FFFFFFF, 0x0, 0xC000, 0xFFFF},
  {N_("Silicon"), "Si", "14", "28.0855", "2.33", "1688", "2623", "132", "111", "42 (+4e) 271  (-4e)", "12.1", "0.703", "50.6", "383", "149", "625.00", "1.90", "786.0", "4, 2, 0, -4", "[Ne] 3s&#178; 3p&#178;, [Ne] 3s 3p&#179;", N_("DIA"), "5.430", N_("n/a"), N_("Amorphous form is brown powder; crystalline form has a gray"), N_("1825 (Sweden)"), N_("Jons Jacob Berzelius"), N_("Latin: silex, silicus (flint)."), 14, 3, 0x7FFFFFFF, 0xFFFF, 0x4444, 0xFFFF},
  {N_("Phosphorus"), "P", "15", "30.973762", N_("1.82 (white phosphorus)"), "317.3", "553", "128", "106", "35 (+5e) 212 (-3e)", "17.0", "0.757", "2.51", "49.8", "(0.236)", N_("n/a"), "2.19", "1011.2", "5, 3, 1, 0, -1, -3", "[Ne] 3s&#178; 3p&#179;", N_("BCC"), "7.170", N_("n/a"), N_("The most common white form is a waxy, phosphorescent solid"), N_("1669 (Germany)"), N_("Hennig Brand"), N_("Greek: phosphoros (bringer of light)."), 15, 3, 0x7FFFFFFF, 0xAAAA, 0xFFFF, 0xAAAA},
  {N_("Sulfur"), "S", "16", "32.066", "2.070", "386", "717.824", "127", "102", "30 (+6e) 184 (-2e)", "15.5", "0.732", "1.23", "10.5", "0.27", N_("n/a"), "2.58", "999.0", "6, 4, 2, 1, 0, -1, -2", "[Ne] 3s&#178; 3p&#8308;", N_("ORC"), "10.470", N_("n/a"), N_("Tasteless, odorless, light-yellow, brittle solid"), N_("n/a (Unknown)"), N_("Known to the ancients."), N_("Latin: sulphur (brimstone)."), 16, 3, 0x7FFFFFFF, 0xAAAA, 0xFFFF, 0xAAAA},
  {N_("Chlorine"), "Cl", "17", "35.4527", N_("1.56 (@ -33.6&#176;C)"), "172.2", "238.6", "100", "99", "(+7e)27 (-1e)181", "18.7", "0.477 (Cl-Cl)", "6.41 (Cl-Cl)", "20.41 (Cl-Cl)", "0.009", N_("n/a"), "3.16", "1254.9", "7, 6, 5, 4, 3, 1, 0, -1", "[Ne] 3s&#178; 3p&#8309;", N_("ORC"), "6.240", N_("n/a"), N_("Greenish-yellow, disagreeable gas"), N_("1774 (Sweden)"), N_("Carl Wilhelm Scheele"), N_("Greek: chloros (greenish yellow)."), 17, 3, 0x7FFFFFFF, 0xFFFF, 0x4444, 0xFFFF},
  {N_("Argon"), "Ar", "18", "39.948", N_("1.40 (@ -189,35&#176;C)"), "83.8", "87.3", "(71)", "98-106", "154", "24.2", "0.138", "7.05", "6.45-6.52", "0.0177", "85.00", "0.0-4.3", "1519.6", "0", "[Ne] 3s&#178; 3p&#8310;", N_("FCC"), "5.260", N_("n/a"), N_("Colorless, tasteless, odorless noble gas"), N_("1894 (Scotland)"), N_("Sir William Ramsey, Baron Rayleigh"), N_("Greek: argos (inactive)."), 18, 3, 0x7FFFFFFF, 0xC000, 0x7000, 0x0},

  {N_("Potassium"), "K", "19", "39.0983", "0.856", "336.53-336.8", "1047", "235", "203", "(+1e)133", "45.3", "0.753", "2.33", "76.9", "79.0", "100.00", "0.82", "418.5", "1", "[Ar] 4s&#185;", N_("BCC"), "5.230", N_("n/a"), N_("Soft, waxy, silvery-white metal"), N_("1807 (England)"), N_("Sir Humphrey Davy"), N_("English: pot ash; symbol from Latin: kalium (alkali)."), 1, 4, 0x7FFFFFFF, 0x4444, 0x4444, 0xFFFF},
  {N_("Calcium"), "Ca", "20", "40.078", "1.55", "1112", "1757", "197", "174", "(+2e)99", "29.9", "0.653", "9.20", "153.6", "(201)", "230.00", "1.00", "589.4", "2", "[Ar] 4s&#178;", N_("FCC"), "5.580", N_("n/a"), N_("Fairly hard, silvery-white metal"), N_("1808 (England)"), N_("Sir Humphrey Davy"), N_("Latin: calx, calcis (lime)."), 2, 4, 0x7FFFFFFF, 0xFFFF, 0x4444, 0x4444},
  {N_("Scandium"), "Sc", "21", "44.95591", "2.99", "1814", "3104-3110", "162", "144", "(+3e)72.3", "15.0", "0.556", "15.8", "332.7", "15.8", N_("n/a"), "1.36", "630.8", "3", "[Ar] 3d&#185; 4s&#178;", N_("HEX"), "3.310", "1.594", N_("Fairly soft, silvery-white metal"), N_("1879 (Sweden)"), N_("Lars Nilson"), N_("Latin: Scandia (Scandinavia)."), 3, 4, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Titanium"), "Ti", "22", "47.867", "4.54", "1933&#177;20", "3560", "147", "132", "(+4e)68 (+2e)94", "10.6", "0.523", "18.8", "422.6", "21.9", "380.00", "1.54", "657.8", "2, 3, 4", "[Ar] 3d&#178; 4s&#178;", N_("HEX"), "2.950", N_("n/a"), N_("Shiny, dark-gray metal"), N_("1791 (England)"), N_("William Gregor"), N_("Greek: titanos (Titans)."), 4, 4, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Vanadium"), "V", "23", "50.9415", "6.11", "2160", "3650", "134", "122", "(+5e)59 (+3e)7", "8.35", "0.485", "17.5", "460", "30.7", "390.00", "1.63", "650.1", "5, 4, 3, 2, 0", "[Ar] 3d&#179; 4s&#178;", N_("BCC"), "3.020", N_("n/a"), N_("Soft, ductile, silvery-white metal"), N_("1830 (Sweden)"), N_("Nils Gabriel Sefstrom"), N_("The scandinavian goddess, Vanadis."), 5, 4, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Chromium"), "Cr", "24", "51.9961", "7,18-7.19", "2130", "2945", "130", "118", "(+6e)52 (+3e)63", "7.23", "0.488", "21", "342", "93.9", "460.00", "1.66", "652.4", "6, 3, 2, 0", "[Ar] 3d&#8309; 4s&#185;", N_("BCC"), "2.880", N_("n/a"), N_("Very hard, crystalline, steel-gray metal"), N_("1797 (France)"), N_("Louis Vauquelin"), N_("Greek: chroma (color)."), 6, 4, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Manganese"), "Mn", "25", "54.93805", "7.21", "1517", "2235", "135", "117", "(+7e) 46 (+2e) 80", "7.39", "0.477", "13.4", "221", "(7.8)", "400.00", "1.55", "716.8", "7, 6, 5, 4, 3, 2, 0, 1, -1", "[Ar] 3d&#8309; 4s&#178;", N_("CUB"), "8.890", N_("n/a"), N_("Hard, brittle, gray-white metal"), N_("1774 (Sweden)"), N_("Johann Gahn"), N_("Latin: magnes (magnet); Italian: manganese."), 7, 4, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Iron"), "Fe", "26", "55.847", "7.874", "1808-1812", "3023-3134", "126", "117", "(+3e) 64 (+2e) 74", "7.1", "25.14", "13.8", "~340", "80.4", "460.00", "1.83", "759.1", "6, 3, 2, 0", "[Ar] 3d&#8310; 4s&#178;", N_("BCC"), "2.870", N_("n/a"), N_("Malleable, ductile, silvery-white metal"), N_("n/a (Unknown)"), N_("Known to the ancients."), N_("Anglo-Saxon: iron; symbol from Latin: ferrum (iron)."), 8, 4, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Cobalt"), "Co", "27", "58.9332", "8.9", "1768", "3143", "125", "116", "(+3e) 63 (+2e) 72", "6.7", "0.456", "15.48", "389.1", "100", "385.00", "1.88", "758.1", "3, 2, 0, -1", "[Ar] 3d&#8311; 4s&#178;", N_("HEX"), "2.510", N_("n/a"), N_("Hard, ductile, lustrous bluish-gray metal"), N_("1735 (Sweden)"), N_("George Brandt"), N_("German: kobold (goblin)."), 9, 4, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Nickel"), "Ni", "28", "58.6934", "8.902", "1726", "3005", "124", "115", "(+2e) 69", "6.6", "0.443", "17.61", "378.6", "90.9", "375.00", "1.91", "736.2", "3, 2, 0", "[Ar] 3d&#8312; 4s&#178;", N_("FCC"), "3.520", N_("n/a"), N_("Hard, malleable, silvery-white metal"), N_("1751 (Sweden)"), N_("Axel Cronstedt"), N_("German: kupfernickel (false copper)."), 10, 4, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Copper"), "Cu", "29", "63.546", "8.92-8.96", "1356.55-1356.6", "2840-2840.15", "128", "117", "(+2e) 72 (+1e) 96", "7.1", "0.385", "13.01", "304.6", "401", "315.00", "1.90", "745.0", "3, 2, 1, 0", "[Ar] 3d&#185;&#8304; 4s&#185;", N_("FCC"), "3.610", N_("n/a"), N_("Malleable, ductile, reddish-brown metal"), N_("n/a (Unknown)"), N_("Known to the ancients."), N_("Symbol from Latin: cuprum (island of Cyprus famed for its copper mines)."), 11, 4, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Zinc"), "Zn", "30", "65.38", "7.133", "692.73-692.75", "1179.35-1180", "138", "125", "(+2e) 74", "9.2", "0.388", "7.28", "114.8", "116", "234.00", "1.65", "905.8", "2", "[Ar] 3d&#185;&#8304; 4s&#178;", N_("HEX"), "2.660", N_("n/a"), N_("Bluish-silver, ductile metal"), N_("n/a (Unknown)"), N_("Known to the ancients."), N_("German: Zink (German for zinc)."), 12, 4, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Gallium"), "Ga", "31", "69.723", "5.904-5.91", "302.93", "2477", "130-141", "122-126", "(+3e) 62 (+1e) 81", "11.8", "0.372", "5.59", "270.3", "28.1", "240.00", "1.81", "578.7", "3", "[Ar] 3d&#185;&#8304; 4s&#178; 4p&#185;", N_("ORC"), "4.510", N_("n/a"), N_("Soft, blue-white metal"), N_("1875 (France)"), N_("Paul-Emile Lecoq de Boisbaudran"), N_("Latin: Gallia (France)."), 13, 4, 0x7FFFFFFF, 0x0, 0xC000, 0xFFFF},
  {N_("Germanium"), "Ge", "32", "72.630", "5.323", "1210.6", "3103", "122.5-137", "122", "(+4e)53 (+2e)73", "13.6", "0.322", "36.8", "328", "60.2", "360.00", "2.01", "760.0", "4, 2", "[Ar] 3d&#185;&#8304; 4s&#178; 4p&#178;", N_("DIA"), "5.660", N_("n/a"), N_("Grayish-white metal"), N_("1886 (Germany)"), N_("Clemens Winkler"), N_("Latin: Germania (Germany)."), 14, 4, 0x7FFFFFFF, 0xFFFF, 0x4444, 0xFFFF},
  {N_("Arsenic"), "As", "33", "74.92160", N_("5.73 (grey arsenic)"), "1090", "876-886", "139", "120", "(+5e)46 (-3e)222", "13.1", "0.328", N_("24.44 (grey arsenic)"), "32.4", "(50.2)", "285.00", "2.18", "946.2", "5, 3, -3", "[Ar] 3d&#185;&#8304; 4s&#178; 4p&#179;", N_("TRG"), "4.130", N_("n/a"), N_("Steel gray, brittle semimetal"), N_("n/a (Unknown)"), N_("Known to the ancients."), N_("Greek: arsenikon; Latin: arsenicum (both names for yellow pigment)."), 15, 4, 0x7FFFFFFF, 0xFFFF, 0x4444, 0xFFFF},
  {N_("Selenium"), "Se", "34", "78.96", "4.79", "490", "958.1", "140", "116", "(+6e)42 (-2e)191", "16.5", "0.321 (Se-Se)", "5.23", "59.7", "0.52", "90", "2.55", "940.4", "6, 4, -2", "[Ar] 3d&#185;&#8304; 4s&#178; 4p&#8308;", N_("HEX"), "4.360", N_("n/a"), N_("A soft metalloid similar to sulfur"), N_("1817 (Sweden)"), N_("Jons Jacob Berzelius"), N_("Greek: selene (moon)."), 16, 4, 0x7FFFFFFF, 0xAAAA, 0xFFFF, 0xAAAA},
  {N_("Bromine"), "Br", "35", "79.904", "3.102-3.12", "265.9", "331.9", N_("n/a"), "114", "(+5e)47 (-1e)196", "23.5", "0.473 (Br-Br)", "10.57 (Br-Br)", "29.56 (Br-Br)", "0.005", N_("n/a"), "2.96", "1142.0", "7, 5, 3, 1, 0, -1", "[Ar] 3d&#185;&#8304; 4s&#178; 4p&#8309;", N_("ORC"), "6.670", N_("n/a"), N_("Reddish-brown liquid"), N_("1826 (France)"), N_("Antoine J. Balard"), N_("Greek: bromos (stench)."), 17, 4, 0x7FFFFFFF, 0xFFFF, 0x4444, 0xFFFF},
  {N_("Krypton"), "Kr", "36", "83.798", N_("2.155 (@ -153&#176;C)"), "116.6", "120.85", "(88)", "112-116", "169", "32.2", "0.247", N_("n/a"), "9.05", "0.0095", "72", "0.0-3.0", "1350.0", "2", "[Ar] 3d&#185;&#8304; 4s&#178; 4p&#8310;", N_("FCC"), "5.720", N_("n/a"), N_("Dense, colorless, odorless, and tasteless gas"), N_("1898 (Great Britain)"), N_("Sir William Ramsey, M.W. Travers"), N_("Greek: kryptos (hidden)."), 18, 4, 0x7FFFFFFF, 0xC000, 0x7000, 0x0},

  {N_("Rubidium"), "Rb", "37", "85.4678", "1.532", "312.2", "961-961.15", "248", "216", "(+1e)147", "55.9", "0.360", "2.20", "75.8", "58.2", "56", "0.82", "402.8", "1", "[Kr] 5s&#185;", N_("BCC"), "5.590", N_("n/a"), N_("Soft, silvery-white, highly reactive metal"), N_("1861 (Germany)"), N_("R. Bunsen, G. Kirchoff"), N_("Latin: rubidus (deep red); the color its salts impart to flames."), 1, 5, 0x7FFFFFFF, 0x4444, 0x4444, 0xFFFF},
  {N_("Strontium"), "Sr", "38", "87.62", "2.54", "1042", "1657", "215", "191", "(+2e) 112", "33.7", "0.301", "9.20", "144", "(35.4)", "147", "0.95", "549.0", "2", "[Kr] 5s&#178;", N_("FCC"), "6.080", N_("n/a"), N_("Silvery, malleable metal"), N_("1790 (Scotland)"), N_("A. Crawford"), N_("The Scottish town, Strontian."), 2, 5, 0x7FFFFFFF, 0xFFFF, 0x4444, 0x4444},
  {N_("Yttrium"), "Y", "39", "88.90585", "4.47", "1795", "3611", "178", "162", "(+3e) 89.3", "19.8", "0.284", "11.5", "367", "(17.2)", "280", "1.22", "615.4", "3", "[Kr] 4d&#185; 5s&#178;", N_("HEX"), "3.650", "1.571", N_("Silvery, ductile, fairly reactive metal"), N_("1794 (Finland)"), N_("Johann Gadolin"), N_("The Swedish village, Ytterby, where one of its minerals was first found."), 3, 5, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Zirconium"), "Zr", "40", "91.224", "6.506", "2125", "4650", "160", "145", "(+4e)79", "14.1", "0.281", "19.2", "567", "22.7", "250-291", "1.33", "659.7", "0, 1, 2, 3, 4", "[Kr] 4d&#178; 5s&#178;", N_("HEX"), "3.230", "1.593", N_("Gray-white, lustrous, corrosion-resistant metal"), N_("1789 (Germany)"), N_("Martin Klaproth"), N_("The mineral, zircon."), 4, 5, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Niobium"), "Nb", "41", "92.90638", "8.57", "2741", "5015", "146", "134-164", "(+5e)69", "10.8", "0.268", "26.8", "680", "53.7", "275.00", "1.6", "663.6", "5, 4, 3, 2, 1", "[Kr] 4d&#8308; 5s&#185;", N_("BCC"), "3.300", N_("n/a"), N_("Shiny white, soft, ductile metal"), N_("1801 (England)"), N_("Charles Hatchet"), N_("Niobe; daughter of the mythical Greek king Tantalus."), 5, 5, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Molybdenum"), "Mo", "42", "95.96", "10.22", "2890", "4885", "139", "130", " (+6e) 62 (+4e) 70", "9.4", "0.251", "28", "~590", "(138)", "380.00-450.00", "2.16", "684.8", "6, 5, 4, 3, 2, 0", "[Kr] 4d&#8309; 5s&#185;", N_("BCC"), "3.150", N_("n/a"), N_("Silvery white, hard metal"), N_("1778 (Sweden)"), N_("Carl Wilhelm Scheele"), N_("Greek: molybdos (lead)."), 6, 5, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Technetium"), "Tc", "43", "97.9072", "11.5", "2430-2445", "4538-5150", "136", "127", "(+7e)56", "8.5", "0.243", "23.8", "585", "50.6", "453", "1.9", "702.2", "-1, 0, 1, 2, 3, 4, 5, 6, 7", "[Kr] 4d&#8309; 5s&#178;", N_("HEX"), "2.740", "1.604", N_("Silvery-gray metal"), N_("1937 (Italy)"), N_("Carlo Perrier, Emilio Segre"), N_("Greek: technetos (artificial)."), 7, 5, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Ruthenium"), "Ru", "44", "101.07", "12.41", "2583-2607.15", "4173-4350.15", "134", "125", "(+4e) 67", "8.3", "0.238", "(25.5)", N_("n/a"), "117.0", "600", "2.2", "710.3", "2, 3, 4, 6, 8, 0, -2", "[Kr] 4d&#8311; 5s&#185;", N_("HEX"), "2.700", "1.584", N_("Rare, silver-gray, extremely brittle metal"), N_("1844 (Russia)"), N_("Karl Klaus"), N_("Latin: Ruthenia (Russia)."), 8, 5, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Rhodium"), "Rh", "45", "102.9055", "12.41", "2239", "4000", "134", "125", "(+3e)68", "8.3", "0.244", "21.8", "494", "150", "480", "2.28", "719.5", "5, 4, 3,  2, 1, 0", "[Kr] 4d&#8312; 5s&#185;", N_("FCC"), "3.800", N_("n/a"), N_("Silvery white, hard metal"), N_("1803 (England)"), N_("William Wollaston"), N_("Greek: rhodon (rose). Its salts give a rosy solution."), 9, 5, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Palladium"), "Pd", "46", "106.42", "12.02", "1825-1827", "2940-3413", "137", "128", "(+4e) 65 (+2e) 80", "8.9", "0.244", "17.24", "372.4", "71.8", "274.00-275.00", "2.20", "803.5", "0, 1, 2, 3, 4, 5, 6", "[Kr] 4d&#185;&#8304;", N_("FCC"), "3.890", N_("n/a"), N_("Silvery-white, soft, malleable and ductile metal"), N_("1803 (England)"), N_("William Wollaston"), N_("Named after the asteroid, Pallas, discovered in 1803."), 10, 5, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Silver"), "Ag", "47", "107.8682", "10.5", "1235.1", "2485", "144", "134", "(+2e) 89 (+1e) 126", "10.3", "0.237", "11.95", "254.1", "429", "215.00-225.00", "1.93", "730.5; 2070; 3361", "2, 1", "[Kr] 4d&#185;&#8304; 5s&#185;", N_("FCC"), "4.090", N_("n/a"), N_("Silvery-ductile, and malleable metal"), N_("n/a (Unknown)"), N_("Known to the ancients."), N_("Anglo-Saxon: siolful (silver); symbol from Latin: argentum."), 11, 5, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Cadmium"), "Cd", "48", "112.411", "8.65", "594.1-594.22", "1038-1040", "154-161", "148", "(+2e) 97", "13.1", "0.232", "6.11", "59.1-100", "96.9", "209.00", "1.69", "867.2-867.8", "2", "[Kr] 4d&#185;&#8304; 5s&#178;", N_("HEX"), "2.980", "1.886", N_("Soft, malleable, blue-white metal"), N_("1817 (Germany)"), N_("Fredrich Stromeyer"), N_("Greek: kadmeia (ancient name for calamine (zinc oxide))."), 12, 5, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Indium"), "In", "49", "114.818", "7.31", "429.32", "2353", "166", "144", "(+3e) 81", "15.7", "0.234", "3.24", "225.1", "81.8", "129.00", "1.78", "558.0", "1, 3", "[Kr] 4d&#185;&#8304; 5s&#178; 5p&#185;", N_("TET"), "4.590", N_("n/a"), N_("Very soft, silvery-white metal"), N_("1863 (Germany)"), N_("Ferdinand Reich, T. Richter"), N_("Latin: indicum (color indigo), the color it shows in a spectroscope."), 13, 5, 0x7FFFFFFF, 0x0, 0xC000, 0xFFFF},
  {N_("Tin"), "Sn", "50", "118.71", "7.31", "505.05-505.1", "2543-2873", "162", "141", "(+4e) 71 (+2) 93", "16.3", "0.222", "7.07", "296", "66.8", "170.00", "1.96", "708.2", "4, 2", "[Kr] 4d&#185;&#8304; 5s&#178; 5p&#178;", N_("TET"), "5.820", N_("n/a"), N_("Silvery-white, soft, malleable and ductile metal"), N_("n/a (Unknown)"), N_("Known to the ancients."), N_("Named after Etruscan god, Tinia; symbol from Latin: stannum (tin)."), 14, 5, 0x7FFFFFFF, 0x0, 0xC000, 0xFFFF},
  {N_("Antimony"), "Sb", "51", "121.760", "6.691", "903.9", "1908", "159", "140", "(+6e)62 (−3e)245", "18.4", "0.205", "20.08", "195.2", "24.43", "200.00", "2.05", "833.3", "5, 3, -3", "[Kr] 4d&#185;&#8304; 5s&#178; 5p&#179;", N_("TRG"), "4.510", N_("n/a"), N_("Hard, silvery-white, brittle semimetal"), N_("n/a (Unknown)"), N_("Known to the ancients."), N_("Greek: anti and monos (not alone); symbol from mineral stibnite."), 15, 5, 0x7FFFFFFF, 0xFFFF, 0x4444, 0xFFFF},
  {N_("Tellurium"), "Te", "52", "127.6", "6.24", "722.7", "1263", "160", "136", "(+6e) 56 211 (−2e)", "20.5", "0.201", "17.91", "49.8", "14.3", N_("n/a"), "2.1", "869.0", "6, 4, 2, -2", "[Kr] 4d&#185;&#8304; 5s&#178; 5p&#8308;", N_("HEX"), "4.450", "1.330", N_("Silvery-white, brittle semimetal"), N_("1782 (Romania)"), N_("Franz Joseph Meller von Reichenstein"), N_("Latin: tellus (earth)."), 16, 5, 0x7FFFFFFF, 0xFFFF, 0x4444, 0xFFFF},
  {N_("Iodine"), "I", "53", "126.90447", "4.93", "386.65-386.85", "457.4-457.5", "136", "133", "(+7e) 50 (-1e) 220", "25.7", "0.427 (I-I)", "15.52 (I-I)", "41.95 (I-I)", "(0.45)", N_("n/a"), "2.66", "1008.3", "7, 5, 3, 1, 0, -1", "[Kr] 4d&#185;&#8304; 5s&#178; 5p&#8309;", N_("ORC"), "7.720", N_("n/a"), N_("Shiny, black nonmetallic solid"), N_("1811 (France)"), N_("Bernard Courtois"), N_("Greek: iodes (violet colored)."), 17, 5, 0x7FFFFFFF, 0xFFFF, 0x4444, 0xFFFF},
  {N_("Xenon"), "Xe", "54", "131.29", N_("3.52 (@ -107.05&#176;C)"), "161.3", "166.1", "(108)", "131-140", "190", "42.9", "0.158", "2.27", "12.65", "0.0057", N_("n/a"), "0.0-2.6", "1170.0", "0, 1, 2, 4, 6, 7, 8", "[Kr] 4d&#185;&#8304; 5s&#178; 5p&#8310;", N_("FCC"), "6.200", N_("n/a"), N_("Heavy, colorless, and odorless noble gas"), N_("1898 (England)"), N_("Sir William Ramsey, M.W. Travers"), N_("Greek: xenos (strange)."), 18, 5, 0x7FFFFFFF, 0xC000, 0x7000, 0x0},

  {N_("Cesium"), "Cs", "55", "132.90545", "1.873", "301.6-301.85", "940.75-951.6", "267", "235", "(+1e) 167", "70.0", "0.241", "2.09", "68.3", "35.9", "39.2", "0.79", "375.5", "1", "[Xe] 6s&#185;", N_("BCC"), "6.050", N_("n/a"), N_("Very soft, ductile, light gray metal"), N_("1860 (Germany)"), N_("Gustov Kirchoff, Robert Bunsen"), N_("Latin: coesius (sky blue); for the blue lines of its spectrum."), 1, 6, 0x7FFFFFFF, 0x4444, 0x4444, 0xFFFF},
  {N_("Barium"), "Ba", "56", "137.327", "3.5", "1002", "1910", "222", "198", "(+2e) 134", "39.0", "0.192", "7.66", "142.0", "(18.4)", N_("n/a"), "0.89", "502.5", "2", "[Xe] 6s&#178;", N_("BCC"), "5.020", N_("n/a"), N_("Soft, slightly malleable, silver-white metal"), N_("1808 (England)"), N_("Sir Humphrey Davy"), N_("Greek: barys (heavy or dense)."), 2, 6, 0x7FFFFFFF, 0xFFFF, 0x4444, 0x4444},

  {N_("Lanthanum"), "La", "57", "138.9055", "6.15-6.18", "920-1194", "3447-3730", "187", "169", "101.(+3e) 6", "22.5", "0.197", "8.5", "402", "13.4", "132.00", "1.10", "541.1", "3", "[Xe] 5d&#185; 6s&#178;", N_("HEX"), "3.750", "1.619", N_("Silvery-white, soft, malleable, and ductile metal"), N_("1839 (Sweden)"), N_("Carl Mosander"), N_("Greek: lanthanein (to be hidden)."), 3, 9, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Cerium"), "Ce", "58", "140.116", "6.757", "1072", "3699", "181", "165", "(+4e) 92 103.(+3e) 4", "21.0", "0.205", "5.2", "398", "11.3", N_("n/a"), "1.12", "540.1", "4, 3", "[Xe] 4f&#185; 5d&#185; 6s&#178;", N_("FCC"), "5.160", N_("n/a"), N_("Malleable, ductile, iron-gray metal"), N_("1803 (Sweden/Germany)"), N_("W. von Hisinger, J. Berzelius, M. Klaproth"), N_("Named after the asteroid, Ceres, discovered two years before the element."), 4, 9, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Praseodymium"), "Pr", "59", "140.90765", "6.773", "1204", "3785", "182", "165", "(+4e) 90 101.(+3e) 3", "20.8", "0.192", "11.3", "331", "12.5", N_("n/a"), "1.13", "526.6", "4, 3", "[Xe] 4f&#179; 6s&#178;", N_("HEX"), "3.670", "3.22", N_("Silvery white, moderately soft, malleable, and ductile metal"), N_("1885 (Austria)"), N_("C.F. Aver von Welsbach"), N_("Greek: prasios and didymos (green twin); from its green salts."), 5, 9, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Neodymium"), "Nd", "60", "144.242", "7.007", "1294", "3341", "182", "184", "99.(+3e) 5", "20.6", "0.205", "7.1", "289", "(16.5)", N_("n/a"), "1.14", "531.5", "3", "[Xe] 4f&#8308; 6s&#178;", N_("HEX"), "3.660", "1.614", N_("Silvery-white, rare-earth metal that oxidizes easily in air"), N_("1925 (Austria)"), N_("C.F. Aver von Welsbach"), N_("Greek: neos and didymos (new twin)."), 6, 9, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Promethium"), "Pm", "61", "144.9127", "7.2-7.26", "1441", "3000-~3273", "183-185(205)", "199", N_("(+3e)97.9 or (+3e)111"), "19.96", "0.185", "7.13", "330.5", "17.9", N_("n/a"), "1.1-1.13", "536", "3", "[Xe] 4f&#8309; 6s&#178;", N_("HEX"), N_("n/a"), N_("n/a"), N_("Light-grey, radioactive element"), N_("1945 (United States)"), N_("J.A. Marinsky, L.E. Glendenin, C.D. Coryell"), N_("Named for the Greek god, Prometheus."), 7, 9, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Samarium"), "Sm", "62", "150.36", "7.520", "1350", "2064", "181", "162", "(+3e) 96.4", "19.9", "0.180", "8.9", "165", "(13.3)", "166.00", "1.17", "540.1", "3, 2", "[Xe] 4f&#8310; 6s&#178;", N_("RHL"), "9.000", N_("n/a"), N_("Silvery rare-earth metal"), N_("1880 (France)"), N_("Jean Charles Galissard de Marignac"), N_("Named after the mineral samarskite."), 8, 9, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Europium"), "Eu", "63", "151.964", "5.243", "1095", "1870", "199", "185", "(+3e) 95 (+2e) 109", "28.9", "0.176", "1095", "176", "13.9", N_("n/a"), "0.0-1.2", "546.9", "3, 2", "[Xe] 4f&#8311; 6s&#178;", N_("BCC"), "4.610", N_("n/a"), N_("Soft, silvery-white metal"), N_("1901 (France)"), N_("Eugene-Antole Demarcay"), N_("Named for the continent of Europe."), 9, 9, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Gadolinium"), "Gd", "64", "157.25", "7.900", "1586", "3539", "179", "161", "(+3e) 93.8", "19.9", "0.230", "10.0", "398", "(10.5)", N_("n/a"), "1.20", "594.2", "3", "[Xe] 4f&#8311; 5d&#185; 6s&#178;", N_("HEX"), "3.640", "1.588", N_("Soft, ductile, silvery-white metal"), N_("1880 (Switzerland)"), N_("Jean Charles Galissard de Marignac"), N_("Named after the mineral gadolinite."), 10, 9, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Terbium"), "Tb", "65", "158.92535", "8.229", "1629", "3296", "180", "159", "(+4e) 84 (+3e) 92.3", "19.2", "0.183", N_("n/a"), "389", "11.1", N_("n/a"), "1.2", "569", "4, 3", "[Xe] 4f&#8313; 6s&#178;", N_("HEX"), "3.600", "1.581", N_("Soft, ductile, silvery-gray, rare-earth metal"), N_("1843 (Sweden)"), N_("Carl Mosander"), N_("Named after Ytterby, a village in Sweden."), 11, 9, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Dysprosium"), "Dy", "66", "162.50", "8.55", "1685", "2835", "180", "159", "(+3e) 90.8", "19.0", "0.173", N_("n/a"), "291", "10.7", N_("n/a"), N_("n/a"), "567", "3", "[Xe] 4f&#185;&#8304; 6s&#178;", N_("HEX"), "3.590", "1.573", N_("Soft. lustrous, silvery metal"), N_("1886 (France)"), N_("Paul-Emile Lecoq de Boisbaudran"), N_("Greek: dysprositos (hard to get at)."), 12, 9, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Holmium"), "Ho", "67", "164.93032", "8.795", "1747", "2968", "179", "158", "(+3e) 89.4", "18.7", "0.164", N_("n/a"), "301", "(16.2)", N_("n/a"), "1.23", "574", "3", "[Xe] 4f&#185;&#185; 6s&#178;", N_("HEX"), "3.580", "1.570", N_("Fairly soft, malleable, lustrous, silvery metal"), N_("1879 (Switzerland)"), N_("J.L. Soret"), N_("Holmia, the Latinized name for Stockholm, Sweden."), 13, 9, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Erbium"), "Er", "68", "167.259", "9.06", "1802", "3136", "178", "157", "(+3e) 88.1", "18.4", "0.168", N_("n/a"), "317", "(14.5)", N_("n/a"), "1.24", "581", "3", "[Xe] 4f&#185;&#178; 6s&#178;", N_("HEX"), "3.560", "1.570", N_("Soft, malleable, silvery metal"), N_("1843 (Sweden)"), N_("Carl Mosander"), N_("Named after the Swedish town, Ytterby."), 14, 9, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Thulium"), "Tm", "69", "168.93421", "9.321", "1818", "2220", "177", "156", "(+3e) 87", "18.1", "0.160", N_("n/a"), "232", "(16.9)", N_("n/a"), "1.25", "589", "3, 2", "[Xe] 4f&#185;&#179; 6s&#178;", N_("HEX"), "3.540", "1.570", N_("Soft, malleable, ductile, silvery metal"), N_("1879 (Sweden)"), N_("Per Theodor Cleve"), N_("Thule, ancient name of Scandinavia."), 15, 9, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Ytterbium"), "Yb", "70", "173.055", "6.9654", "1097", "1466", "194", "170", "(+3e) 85.8 (+2e) 93", "24.8", "0.145", "3.35", "159", "(34.9)", N_("n/a"), "1.1", "603", "3, 2", "[Xe] 4f&#185;&#8308; 6s&#178;", N_("FCC"), "5.490", N_("n/a"), N_("Silvery, lustrous, malleable, and ductile metal"), N_("1878 (Switzerland)"), N_("Jean Charles Galissard de Marignac"), N_("Named for the Swedish village of Ytterby."), 16, 9, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},

  {N_("Lutetium"), "Lu", "71", "174.9668", "9.8404", "1936", "3668", "175", "156", "(+3e) 85", "17.8", "0.155", N_("n/a"), "414", "(16.4)", N_("n/a"), "1.27", "513", "3", "[Xe] 4f&#185;&#8308; 5d&#185; 6s&#178;", N_("HEX"), "3.510", "1.585", N_("Silvery-white, hard, dense, rare-earth metal"), N_("1907 (France)"), N_("Georges Urbain"), N_("Named for the ancient name of Paris, Lutecia."), 3, 6, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Hafnium"), "Hf", "72", "178.49", "13.31", "2503-2506", "4876-5470", "167", "144", "(+4e) 78", "13.6", "0.146", "(25.1)", "575", "23.0", N_("n/a"), "1.3", "575.2", "4", "[Xe] 4f&#185;&#8308; 5d&#178; 6s&#178;", N_("HEX"), "3.200", "1.582", N_("Silvery, ductile metal"), N_("1923 (Denmark)"), N_("Dirk Coster, Georg von Hevesy"), N_("Hafnia, the Latin name of Copenhagen."), 4, 6, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Tantalum"), "Ta", "73", "180.9479", "16.65-16.654", "3269-3290", "5698-5731", "149", "134", "(+5e) 68", "10.9", "0.140", "24.7", "758", "57.5", "225.00", "1.5", "760.1", "5", "[Xe] 4f&#185;&#8308; 5d&#179; 6s&#178;", N_("BCC"), "3.310", N_("n/a"), N_("Gray, heavy, hard metal"), N_("1802 (Sweden)"), N_("Anders Ekeberg"), N_("King Tantalus of Greek mythology, father of Niobe."), 5, 6, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Tungsten"), "W", "74", "183.84", "19.25-19.3", "3680-3695", "5828-5930", "141", "130-170", "(+6e) 62 (+4e) 70", "9.53", "0.133", "35", "824", "173", "310.00", "1.7-2.3", "769.7", "6, 5, 4, 3, 2, 0", "[Xe] 4f&#185;&#8308; 5d&#8308; 6s&#178;", N_("BCC"), "3.160", N_("n/a"), N_("Tough, steel-gray to white metal"), N_("1783 (Spain)"), N_("Juan Jose, Fausto Elhuyar"), N_("Swedish: tung sten (heavy stone): symbol from its German name wolfram."), 6, 6, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Rhenium"), "Re", "75", "186.207", "21.02", "3453-3459", "5869-5900", "137", "128", "(+7e) 53 (+4e) 72", "8.85", "0.138", "34", "704", "48.0", "416.00", "1.9", "759.1", "7, 6, 5, 4, 3, 2, -1", "[Xe] 4f&#185;&#8308; 5d&#8309; 6s&#178;", N_("HEX"), "2.760", "1.615", N_("Dense, silvery-white metal"), N_("1925 (Germany)"), N_("Walter Noddack, Ida Tacke, Otto Berg"), N_("Latin: Rhenus, the Rhine River."), 7, 6, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Osmium"), "Os", "76", "190.23", "22.57-22.61", "3306-3327", "5285-5300", "135", "126", "(+6e) 69 (+4e) 88", "8.43", "0.131", "31.7", "738", "(87.6)", N_("n/a"), "2.2", "819.8", "8, 6, 4, 3, 2, 0, -2", "[Xe] 4f&#185;&#8308; 5d&#8310; 6s&#178;", N_("HEX"), "2.740", "1.579", N_("Blue-white, lustrous, hard metal"), N_("1804 (England)"), N_("Smithson Tenant"), N_("Greek: osme (odor)."), 8, 6, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Iridium"), "Ir", "77", "192.217", "22.42-22.65", "2683-2739", "4403-4701", "136", "127", "(+4e) 68", "8.54", "0.133", "26.0-27.61", "604-610", "147", "430.00", "2.20", "868.1", "6, 4, 3, 2, 1, 0, -1", "[Xe] 4f&#185;&#8308; 5d&#8311; 6s&#178;", N_("FCC"), "3.840", N_("n/a"), N_("White, brittle metal"), N_("1804 (England/France)"), N_("S.Tenant, A.F.Fourcroy, L.N.Vauquelin, H.V.Collet-Descoltils"), N_("Latin: iris (rainbow)."), 9, 6, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Platinum"), "Pt", "78", "195.08", "21.09-21.45", "2041.4-2045", "4098-4100", "139", "130", "(+4e) 65 (+2e) 80", "9.10", "0.133", "21.76", "~470", "71.6", "230.00", "2.28", "868.1", "4, 2, 0", "[Xe] 4f&#185;&#8308; 5d&#8313; 6s&#185;", N_("FCC"), "3.920", N_("n/a"), N_("Very heavy, soft, silvery-white metal"), N_("1557 (Italy), but Incas use before"), N_("Julius Scaliger"), N_("Spanish: platina (little silver)."), 10, 6, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Gold"), "Au", "79", "196.96657", "19.3", "1337.33-1337.58", "3080-3129", "144-146", "134", N_("(-3e) 185 (+1e) 137 or (+3e) 85 (+1e) 137"), "10.2", "0.129", "12.68", "~340", "318", "170.00", "2.54-2.64", "889.3", "-1, 1, 3, 5", "[Xe] 4f&#185;&#8308; 5d&#185;&#8304; 6s&#185;/[Xe] 5p&#8310; 5d&#185;&#8304; 6s&#185;", N_("FCC"), "4.080", N_("n/a"), N_("Soft, malleable, yellow metal"), N_("n/a (Unknown)"), N_("Known to the ancients."), N_("Anglo-Saxon: geolo (yellow); symbol from Latin: aurum (shining dawn)."), 11, 6, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Mercury"), "Hg", "80", "200.59", N_("13.546 (@ +20&#176;C)"), "234.28-234.32", "629.73-629.88", "157", "149", "(+2e) 110 (+1e) 127", "14.8", "0.138", "2.295", "58.5", "8.3", "100.00", "2.00", "1006.0", "2, 1", "[Xe] 4f&#185;&#8308; 5d&#185;&#8304; 6s&#178;", N_("RHL"), "2.990", "1.94", N_("Heavy, silver-white metal that is in its liquid state at"), N_("n/a (Unknown)"), N_("Known to the ancients."), N_("The Roman god Mercury; symbol from Latin: hydrargyrus (liquid silver)."), 12, 6, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Thallium"), "Tl", "81", "204.3833", "11.85", "576.6-577", "1730-1746", "171", "148", "(+3e) 95 (+1e) 147", "17.2", "0.128", "4.31", "162.4", "46.1", "96.00", "1.62", "588.9", "3, 1", "[Xe] 4f&#185;&#8308; 5d&#185;&#8304; 6s&#178; 6p&#185;", N_("HEX"), "3.460", "1.599", N_("Soft, gray metal"), N_("1861 (England)"), N_("Sir William Crookes"), N_("Greek: thallos (green twig), for a bright green line in its spectrum."), 13, 6, 0x7FFFFFFF, 0x0, 0xC000, 0xFFFF},
  {N_("Lead"), "Pb", "82", "207.2", "11.34-11.35", "600.61-600.65", "2013-2022", "175", "147", "(+4e) 84 (+2e) 120", "18.3", "0.159", "4.77", "177.8", "35.3", "88.00", "1.8", "715.2", "4, 2, 0", "[Xe] 4f&#185;&#8308; 5d&#185;&#8304; 6s&#178; 6p&#178;", N_("FCC"), "4.950", N_("n/a"), N_("Very soft, highly malleable and ductile, blue-white shiny metal"), N_("n/a (Unknown)"), N_("Known to the ancients."), N_("Anglo-Saxon: lead; symbol from Latin: plumbum."), 14, 6, 0x7FFFFFFF, 0x0, 0xC000, 0xFFFF},
  {N_("Bismuth"), "Bi", "83", "208.9804", "9.747-9.79", "544.5", "1564-1883", "170", "146", "(+5e) 74 (+3e) 96", "21.3", "0.124", "11.00-11.30", "172.0", "7.9", "120.00", "2.02", "702.9", "5, 3", "[Xe] 4f&#185;&#8308; 5d&#185;&#8304; 6s&#178; 6p&#179;", N_("RHL"), "4.750", N_("n/a"), N_("Hard, brittle, steel-gray metal with a pinkish tinge"), N_("n/a (Unknown)"), N_("Known to the ancients."), N_("German: bisemutum (white mass), Now spelled wismut."), 15, 6, 0x7FFFFFFF, 0x0, 0xC000, 0xFFFF},
  {N_("Polonium"), "Po", "84", "208.9824", "9.196-9.32", "527", "1235", "176", "146", "(+6e) 67", "22.7", "0.125", "10", "102.9", N_("n/a"), N_("n/a"), "2.0-2.3", "813.1", "-2, 2, 4, 6", "[Xe] 4f&#185;&#8308; 5d&#185;&#8304; 6s&#178; 6p&#8308;", N_("CUB"), "3.350", N_("n/a"), N_("Silvery-gray metal"), N_("1898 (France/Poland)"), N_("Pierre and Marie Curie-Sklodowska"), N_("Named for Poland, native country of Marie Curie."), 16, 6, 0x7FFFFFFF, 0xFFFF, 0x4444, 0xFFFF},
  {N_("Astatine"), "At", "85", "209.9871", "(6.4)", "503-575", "575-610", "145", "(145)", "(+7e) 62", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), "(195)", "2.2-2.5", "916.3", "7, 5, 3, 1, -1", "[Xe] 4f&#185;&#8308; 5d&#185;&#8304; 6s&#178; 6p&#8309;", N_("FCC"), N_("n/a"), N_("n/a"), N_("Unstable, radioactive halogen"), N_("1940 (United States)"), N_("D.R.Corson, K.R.MacKenzie, E. Segre"), N_("Greek: astatos (unstable)."), 17, 6, 0x7FFFFFFF, 0xFFFF, 0x4444, 0xFFFF},
  {N_("Radon"), "Rn", "86", "222.0176", N_("4.4 (@ -62&#176;C)"), "202", "211.4", "214", "140-150", N_("n/a"), N_("n/a"), "0.094", "2.7", "18.1", "0.0036", N_("n/a"), N_("n/a"), "1036.5", "2, 4, 6, 8", "[Xe] 4f&#185;&#8308; 5d&#185;&#8304; 6s&#178; 6p&#8310;", N_("FCC"), N_("n/a"), N_("n/a"), N_("Heavy radioactive gas"), N_("1898 (Germany/England)"), N_("Fredrich Ernst Dorn, Ernest Rutherford"), N_("Variation of the name of another element, radium."), 18, 6, 0x7FFFFFFF, 0xC000, 0x7000, 0x0},

  {N_("Francium"), "Fr", "87", "223.0197", "1.87", "291-300", "913-950", N_("n/a"), N_("n/a"), "(+1e) 180", N_("n/a"), N_("n/a"), "~2-15", "~65", N_("n/a"), N_("n/a"), "0.7", "380", "1, 2", "[Rn] 7s&#185;", N_("BCC"), N_("n/a"), N_("n/a"), N_("n/a"), N_("1939 (France)"), N_("Marguerite Derey"), N_("Named for France, the nation of its discovery."), 1, 7, 0x7FFFFFFF, 0x4444, 0x4444, 0xFFFF},
  {N_("Radium"), "Ra", "88", "226.0254", N_("5.5 (@ +20&#176;C)"), "973", "1413-2010", N_("n/a"), N_("n/a"), "(+2e) 143", "45.0", "0.120", "8.5-(9.6)", "113", "(18.6)", N_("n/a"), "0.9", "509.3; 979.0", "2", "[Rn] 7s&#178;", N_("BCC"), N_("n/a"), N_("n/a"), N_("Silvery white, radioactive element"), N_("1898 (France/Poland)"), N_("Pierre and Marie Curie-Sklodowska"), N_("Latin: radius (ray)."), 2, 7, 0x7FFFFFFF, 0xFFFF, 0x4444, 0x4444},

  {N_("Actinium"), "Ac", "89", "227.0278", "10.07", "1320", "3470", "188", N_("n/a"), "(+3e) 118", "22.54", N_("n/a"), "(10.5)", "(292.9)", N_("n/a"), N_("n/a"), "1.1", "665.5", "3", "[Rn] 6d&#185; 7s&#178;", N_("FCC"), "5.310", N_("n/a"), N_("Heavy, Silvery-white metal that is very radioactive"), N_("1899 (France)"), N_("Andre-Louis Debierne"), N_("Greek: akis, aktinos (ray)."), 3, 10, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Thorium"), "Th", "90", "232.0381", "11.78", "2028", "5060", "180", "165", "(+4e) 102", "19.8", "0.113", "16.11", "513.7", "(54.0)", "100.00", "1.3", "670.4", "4", "[Rn] 6d&#178; 7s&#178;", N_("FCC"), "5.080", N_("n/a"), N_("Gray, soft, malleable, ductile, radioactive metal"), N_("1828 (Sweden)"), N_("Jons Jacob Berzelius"), N_("Named for Thor, Norse god of thunder."), 4, 10, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Protactinium"), "Pa", "91", "231.03588", "15.37", "2113", "4300", "161", N_("n/a"), "(+5e) 89 (+3e) 113", "15.0", "0.121", "16.7", "481.2", N_("n/a"), N_("n/a"), "1.5", "0.0", "5, 4", "[Rn] 5f&#178; 6d&#185; 7s&#178;", N_("TET"), "3.920", N_("n/a"), N_("Silvery-white, radioactive metal"), N_("1918 (England/France)"), N_("Fredrich Soddy, John Cranston, Otto Hahn, Lise Meitner"), N_("Greek: proto and actinium (parent of actinium); it forms actinium when it radioactively decays."), 5, 10, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Uranium"), "U", "92", "238.0289", "19.05", "1405.5", "4018", "138", "142", "(+6e) 80 (+4e) 97", "12.5", "0.115", "12.6", "417", "27.5", N_("n/a"), "1.38", "686.4", "6, 5, 4, 3", "[Rn] 5f&#179; 6d&#185; 7s&#178;", N_("ORC"), "2.850", N_("n/a"), N_("Silvery-white, dense, ductile and malleable, radioactive metal."), N_("1789 (Germany)"), N_("Martin Klaproth"), N_("Named for the planet Uranus."), 6, 10, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Neptunium"), "Np", "93", "237.048", "20.25", "913", "4175", "130", N_("n/a"), "(+4e) 95 (+3e) 110", "21.1", N_("n/a"), "(9.6)", "336", "(6.3)", N_("n/a"), "1.36", "0.0", "7, 6, 5, 4, 3", "[Rn] 5f&#8308; 6d&#185; 7s&#178;", N_("ORC"), "4.720", N_("n/a"), N_("Silvery metal"), "1940 (United States)", N_("E.M. McMillan, P.H. Abelson"), N_("Named for the planet Neptune."), 7, 10, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Plutonium"), "Pu", "94", "244.0642", "19.84", "641-912.85", "3505-3508.15", "151-162", N_("n/a"), N_("71-100 or (+4e) 93 (+3e) 108"), N_("n/a"), N_("n/a"), "2.8", "343.5", "(6.7)", N_("n/a"), "1.28", "491.9-584.7", "2, 3, 4, 5, 6, 7", "[Rn] 5f&#8310; 7s&#178;", N_(N_("MCL")), N_("n/a"), N_("n/a"), N_("Silvery-white, radioactive metal"), N_("1940 (United States)"), N_("G.T.Seaborg, J.W.Kennedy, E.M.McMillan, A.C.Wohl"), N_("Named for the planet Pluto."), 8, 10, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Americium"), "Am", "95", "243.0614", "13.67", "1267-1448", "2880-2880.15", "173", N_("n/a"), "(+4e) 92 (+3e) 107", "20.8", N_("n/a"), "(10.0)", "238.5", N_("n/a"), N_("n/a"), "1.3", "0.0", "6, 5, 4, 3", "[Rn] 5f&#8311; 7s&#178;", N_("HEX"), N_("n/a"), N_("n/a"), N_("Silvery-white, radioactive metal"), N_("1944 (United States)"), N_("G.T.Seaborg, R.A.James, L.O.Morgan, A.Ghiorso"), N_("Named for the American continent, by analogy with europium."), 9, 10, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Curium"), "Cm", "96", "247.0703", "13.51", "1340-1613", "3383", "299", N_("n/a"), N_("n/a"), "18.28", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), "1.3", "581", "4, 3", "[Rn] 5f&#8311; 6d&#185; 7s&#178;", N_("HEX"), N_("n/a"), N_("n/a"), N_("Silvery, malleable, synthetic radioactive metal"), N_("1944 (United States)"), N_("G.T.Seaborg, R.A.James, A.Ghiorso"), N_("Named in honor of Pierre and Marie Curie."), 10, 10, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Berkelium"), "Bk", "97", "247.0703", "13.25", "1259", "2900", "297", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), "1.3", "(600)", "4, 3", "[Rn] 5f&#8313; 7s&#178;", N_("HEX"), N_("n/a"), N_("n/a"), N_("Radioactive synthetic metal"), N_("1949 (United States)"), N_("G.T.Seaborg, S.G.Tompson, A.Ghiorso"), N_("Named after Berkeley, California the city of its discovery."), 11, 10, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Californium"), "Cf", "98", "251.0796", "15.1", "900-1173.15", "(1743)", "295", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), "1.3", "(610)", "2, 3, 4", "[Rn] 5f&#185;&#8304; 7s&#178;", N_("HEX"), N_("n/a"), N_("n/a"), N_("Powerful neutron emitter"), N_("1950 (United States)"), N_("G.T.Seaborg, S.G.Tompson, A.Ghiorso, K.Street Jr."), N_("Named after the state and University of California."), 12, 10, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Einsteinium"), "Es", "99", "252.083", "13.5", "860", "1130", "292", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), "1.3", "619", "2, 3, 4", "[Rn] 5f&#185;&#185; 7s&#178;", N_("n/a"), N_("n/a"), N_("n/a"), N_("Radioactive, synthetic metal"), N_("1952 (United States)"), N_("Argonne, Los Alamos, University of California"), N_("Named in honor of the scientist Albert Einstein."), 13, 10, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Fermium"), "Fm", "100", "257.0951", N_("n/a"), "1800", N_("n/a"), "290", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), "1.3", "627-630", "3", "[Rn] 5f&#185;&#178; 7s&#178;", N_("n/a"), N_("n/a"), N_("n/a"), N_("Radioactive, synthetic metal"), N_("1952 (United States)"), N_("Argonne, Los Alamos, University of California"), N_("Named in honor of the scientist Enrico Fermi."), 14, 10, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Mendelevium"), "Md", "101", "258.1", N_("n/a"), "1100", N_("n/a"), "287", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), "1.3", "635", "1, 2, 3", "[Rn] 5f&#185;&#179; 7s&#178;", N_("n/a"), N_("n/a"), N_("n/a"), N_("Radioactive, synthetic metal"), N_("1955 (United States)"), N_("G.T.Seaborg, S.G.Tompson, A.Ghiorso, K.Street Jr."), N_("Named in honor of the scientist Dmitri Ivanovitch Mendeleyev, who devised the periodic table."), 15, 10, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Nobelium"), "No", "102", "259.1009", N_("n/a"), "1100", N_("n/a"), "285", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), "1.3", "640", "3, 2", "[Rn] 5f&#185;&#8308; 7s&#178;", N_("n/a"), N_("n/a"), N_("n/a"), N_("Radioactive, synthetic metal"), N_("1963 (USSR)"), N_("Dubna"), N_("Named in honor of Alfred Nobel, who invented dynamite and founded Nobel prize."), 16, 10, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},

  {N_("Lawrencium"), "Lr", "103", "262.11", N_("n/a"), "1900", N_("n/a"), "282", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), "1.3", "470", "3", "[Rn] 5f&#185;&#8308; 6d&#185; 7s&#178;", N_("n/a"), N_("n/a"), N_("n/a"), N_("Radioactive, synthetic metal"), N_("1961 (USSR/United States)"), N_("Soviet Nuclear Research/University of California at Berkeley"), N_("Named in honor of Ernest O. Lawrence, inventor of the cyclotron."), 3, 7, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Rutherfordium"), "Rf", "104", "261", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), "4", "[Rn] 5f&#185;&#8308; 6d&#178; 7s&#178;", N_("n/a"), N_("n/a"), N_("n/a"), N_("Radioactive, synthetic metal"), N_("1964 (USSR)"), N_("Dubna"), N_("Named in honor of Ernest Rutherford"), 4, 7, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Dubnium"), "Db", "105", "268", "26.1", N_("n/a"), N_("n/a"), "139", "66", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), "2, 3, 4, 5", "[Rn] 5f&#185;&#8308; 6d&#179; 7s&#178;", N_("n/a"), N_("n/a"), N_("n/a"), N_("Radioactive, synthetic metal"), N_("1970 (USSR/United States)"), N_("Soviet Nuclear Research/University of California at Berkeley"), N_("Dubna"), 5, 7, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Seaborgium"), "Sg", "106", "[271]", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), "[Rn] 5f&#185;&#8308; 6d&#8308; 7s&#178;", N_("n/a"), N_("n/a"), N_("n/a"), N_("Radioactive, synthetic metal"), N_("1974 (USSR/United States)"), N_("Soviet Nuclear Research/University of California at Berkeley"), N_("Named in honor of Glenn Seaborg, American physical chemist known for research on transuranium elements."), 6, 7, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Bohrium"), "Bh", "107", "[267]", N_("n/a"), N_("n/a"), N_("n/a"), "(128)", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), "(660)", N_("n/a"), "[Rn] 5f&#185;&#8308; 6d&#8309; 7s&#178;", N_("n/a"), N_("n/a"), N_("n/a"), N_("Radioactive, synthetic metal"), N_("1976 (Germany)"), N_("Heavy Ion Research Laboratory (HIRL)"), N_("Named in honor of Niels Bohr"), 7, 7, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Hassium"), "Hs", "108", "[269]", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), "[Rn] 5f&#185;&#8308; 6d&#8310; 7s&#178;", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("1984 (Germany)"), N_("Heavy Ion Research Laboratory (HIRL)"), N_("Named in honor of Hessen Land, Germany. Latin: Hassia."), 8, 7, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Meitnerium"), "Mt", "109", "[278]", "37.4", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), "9, 8, 6, 4, 3, 1", "[Rn] 5f&#185;&#8308; 6d&#8311; 7s&#178;", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("1982 (Germany)"), N_("Heavy Ion Research Laboratory (HIRL)"), N_("Named in honor of Lise Meitner"), 9, 7, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA}, 
  {N_("Darmstadtium"), "Ds", "110", "[281]", "(34.8)", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), "[Rn] 5f&#185;&#8308; 6d&#8313; 7s&#185;/[Rn] 5f&#185;&#8308; 6d&#8312; 7s&#178;", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("1994 (Germany)"), N_("Heavy Ion Research Laboratory (HIRL)"), N_("Named after Darmstadt, Germany, the city of its discovery."), 10, 7, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA}, 
  {N_("Roentgenium"), "Rg", "111", "[281]", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), "[Rn] 5f&#185;&#8308; 6d&#185;&#8304; 7s&#185;", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("1994 (Germany)"), N_("Heavy Ion Research Laboratory (HIRL)"), N_("Named in honor of physicist Wilhelm Conrad Roentgen."), 11, 7, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA},
  {N_("Copernicium"), "Cn", "112", "285", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), "[Rn] 5f&#185;&#8308; 6d&#185;&#8304; 7s&#178;", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("1996 (Germany)"), N_("Heavy Ion Research Laboratory (HIRL)"), N_("Named in honor of Nicolaus Copernicus"), 12, 7, 0x7FFFFFFF, 0xFFFF, 0xFFFF, 0xAAAA}, 
  {N_("Ununtrium"), "Uut", "113", "(286)", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), "[Rn] 5f&#185;&#8308; 6d&#185;&#8304; 7s&#178; 7p&#185;", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("2004 (Russia/United States)"), N_("Dubna/Livermore"), N_("Un (one) un (one) trium (three)"), 13, 7, 0x7FFFFFFF, 0xC000, 0xC000, 0xC000}, 
  {N_("Flerovium"), "Fl", "114", "[289]", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), "4, 2", "[Rn] 5f&#185;&#8308; 6d&#185;&#8304; 7s&#178; 7p&#178;", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("1998 (Russia/United States)"), N_("Dubna/Livermore"), N_("Named in honor of Soviet physicist Flerov"), 14, 7, 0x7FFFFFFF, 0xC000, 0xC000, 0xC000},
  {N_("Ununpentium"), "Uup", "115", "288", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), "[Rn] 5f&#185;&#8308; 6d&#185;&#8304; 7s&#178; 7p&#179;", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("2003 (Russia/United States)"), N_("Dubna/Livermore"), N_("Un (one) un (one) pentium (five)"), 15, 7, 0x7FFFFFFF, 0xC000, 0xC000, 0xC000}, 
  {N_("Livermorium"), "Lv", "116", "[193]", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), "-2, 2, 4, 6", "[Rn] 5f&#185;&#8308; 6d&#185;&#8304; 7s&#178; 7p&#8308;", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("2000 (Russia/United States)"), N_("Dubna, Dimitrovgrad, Lesnoy/Livermore"), N_("Named in honor of Livermore"), 16, 7, 0x7FFFFFFF, 0xC000, 0xC000, 0xC000},
  {N_("Ununseptium"), "Uus", "117", "[294]", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), "[Rn] 5f&#185;&#8308; 6d&#185;&#8304; 7s&#178; 7p&#8309;", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("2009 (Russia/United States)"), N_("Dubna/Livermore"), N_("Un (one) un (one) septium (seven)"), 17, 7, 0x7FFFFFFF, 0xC000, 0xC000, 0xC000}, 
  {N_("Ununoctium"), "Uuo", "118", "[294]", "13.65", N_("n/a"), "350&#177;30", "152", "230", N_("n/a"), N_("n/a"), N_("n/a"), "23.5", "19.4", N_("n/a"), N_("n/a"), N_("n/a"), "975&#177;155", "-1, 0, 1, 2, 4, 6, 8", "[Rn] 5f&#185;&#8308; 6d&#185;&#8304; 7s&#178; 7p&#8310;", N_("n/a"), N_("n/a"), N_("n/a"), N_("n/a"), N_("2006 (Russia/United States)"), N_("Dubna/Livermore"), N_("Un (one) un (one) octium (eight)"), 18, 7, 0x7FFFFFFF, 0xC000, 0xC000, 0xC000},
  {NULL}
 };


void dump_table(void)
{
  struct table_entry *p = table;
  int i;
  while(p->info[NAME]) {
    /*      1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 */
    printf("%s,%s,%s,%s,%s,%s,%s,%d,%d,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,0x%08X,0x%08X,0x%08X,0x%08X\n ",
      p->info[NAME],                 /*  1 */
      p->info[SYMBOL],               /*  2 */
      p->info[NUMBER],               /*  3 */
      p->info[WEIGHT],               /*  4 */
      p->info[DENSITY],              /*  5 */
      p->info[MELTING],              /*  6 */
      p->info[BOILING],              /*  7 */
      p->info[ATOMIC_RADIUS],        /*  8 */
      p->info[COVALENT_RADIUS],      /*  9 */
      p->info[IONIC_RADIUS],         /* 10 */
      p->info[ATOMIC_VOLUME],        /* 11 */
      p->info[SPECIFIC_HEAT],        /* 12 */
      p->info[FUSION_HEAT],          /* 13 */
      p->info[EVAPORATION_HEAT],     /* 14 */
      p->info[TERMAL_CONDUCTIVITY],  /* 15 */
      p->info[PAULING],              /* 16 */
      p->info[ENERGY],               /* 17 */
      p->info[OXIDATION],            /* 18 */
      p->info[CONFIGURATION],        /* 19 */
      p->info[LATTICE],              /* 20 */
      p->info[LATTICE_CONSTANT],     /* 21 */
      p->info[LATTICE_CA_RATIO],     /* 22 */
      p->info[APPEARANCE],           /* 23 */
      p->info[DISCOVERY_DATE],       /* 24 */
      p->info[DISCOVERED_BY],        /* 25 */
      p->info[NAMED_AFTER],          /* 26 */
      p->x,                          /* 27 */
      p->y,                          /* 28 */
      p->color.pixel,                /* 29 */
      p->color.red,                  /* 30 */
      p->color.green,                /* 31 */
      p->color.blue                  /* 32 */
    );
    ++p;
  }
}

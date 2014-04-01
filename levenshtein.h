/*
  Levenshtein.c v2003-05-10
  Python extension computing Levenshtein distances, string similarities,
  median strings and other goodies.

  Copyright (C) 2002-2003 David Necas (Yeti) <yeti@physics.muni.cz>.

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the Free
  Software Foundation; either version 2 of the License, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
*/

/*
 * Levenshtein distance between string1 and string2.
 *
 * Replace cost is normally 1, and 2 with nonzero xcost.
 */

#include <string>


using namespace std;


size_t lev_distance_wstr(size_t len1, wchar_t* string1, 
						 size_t len2, wchar_t* string2);
size_t lev_distance(size_t len1, const char* string1, 
                    size_t len2, const char* string2);
int get_levenshtein_distance(string str1, string str2, double* ratio = NULL);
int convert_mbs_to_wcs(string str, wchar_t* wcs);
int get_ucs4_levenshtein_distance(string str1, string str2, double* ratio = NULL);

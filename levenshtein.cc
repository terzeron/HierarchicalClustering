#include <iostream>
#include <cerrno>
#include <string.h>

#include "common.h"


using namespace std;


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
size_t lev_distance_wstr(size_t len1, wchar_t *string1, 
						 size_t len2, wchar_t *string2)
{
    size_t i;
    size_t *row;		/* we only need to keep one row of costs */
    size_t *end;
    size_t half;

    /* strip common prefix */
    while (len1 > 0 && len2 > 0 && *string1 == *string2) {
		len1--;
		len2--;
		string1++;
		string2++;
    }

    /* strip common suffix */
    while (len1 > 0 && len2 > 0 && string1[len1 - 1] == string2[len2 - 1]) {
		len1--;
		len2--;
    }

    /* catch trivial cases */
    if (len1 == 0)
		return len2;
    if (len2 == 0)
		return len1;

    /* make the inner cycle (i.e. string2) the longer one */
    if (len1 > len2) {
		size_t nx = len1;
		wchar_t *sx = string1;
		len1 = len2;
		len2 = nx;
		string1 = string2;
		string2 = sx;
    }
    /* check len1 == 1 separately */
    if (len1 == 1) {
        return len2 - (memchr(string2, *string1, len2) != NULL);
    }
    len1++;
    len2++;
    half = len1 >> 1;

    /* initalize first row */
    row = (size_t *) malloc(len2 * sizeof(size_t));
    if (!row)
		return (size_t) (-1);
    end = row + len2 - 1;
    for (i = 0; i < len2 - half; i++)
		row[i] = i;

    /* go through the matrix and compute the costs.  yes, this is an extremely
     * obfuscated version, but also extremely memory-conservative and
     * relatively fast.
     */

    /* in this case we don't have to scan two corner triangles 
     * (of size len1/2)
     * in the matrix because no best path can go throught them. note this
     * breaks when len1 == len2 == 2 so the memchr() special case above is
     * necessary */
    row[0] = len1 - half - 1;
    for (i = 1; i < len1; i++) {
        size_t *p;
        wchar_t char1 = string1[i - 1];
        wchar_t *char2p;
        size_t D, x;
        /* skip the upper triangle */
        if (i >= len1 - half) {
            size_t offset = i - (len1 - half);
            size_t c3;
            
            char2p = string2 + offset;
            p = row + offset;
            c3 = *(p++) + (char1 != *(char2p++));
            x = *p;
            x++;
            D = x;
            if (x > c3)
                x = c3;
            *(p++) = x;
        } else {
            p = row + 1;
            char2p = string2;
            D = x = i;
        }
        /* skip the lower triangle */
        if (i <= half + 1)
            end = row + len2 + i - half - 2;
        /* main */
        while (p <= end) {
            size_t c3 = --D + (char1 != *(char2p++));
            x++;
            if (x > c3)
                x = c3;
            D = *p;
            D++;
            if (x > D)
                x = D;
            *(p++) = x;
        }
        /* lower triangle sentinel */
        if (i <= half) {
            size_t c3 = --D + (char1 != *char2p);
            x++;
            if (x > c3)
                x = c3;
            *p = x;
        }
    }

    i = *end;
    free(row);
    return i;
}


size_t lev_distance(size_t len1, const char* string1,
					size_t len2, const char* string2)
{
    size_t i;
    size_t *row;		/* we only need to keep one row of costs */
    size_t *end;
    size_t half;

    /* strip common prefix */
    while (len1 > 0 && len2 > 0 && *string1 == *string2) {
		len1--;
		len2--;
		string1++;
		string2++;
    }

    /* strip common suffix */
    while (len1 > 0 && len2 > 0 && string1[len1 - 1] == string2[len2 - 1]) {
		len1--;
		len2--;
    }

    /* catch trivial cases */
    if (len1 == 0)
		return len2;
    if (len2 == 0)
		return len1;

    /* make the inner cycle (i.e. string2) the longer one */
    if (len1 > len2) {
		size_t nx = len1;
		const char* sx = string1;
		len1 = len2;
		len2 = nx;
		string1 = string2;
		string2 = sx;
    }
    /* check len1 == 1 separately */
    if (len1 == 1) {
        return len2 - (memchr(string2, *string1, len2) != NULL);
    }
    len1++;
    len2++;
    half = len1 >> 1;

    /* initalize first row */
    row = (size_t *) malloc(len2 * sizeof(size_t));
    if (!row)
		return (size_t) (-1);
    end = row + len2 - 1;
    for (i = 0; i < len2 - half; i++)
		row[i] = i;

    /* go through the matrix and compute the costs.  yes, this is an extremely
     * obfuscated version, but also extremely memory-conservative and
     * relatively fast.
     */

    /* in this case we don't have to scan two corner triangles 
     * (of size len1/2)
     * in the matrix because no best path can go throught them. note this
     * breaks when len1 == len2 == 2 so the memchr() special case above is
     * necessary */
    row[0] = len1 - half - 1;
    for (i = 1; i < len1; i++) {
        size_t *p;
        char char1 = string1[i - 1];
        const char* char2p;
        size_t D, x;
        /* skip the upper triangle */
        if (i >= len1 - half) {
            size_t offset = i - (len1 - half);
            size_t c3;
            
            char2p = string2 + offset;
            p = row + offset;
            c3 = *(p++) + (char1 != *(char2p++));
            x = *p;
            x++;
            D = x;
            if (x > c3)
                x = c3;
            *(p++) = x;
        } else {
            p = row + 1;
            char2p = string2;
            D = x = i;
        }
        /* skip the lower triangle */
        if (i <= half + 1)
            end = row + len2 + i - half - 2;
        /* main */
        while (p <= end) {
            size_t c3 = --D + (char1 != *(char2p++));
            x++;
            if (x > c3)
                x = c3;
            D = *p;
            D++;
            if (x > D)
                x = D;
            *(p++) = x;
        }
        /* lower triangle sentinel */
        if (i <= half) {
            size_t c3 = --D + (char1 != *char2p);
            x++;
            if (x > c3)
                x = c3;
            *p = x;
        }
    }

    i = *end;
    free(row);
    return i;
}


int get_levenshtein_distance(string str1, string str2, double* ratio = NULL)
{
    uint len1 = strlen(str1.c_str());
    uint len2 = strlen(str2.c_str());
    uint max_len = max(len1, len2);
    uint ld = lev_distance(len1, str1.c_str(), len2, str2.c_str());

#ifdef VERBOSE0
    cout << "str1=" << str1 << ", str2=" << str2 << ", len1=" << len1 << ", len2=" << len2 << endl;
    cout.flush();
#endif // VERBOSE0

    if (ratio != NULL) {
        *ratio = 1.0 - double(ld) / double(max_len);
        if (*ratio > 0.33) {
            if (len1 >= 2 && len2 >= 2) {
                if (max_len - ld > 1) {
                    *ratio = *ratio + 0.05 * double(max_len - ld) / max_len;
                }
            }
            if (len1 < 3 && len2 < 3) {
                *ratio /= 2;
            }
        }
#ifdef VERBOSE
        if (*ratio > 0.0) {
            cout << str1 << "\t" << str2 << "\t" << *ratio << endl;
            cout.flush();
        }
#endif // VERBOSE
    }

    return ld;
}


int convert_mbs_to_wcs(string str, wchar_t* wcs)
{
    int ret = mbstowcs(wcs, str.c_str(), str.length());
    if (ret < 0) {
        cerr << "can't convert multibyte string to wide string, "
             << errno << ": " << strerror(errno) << endl;
        return -1;
    }
    wcs[ret] = 0;
    return 0;
}


int get_ucs4_levenshtein_distance(string str1, string str2, 
                                  double* ratio = NULL)
{
    wchar_t wcs1[MAX_LINE_LEN];
    wchar_t wcs2[MAX_LINE_LEN];

    if (convert_mbs_to_wcs(str1, wcs1) < 0) {
        return -1;
    }
    if (convert_mbs_to_wcs(str2, wcs2) < 0) {
        return -1;
    }
    uint len1 = wcslen(wcs1);
    uint len2 = wcslen(wcs2);
    uint max_len = max(len1, len2);

    uint ld = lev_distance_wstr(len1, wcs1, len2, wcs2);

#ifdef VERBOSE0
    cout << "str1=" << str1 << ", str2=" << str2 << ", len1=" << len1 << ", len2=" << len2 << endl;
    cout.flush();
#endif // VERBOSE0

    if (ratio != NULL) {
        *ratio = 1.0 - double(ld) / double(max_len);
        if (*ratio > 0.33) {
            if (len1 >= 2 && len2 >= 2) {
                if (max_len - ld > 1) {
                    *ratio = *ratio + 0.05 * double(max_len - ld) / max_len;
                }
            }
            if (len1 < 3 && len2 < 3) {
                *ratio /= 2;
            }
        }
#ifdef VERBOSE
        if (*ratio > 0.0) {
            cout << str1 << "\t" << str2 << "\t" << *ratio << endl;
            cout.flush();
        }
#endif // VERBOSE
    }

    return ld;
}


#if 0
int main()
{

    char a[] = "한a빛a을";
    char b[] = "한a빛만을";
    wchar_t wa[1024];
    wchar_t wb[1024];
    int ret;
    
    setlocale(LC_CTYPE, "ko_KR.utf8");
    
    if ((ret = mbstowcs(wa, a, strlen(a))) < 0) {
        cerr << strerror(errno) << endl;
        return -1;
    }
    wa[ret] = 0;
    if ((ret = mbstowcs(wb, b, strlen(b))) < 0) {
        cerr << strerror(errno) << endl;
        return -1;
    }
    wb[ret] = 0;

    cout << lev_distance(wcslen(wa), wa, wcslen(wb), wb) << endl;
}
#endif // 0

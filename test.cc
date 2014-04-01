#include <iostream>
#include <locale>
#include <errno.h>
#include "util.h"
#include "levenshtein.h"


using namespace std;


const uint MAX_LINE_LEN = 4096;


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
    size_t len1 = wcslen(wcs1);
    size_t len2 = wcslen(wcs2);
    size_t max_len = max(len1, len2);

    size_t ld = lev_distance_wstr(len1, wcs1, len2, wcs2);

#ifdef VERBOSE0
    cout << "str1=" << str1 << ", str2=" << str2 << ", len1=" << len1 << ", len\
2=" << len2 << endl;
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


int main(void)
{
    setlocale(LC_CTYPE, "ko_KR.utf8");
    double similarity;
    get_ucs4_levenshtein_distance("스마프스마프동방신기", "스마스마동방신기", &similarity);
    cout << similarity << endl;
}

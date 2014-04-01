#include <iostream>
#include <locale>
#include <errno.h>
#include "util.h"
#include "levenshtein.h"


using namespace std;


double similarity_threshold = default_similarity_threshold;


int main(void)
{
    setlocale(LC_CTYPE, "ko_KR.utf8");
    double similarity;
    get_ucs4_levenshtein_distance("스마프스마프동방신기", "스마스마동방신기", &similarity);
    cout << similarity << endl;
}

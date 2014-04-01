#include <iostream>

#include "keyword_table.h"
#include "util.h"
#include "levenshtein.h"


using namespace std;


KeywordTable::KeywordTable()
{
}


KeywordTable::~KeywordTable()
{
    keyword_list.clear();
    keyword_similarity_map.clear();
}


vector<string>* KeywordTable::get_keyword_vector()
{
	return &keyword_list;
}


string& KeywordTable::get_keyword(uint idx) 
{
    return keyword_list[idx]; 
}


uint KeywordTable::get_num_keywords(void) { 
    return keyword_list.size(); 
}


uint KeywordTable::get_num_cluster_pairs(void)
{
    return keyword_similarity_map.size(); 
}


double KeywordTable::get(string& keyword1, string& keyword2) 
{
    string key = keyword1 + "\t" + keyword2;
    unordered_map<string, double, string_hash_t>::iterator iter =
        keyword_similarity_map.find(key);
    if (iter != keyword_similarity_map.end()) {
        return iter->second;
    }
    return 0.0;
}


void KeywordTable::set(string& keyword1, string& keyword2, double similarity) 
{
    string key = keyword1 + "\t" + keyword2;
    keyword_similarity_map.insert(make_pair(key, similarity));
    key = keyword2 + "\t" + keyword1;
    keyword_similarity_map.insert(make_pair(key, similarity));
#ifdef VERBOSE
    cout << key << "\t" << similarity << endl;
    cout.flush();
#endif // VERBOSE
}


void KeywordTable::print_similarity(void) 
{
    unordered_map<string, double, string_hash_t>::iterator iter;
    for (iter = keyword_similarity_map.begin();
         iter != keyword_similarity_map.end(); ++iter) {
        cout << iter->first << "\t" << iter->second << endl;
    }
}


int KeywordTable::make_similarity_map(void)
{
#ifdef VERBOSE
    cout << "KeywordTable::make_similarity_map()" << endl;
    cout.flush();
#endif // VERBOSE

    for (uint i = 0; i < get_num_keywords(); ++i) {
        for (uint j = i + 1; j < get_num_keywords(); ++j) {
#ifdef VERBOSE
            cout << "i=" << i << "\tj=" << j << endl;
            cout.flush();
#endif // VERBOSE
            string keyword1 = get_keyword(i);
            string keyword2 = get_keyword(j);
            char clean_keyword1[MAX_LINE_LEN];
            char clean_keyword2[MAX_LINE_LEN];
            strcpy(clean_keyword1, keyword1.c_str());
            strcpy(clean_keyword2, keyword2.c_str());
            remove_whitespace(clean_keyword1);
            remove_whitespace(clean_keyword2);
            tolower_str(clean_keyword1);
            tolower_str(clean_keyword2);
            double similarity = 0.0;

            get_levenshtein_distance(clean_keyword1, clean_keyword2, &similarity);
#ifdef VERBOSE
            cout << clean_keyword1 << "\t" << clean_keyword2 << "\t" 
                 << similarity << endl;
            cout.flush();
#endif // VERBOSE
            if (similarity == 0.0) {
                continue;
            }
            set(keyword1, keyword2, similarity);
        }
    }
    
    return 0;
}


double KeywordTable::get_cluster_similarity(Cluster* cluster1, Cluster* cluster2)
{
    double max_similarity = 0.0;
	
    for (uint i = 0; i < cluster1->get_num_keywords(); ++i) {
        for (uint j = 0; j < cluster2->get_num_keywords(); ++j) {
            double similarity = get(cluster1->get_keyword(i), cluster2->get_keyword(j));
            if (max_similarity < similarity) {
                max_similarity = similarity;
            }
        }
    }

    return max_similarity;
}



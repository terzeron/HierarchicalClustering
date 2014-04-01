#ifndef _KEYWORD_TABLE_H_
#define _KEYWORD_TABLE_H_


#include <iostream>
#include <vector>
#include <unordered_map>
#include <sys/types.h>

#include "common.h"
#include "cluster.h"


using namespace std;


class KeywordTable
{
private:
    vector<string> keyword_list;
    unordered_map<string, double, string_hash_t> keyword_similarity_map;

public:
    KeywordTable();
    ~KeywordTable();
	vector<string>* get_keyword_vector();
    string& get_keyword(uint idx);
    uint get_num_keywords(void);
    uint get_num_cluster_pairs(void);
    double get(string& keyword1, string& keyword2);
    void set(string& keyword1, string& keyword2, double similarity);
    void print_similarity(void);
	int make_similarity_map(void);
	double get_cluster_similarity(Cluster* cluster1, Cluster* cluster2);
};


#endif // _KEYWORD_TABLE_H_

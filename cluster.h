#ifndef _CLUSTER_H_
#define _CLUSTER_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <sys/types.h>

using namespace std;


class Cluster 
{
private:
    vector<string> keyword_list;
	vector<int> num_list;
public:
    Cluster();
    ~Cluster();
    Cluster(string& keyword, int num);
    void add(string& keyword, int num);
    uint get_num_keywords(void);
    void print(ofstream& fstrm);
    void print(void);
    string& get_keyword(uint idx);
	int get_num(uint idx);
};


const string result_dir_prefix = "result";

#endif // _CLUSTER_H_

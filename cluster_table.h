#ifndef _CLUSTER_TABLE_H_
#define _CLUSTER_TABLE_H_

#include "cluster.h"
#include "keyword_table.h"


class ClusterTable 
{
private:
    vector<Cluster*> cluster_list;
	KeywordTable* keyword_table;

public:
    ClusterTable(KeywordTable* keyword_table);
    ~ClusterTable();
	int make();
    void add(Cluster& cluster);
    void remove(Cluster* cluster);
    uint get_num_clusters(void);
    void print(ofstream& fstrm);
    Cluster* get_cluster(uint idx);
	pair<Cluster*, Cluster*> find_most_similar_pair();
	void merge_two_clusters(Cluster* cluster1, Cluster* cluster2);
};

#endif // _CLUSTER_TABLE_H_

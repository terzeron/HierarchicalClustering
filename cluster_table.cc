#include <iostream>
#include <fstream>

#include "levenshtein.h"
#include "cluster_table.h"
#include "util.h"


using namespace std;


ClusterTable::ClusterTable(KeywordTable* _keyword_table) 
{ 
#ifdef VERBOSE
    cout << "ClusterTable()" << endl;
    cout.flush();
#endif // VERBOSE

	cluster_list.clear();
	keyword_table = _keyword_table;
}


int ClusterTable::make()
{
    keyword_table->make_similarity_map();
    
    for (uint i = 0; i < keyword_table->get_num_keywords(); ++i) {
        Cluster cluster(keyword_table->get_keyword(i), i + 1);
        add(cluster);
    }

    while (get_num_clusters() > 1) {
#ifdef VERBOSE
        cout << "# of clusters: " << get_num_clusters() << endl;
        cout.flush();
#endif // VERBOSE
        pair<Cluster*, Cluster*> cluster_pair = find_most_similar_pair();
        if (cluster_pair.first == NULL && cluster_pair.second == NULL) {
            break;
        }
        merge_two_clusters(cluster_pair.first, cluster_pair.second);
    }
	
	return 0;
}


ClusterTable::~ClusterTable()
{
    cluster_list.clear();
}

void ClusterTable::add(Cluster& _cluster) 
{
    Cluster* cluster = new Cluster(_cluster);
    cluster_list.push_back(cluster); 
}


void ClusterTable::remove(Cluster* cluster) 
{
    vector<Cluster*>::iterator iter;
    for (iter = cluster_list.begin(); iter != cluster_list.end(); ++iter) {
        if (*iter == cluster) {
            break;
        }
    }
    cluster_list.erase(iter);
    delete cluster;
}


uint ClusterTable::get_num_clusters(void) 
{
    return cluster_list.size(); 
}


void ClusterTable::print(ofstream& fstrm) 
{
    for (uint i = 0; i < cluster_list.size(); ++i) {
		// cluster number
		fstrm << i << "\t";

        cluster_list[i]->print(fstrm);
        fstrm << endl;
    }
    fstrm << endl;
}


Cluster* ClusterTable::get_cluster(uint idx) 
{
    return cluster_list[idx]; 
}


pair<Cluster*, Cluster*> ClusterTable::find_most_similar_pair()
{
#ifdef VERBOSE
    cout << "find_most_similar_pair(keyword_table=" << &keyword_table << ")" << endl;
    cout.flush();
#endif // VERBOSE

    double max_similarity = 0.0;
    pair<Cluster*, Cluster*> cluster_pair;
    for (uint i = 0; i < get_num_clusters(); ++i) {
        for (uint j = i + 1; j < get_num_clusters(); ++j) {
            Cluster* cluster1 = get_cluster(i);
            Cluster* cluster2 = get_cluster(j);
            double similarity = keyword_table->get_cluster_similarity(cluster1, cluster2);
#ifdef VERBOSE
            if (similarity >= similarity_threshold) {
                cout << cluster1 << "( ";
                cluster1->print();
                cout << " ) + " << cluster2 << "( ";
                cluster2->print();
                cout << " )\tsimilarity=" << similarity << endl;
                cout.flush();
            }
#endif // VERBOSE
            if (similarity >= similarity_threshold &&
                max_similarity < similarity) {
                max_similarity = similarity;
                cluster_pair = make_pair(cluster1, cluster2);
            }
        }
    }

#ifdef VERBOSE
    cout << "max_similarity=" << max_similarity << endl;
    cout.flush();
#endif // VERBOSE
    return cluster_pair;
}


void ClusterTable::merge_two_clusters(Cluster* cluster1, Cluster* cluster2) 
{
#ifdef VERBOSE
    cout << "merge_two_clusters(cluster1=" << cluster1 << ", cluster2=" << cluster2 
         << ")" << endl;
    cout.flush();
#endif // VERBOSE

#ifdef VERBOSE
    cout << cluster1 << "( ";
    cluster1->print();
    cout << " ) + " << cluster2 << "( ";
    cluster2->print();
    cout << " )" << endl;
    cout.flush();
#endif // VERBOSE

    for (uint i = 0; i < cluster2->get_num_keywords(); ++i) {
        cluster1->add(cluster2->get_keyword(i), cluster2->get_num(i));
    }
    remove(cluster2);
}


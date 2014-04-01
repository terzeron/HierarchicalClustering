#include <iostream>
#include <fstream>

#include "cluster.h"


using namespace std;


Cluster::Cluster() 
{ 
}


Cluster::~Cluster()
{
    keyword_list.clear();
}


Cluster::Cluster(string& keyword, int num) 
{
    keyword_list.push_back(keyword);
	num_list.push_back(num);
}


void Cluster::add(string& keyword, int num) 
{
    keyword_list.push_back(keyword); 
	num_list.push_back(num);
}


uint Cluster::get_num_keywords(void) 
{
    return keyword_list.size(); 
}


void Cluster::print(ofstream& fstrm) 
{		
	// the size of this cluster
	fstrm << keyword_list.size() << "\t";

    for (uint i = 0; i < keyword_list.size(); ++i) {
        fstrm << num_list[i] << "\t" << keyword_list[i];
        if (i < keyword_list.size() - 1) {
            fstrm << "\t";
        }
    }
}


void Cluster::print(void) 
{
    for (uint i = 0; i < keyword_list.size(); ++i) {
        cout << keyword_list[i];
        if (i < keyword_list.size() - 1) {
            cout << "\t";
        }
    }
}


string& Cluster::get_keyword(uint idx) { 
    return keyword_list[idx]; 
}


int Cluster::get_num(uint idx) {
	return num_list[idx];
}

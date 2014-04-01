#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
//#include <ext/hash_fun.h>
#include <cerrno>
#include <dirent.h>
#include <getopt.h>

#include "common.h"
#include "keyword_table.h"
#include "cluster_table.h"
#include "util.h"


using namespace std;


//#define VERBOSE


double similarity_threshold = default_similarity_threshold;


void print_usage(string program_name)
{
    cout << "Usage:\t" << program_name << "\t-s <stop words file>" << endl
		 << "\t\t\t-t <similarity threshold>" << endl
		 << "\t\t\t<input file> <output file>" << endl;
    cout.flush();
}


class StopWordRemover {
private:
	unordered_set<string, string_hash_t> stop_words;
public:
	StopWordRemover(unordered_set<string, string_hash_t> _stop_words) { 
		stop_words = _stop_words;
	}
	void operator() (string& keyword) { 
		std::transform(keyword.begin(), keyword.end(), 
					   keyword.begin(), (int (*)(int))std::tolower);
		
        // remove stop words from original string
        for (unordered_set<string, string_hash_t>::iterator stop_iter = stop_words.begin();
			 stop_iter != stop_words.end(); ++stop_iter) {
			string stop_word = *stop_iter;
			while (1) {
				size_t found = keyword.find(stop_word);
				if (found != string::npos) {
					keyword.replace(found, stop_word.length(), " ");
				} else {
					break;
				}
			}
        }
	}
};


int read_input(string input_file_path, string stop_words_file_path, 
			   KeywordTable& keyword_table)
{
#ifdef VERBOSE
    cout << "read_input(input_file_path=" << input_file_path 
		 << ", keyword_table=" << &keyword_table << ")" << endl;
    cout.flush();
#endif // VERBOSE

	// make up the stop word set
	unordered_set<string, string_hash_t> stop_hs;
	if (stop_words_file_path != "") {
		read_file_into_unordered_set(stop_words_file_path, stop_hs);
	}

    ifstream input_fstrm;
	vector<string>* keyword_list = keyword_table.get_keyword_vector();
	StopWordRemover stop_words_remover(stop_hs);

	read_file_into_vector(input_file_path, *keyword_list);
#ifdef VERBOSE
	cout << keyword_list->size() << endl;
	cout << keyword_table.get_num_keywords() << endl;
#endif // VERBOSE

	for_each(keyword_list->begin(), keyword_list->end(), stop_words_remover);

    return 0;
}


int main(int argc, char* argv[])
{
#ifdef VERBOSE
    cout << "main(argc=" << argc << ", argv=" << argv << ")" << endl;
    cout.flush();
#endif // VERBOSE

	string stop_words_file_name = "";
    int c;
    int option_index = 0;
    const struct option long_options[] = {
        {"help", 0, 0, 'h'},
		{"stop_words_file", optional_argument, NULL, 's'},
		{"similarity_threshold", optional_argument, NULL, 't'},
		{ 0, 0, 0, 0 },
    };

    while (1) {
        c = getopt_long(argc, argv, "t:s:h", long_options, &option_index);
        if (c == -1)
            break;
        switch (c) {
		case 's':
			stop_words_file_name = optarg;
			break;
		case 't':
			similarity_threshold = atof(optarg);
			break;
        case 'h':
        default:
            print_usage(argv[0]);
            return -1;
        }
    }
    if (optind + 2 != argc) {
        print_usage(argv[0]);
        return -1;
    }
	
	string input_file_name = argv[optind];
    string output_file_name = argv[optind + 1];
#ifdef VERBOSE
	cout << "input_file_name=" << input_file_name << ", output_file_name=" << output_file_name 
		 << ", stop_words_file_name=" << stop_words_file_name << endl;
#endif // VERBOSE

    ofstream output_fstrm;
    string temp_output_file_name = "";
    
    setlocale(LC_CTYPE, "ko_KR.utf8");

	KeywordTable keyword_table;
	if (read_input(input_file_name, stop_words_file_name, keyword_table) < 0) {
		cerr << "can't read input from file" << endl;
		return -1;
	}

	ClusterTable cluster_table(&keyword_table);
	cluster_table.make();

    temp_output_file_name = output_file_name + ".temp";
    output_fstrm.open(temp_output_file_name.c_str());
    if (output_fstrm.is_open() == false) {
        cerr << "can't open '" << temp_output_file_name << "', " 
             << errno << ": " << strerror(errno) << endl;
        return -1;
    }
	cluster_table.print(output_fstrm);
    output_fstrm.close();
    
    if (rename(temp_output_file_name.c_str(), output_file_name.c_str()) < 0) {
        cerr << "can't rename '" << temp_output_file_name << "' to '"
             << output_file_name << "', " << errno << ": " << endl;
        return -1;
    }

    return 0;
}

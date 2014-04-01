#ifndef __COMMON_H__
#define __COMMON_H__

#include <string>
#include <unordered_map>
#include <sys/types.h>


using namespace std;


extern const uint MAX_LINE_LEN;
extern const double default_similarity_threshold;
extern double similarity_threshold;


typedef struct _string_hash {
    size_t operator()(const string& s) const {
        return hash<const char *>()(s.c_str());
    }
} string_hash_t;

#endif // __COMMON_H__

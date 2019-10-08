#ifndef _UTIL_H_
#define _UTIL_H_

/**
 * \file util.h
 * \brief utility function declarations
 */

#include <string>
#include <vector>
#include <unordered_set>

#include "common.h"


using namespace std;


int read_file_into_unordered_set(string& file_path, unordered_set<string>& hs);

int read_file_into_vector(string& file_path, vector<string>& v);

/**
 * \fn escape_xml(const char* s, char* d, int n)
 * \brief 
 */
int escape_xml(const char* s, char* d, int n);

/**
 * \fn escape_url(const char* s, char* d)
 * \brief 
 */
int escape_url(const char* s, char* d);

/** 
 * \fn replace_str(char* d, const char* s, const char* str1, const char* str2)
 * \brief
 */
int replace_str(char* d, const char* s, const char* str1, const char* str2);

/**
 * \fn trim_whitespace(char* s)
 * \brief
 */
int trim_whitespace(char* s);

/**
 * \fn remove_whitespace
 * \brief removes all whitespace characters in a string
 */
void remove_whitespace(char* s);
void remove_whitespace(const string& str, string& result);

/**
 * \fn tolower_str(
 * \brief converts the alphabets to the lowercase
 */
void tolower_str(char* s);

/**
 * \fn is_digit_string
 * \brief checks if a string is composed of only digit
 * \param s string
 * \return true or false
 */
bool is_digit_string(const char *s);
bool is_digit_string(const string& s);

/**
 * \fn is_in_time_intvl
 * \brief 
 * \return true or false
 */
bool is_in_time_intvl(const time_t start_time, const time_t end_time, 
		      const time_t t);
int get_hour_from_ts(const time_t ts);

bool is_date_string(const char *s);
bool is_date_string(const string& s);

time_t get_ts_from_HHMISS(const string& time_str);
time_t get_ts_from_HHMISS(const char* time_str);
time_t get_current_HHMISS_ts(void);
string get_HHMISS_from_ts(const time_t ts);

wstring mbs2wcs(string const& str, locale const& loc=locale(""));
string wcs2mbs(wstring const& str, locale const& loc=locale(""));

/**
 * \def uhc2wcs
 * \brief converts an UHC(CP949) string to a WCS wide string
 * \remarks null function 
 */
#if 0
wstring uhc2wcs(string const& str);
#else // 0
#define uhc2wcs(str) (str)
#endif // 0

/**
 * \def wcs2uhc
 * \brief converts a WCS wide string to an UHC(CP949) string
 * \remarks null function
 */
#if 0
string wcs2uhc(wstring const& str);
#else // 0
#define wcs2uhc(str) (str)
#endif // 0

#endif // _UTIL_H_

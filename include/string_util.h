#ifndef _STRING_UTIL_H_
#define _STRING_UTIL_H_

#include <string>

const std::string WHITESPACE = " \n\r\t\f\v";

std::string ltrim(const std::string& s);

std::string rtrim(const std::string& s);

std::string trim(const std::string& s);


#endif

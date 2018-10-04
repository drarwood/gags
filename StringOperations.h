#ifndef _STRINGOPERATIONS
#define _STRINGOPERATIONS

#include <string>
#include <vector>

double      stringToDouble(std::string);
int         stringToInt(std::string);
std::string intToString(int);
std::string doubleToString(double);

//void        split(std::string&, char, std::vector<std::string>&);
void        split(std::string&, std::string, std::vector<std::string>&);


//template<typename T>
//void  split(const std::basic_string<T>&, T, std::vector<std::basic_string<T> >&) {

#endif



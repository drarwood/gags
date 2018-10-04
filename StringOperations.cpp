// GAGS
// Developed by Andrew R Wood
// a.r.wood@exeter.ac.uk

#include <string>
#include <sstream>
#include <vector>
#include <functional>
#include <iostream>
#include "StringOperations.h"

using namespace std;


// function to convert string to double
double stringToDouble(string s) {
    double d;
    stringstream ss(s); //turn the string into a stream
    ss >> d; //convert
    return d;
}


// function to convert string to int
int stringToInt(string s) {
    int i;
    stringstream ss(s); //turn the string into a stream
    ss >> i; //convert
    return i;
}


// function to convert int to string
string intToString(int i) {
    string s;
    ostringstream convert;
    convert << i;
    s = convert.str();
    return s;
}


// function to convert double to string
string doubleToString(double d) {
    string s;
    ostringstream convert;
    convert << d;
    s = convert.str();
    return s;
}


// function to split a string based on a delimiter
//
//void split(string& s, char c, vector<string>& v) {
void split(string& s, string sc, vector<string>& v) {

    string::size_type i = 0;
    string::size_type j = s.find(sc);

    while (j != string::npos) {
        v.push_back(s.substr(i,j-i));
        i = ++j;
        j = s.find(sc,j);
        if (j == string::npos) {
            v.push_back(s.substr(i, s.length()));
        }
    }
    if (v.size() == 0) {
        v.push_back(s);
    }
}


/*
// function to split a string based on a delimiter
template<typename T>
void split(const basic_string<T>& s, T c,
           vector<basic_string<T> >& v) {
   basic_string<T>::size_type i = 0;
   basic_string<T>::size_type j = s.find(c);

   while (j != basic_string<T>::npos) {
      v.push_back(s.substr(i, j-i));
      i = ++j;
      j = s.find(c, j);

      if (j == basic_string<T>::npos)
         v.push_back(s.substr(i, s.length()));
   }
}

*/

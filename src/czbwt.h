#ifndef CZBWT_H_
#define CZBWT_H_

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <sstream>
#include <tuple>

using std::string;
using std::tuple;

tuple<int, string> encode(string);
string decode(int, string);

#endif
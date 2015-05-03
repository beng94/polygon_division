#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "data.hpp"

std::list<Point> gen_data (const int n);
void print_vector (const char* file, const std::vector<Point>& vec);
void print_list (const char* file, const std::list<Point>& list);

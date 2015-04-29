#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "data.hpp"

std::vector<Point> gen_data (const int n);
void print_vector (const char* file, const std::vector<Point>& vec);

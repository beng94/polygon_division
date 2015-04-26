#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <cstdlib>
#include <ctime>
#include "data.hpp"

bool valid_coo (std::vector<Coordinate> vec, Coordinate tst);
std::vector<Coordinate> gen_data (int n);
void write_data_vector (const char* file, std::vector<Coordinate>& vec);
void write_data_stack (const char* file, std::stack<Coordinate>& stack);


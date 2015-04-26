#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "data.hpp"

bool valid_coo (std::vector<Coordinate> vec, Coordinate tst);
std::vector<Coordinate> gen_data (int n);
void write_data (const char* file, std::vector<Coordinate>& vec);


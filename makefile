all:
	g++ -g -std=c++11 -o ALL \
	main.cpp \
	graham_scan.hpp graham_scan.cpp \
	test.hpp test.cpp \
	data.hpp data.cpp \
	polygon_division.hpp polygon_division.cpp \
	geometria.h geometria.cpp

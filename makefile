all:
	g++ -g -std=c++11 -o ALL \
	main.cpp \
	graham_scan.hpp graham_scan.cpp \
	test.hpp test.cpp \
	data.hpp data.cpp

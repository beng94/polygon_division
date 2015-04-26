#include "test.hpp"

bool valid_coo (std::vector<Coordinate> vec, Coordinate tst)
{
    for(int i = 0; i<vec.size(); i++)
    {
        if(vec[i] == tst) return false;
    }

    return true;
}

std::vector<Coordinate> gen_data(int n)
{
    std::vector<Coordinate> vec(n);
    int cnt = 0;

    std::srand(std::time(0));
    while(cnt != n)
    {
        int x = std::rand() % 20;
        int y = std::rand() % 20;

        Coordinate new_coo(x, y);

        if(valid_coo(vec, new_coo))
        {
            vec[cnt] = new_coo;
            cnt++;
        }
    }

    return vec;
}

void write_data (const char* file, std::vector<Coordinate>& vec)
{
    std::ofstream myfile;
    myfile.open(file);

    for(int i = 0; i<vec.size(); i++)
    {
        myfile << vec[i] << std::endl;
    }

    myfile.close();
}

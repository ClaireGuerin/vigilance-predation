#ifndef GRID_H
#define GRID_H
#include <vector>
using namespace std;

template <typename T>
class Grid { 

public:
    Grid(int); // create an empty vector of size edge size ^ 2
    Grid(int, T); // first par int = edge size, second par type T = init value
    vector<T> getGrid();
    int dim;
    
private:
    vector<T> linearGrid; // store the square grid info into a linear vector
};
 
#endif
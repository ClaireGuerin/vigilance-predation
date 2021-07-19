#ifndef GRID_H
#define GRID_H
#include <vector>
using namespace std;

template <typename T>
class Grid { 

public:
    
    int dim;

    Grid(int edgeSize) :    dim (edgeSize * edgeSize),
                            linResources (edgeSize * edgeSize),
                            linShare (dim) { 
        // create an empty vector of size edge size ^ 2
    }

    Grid(int edgeSize, T initResources) :   dim (edgeSize * edgeSize),
                                            linResources (dim, initResources), 
                                            linShare (dim) { 
        // first par int = edge size, second par type T = init value
    }

    vector<T> resourceDistri() {
        return linResources;
    }

    vector<T> shareDistri() {
        return linResources;
    }
    
private:
    vector<T> linResources; // store the square grid's resource info into a linear vector
    vector<T> linShare; // store the square grid's resource share info into a linear vector
};
 
#endif
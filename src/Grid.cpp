#include "Grid.h"

template <typename T>
Grid<T>::Grid(int edgeSize) {
    dim = edgeSize * edgeSize;
    linearGrid(dim);
}

template <typename T>
Grid<T>::Grid(int edgeSize, T initResources) {
    dim = edgeSize * edgeSize;
    linearGrid(dim, initResources);
}

template <typename T>
vector<T> Grid<T>::getGrid() {
    return linearGrid;
}
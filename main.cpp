#include <iostream>
#include "src/Grid.h"
using namespace std;

int main() {
    Grid<double> myGrid(3, 0);
    
    cout << "Created grid of dim " << myGrid.dim << "\n";

    vector<double> linGrid = myGrid.getGrid();
    for (int i = 0; i < myGrid.dim; ++i) {
        cout << "value " << i << " = " << linGrid[i];
    }

    return 0;
}
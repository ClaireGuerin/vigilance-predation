#include <iostream>
#include "src/Grid.h"
using namespace std;

int main() {
    Grid<double> myGrid(3, 0);
    
    cout << "Created grid of dim " << myGrid.dim << "\n";

    vector<double> linGrid = myGrid.resourceDistri();
    for (int i = 0; i < myGrid.dim; ++i) {
        cout << "value " << i << " = " << linGrid[i] << "\n";
    }

    return 0;
}
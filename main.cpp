#include <iostream>
#include "src/Grid.h"
#include "src/Individual.h"
using namespace std;

int main() {
    Grid<double> myGrid(3, 0);
    
    cout << "Created grid of dim " << myGrid.dim << "\n";

    vector<double> linGrid = myGrid.resourceDistri();
    for (int i = 0; i < myGrid.dim; ++i) {
        cout << "value " << i << " = " << linGrid[i] << "\n";
    }

    Individual myInd(10);
    Coord coord = myInd.getCoordinates();

    cout    << "Created indiv and placed on grid of size 10, at coordinates: "
            << coord.x << ", " << coord.y << "\n";

    Individual myInd2(10);
    Coord coord2 = myInd2.getCoordinates();

    cout    << "Created indiv and placed on grid of size 10, at coordinates: "
            << coord2.x << ", " << coord2.y << "\n";

    Individual myInd3(10);
    Coord coord3 = myInd3.getCoordinates();

    cout    << "Created indiv and placed on grid of size 10, at coordinates: "
            << coord3.x << ", " << coord3.y << "\n";


    return 0;
}
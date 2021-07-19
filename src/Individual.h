#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H
#include <vector>
#include "Random.h"
using namespace std;

struct Coord {
    int x;
    int y;
};

class Individual {
private:
    int m;
    double v;
    bool mutant;
    double storage = 0;
    bool isAlive = true;
    Coord coordinates;
public:
    Individual(int);
    Individual(int, double);
    //~Individual(); // in case I need a destructor, might come in handy for new generation? 
    Coord getCoordinates() { return coordinates;}; 
};



#endif
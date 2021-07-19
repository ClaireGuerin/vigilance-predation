#ifndef RANDOM_H
#define RANDOM_H
#include <random>

namespace rnd 
{
    typedef std::uniform_int_distribution<int> randomInt;
    typedef std::mt19937_64 rng;
    
} 


#endif
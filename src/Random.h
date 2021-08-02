#ifndef VIGI_RANDOM_H_INCLUDED
#define VIGI_RANDOM_H_INCLUDED

#include <random>
#include "Utils.h"

namespace vigi 
{
    class Random
    {
    public:
        Random(const Parameter &par) :
        normalDist_ (0.0, par.mutStep),
        uniformDist_ (0, static_cast<int>(par.edgeSize) - 1)
        {
        }

        template <typename RENG>
        double deviation(RENG & reng) { return normalDist_(reng); }

        template <typename RENG>
        Coord coordinate(RENG& reng) { return { uniformDist_(reng), uniformDist_(reng) }; }

    private:
        std::normal_distribution<> normalDist_;
        std::uniform_int_distribution<> uniformDist_;
    };    

}

#endif
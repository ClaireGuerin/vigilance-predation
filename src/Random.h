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
        uniformEdgeDist_ (0, static_cast<int>(par.edgeSize) - 1),
        uniformStepDist_ (-1, 1)
        {
        }

        template <typename RENG>
        double deviation(RENG & reng) { return normalDist_(reng); }

        template <typename RENG>
        Coord coordinate(RENG& reng) { return { uniformEdgeDist_(reng), uniformEdgeDist_(reng) }; }

        template <typename RENG>
        Coord move(const Coord& current, RENG& reng) { return { current.x + uniformStepDist_(reng), current.y + uniformStepDist_(reng) }; }

    private:
        std::normal_distribution<> normalDist_;
        std::uniform_int_distribution<> uniformEdgeDist_;
        std::uniform_int_distribution<> uniformStepDist_;
    };    

}

#endif
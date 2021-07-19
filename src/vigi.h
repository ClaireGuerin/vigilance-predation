#ifndef VIGI_VIGI_H_INCLUDED
#define VIGI_VIGI_H_INCLUDED

#include <random>

namespace vigi {

  struct Parameter
  {
    size_t edgeSize = 100;                   // grid size
    double initResources = 1.0;   // initial resources

    size_t popSize = 1000;                         
    double v = 1.0;                       // ??
  };


  struct Coord
  {
    int x;
    int y;
  };

  template <typename RENG>
  Coord randomCoord(size_t edgeSize, RENG& reng)
  {
    auto pdist = std::uniform_int_distribution<>(0, static_cast<int>(edgeSize) - 1);
    return { pdist(reng), pdist(reng) };
  }

}

#endif

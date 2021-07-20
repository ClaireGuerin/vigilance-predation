#ifndef VIGI_UTILS_H_INCLUDED
#define VIGI_UTILS_H_INCLUDED

#include <random>

namespace vigi {

  struct Parameter
  {
    size_t edgeSize = 100;        // grid size
    double initResources = 1.0;   // initial resources

    size_t popSize = 1000;                         
    double v = 1.0;               // initial vigilance level
    double mutRate = 0.01;        // mutation rate
    double mutStep = 0.02;        // mutation step (deviation)
    bool bounded = true;          // is the phenotype bounded between 0 and 1?

    double p = 0.2;               // basal predation risk

    double eff = 0.9;             // efficiency in resources extraction
    double competition = 1.0;      // gamma, intra-cell competition for resources
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

  template <typename RENG>
  Coord randomMove(Coord current, RENG& reng)
  {
    auto pdist = std::uniform_int_distribution<>(-1, 1);
    return { current.x + pdist(reng), current.y + pdist(reng) };
  }

  template <typename RENG>
  bool randomBool(double pTrue, RENG& reng)
  {
    auto pdist = std::bernoulli_distribution(pTrue);
    return pdist(reng);
  }

  template <typename RENG>
  double randomDev(double mutationStep, RENG& reng)
  {
    auto pdist = std::normal_distribution<>(0, mutationStep);
    return pdist(reng);
  }

  template <typename N>
  N bound(N num, N min, N max) {
    if (num > max) num = max;
    else if (num < min) num = min;

    return num;
  }

}

#endif

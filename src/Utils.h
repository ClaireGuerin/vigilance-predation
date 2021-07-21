#ifndef VIGI_UTILS_H_INCLUDED
#define VIGI_UTILS_H_INCLUDED

#include <random>

namespace vigi {

  struct Parameter
  {
    size_t edgeSize = 100;              // grid size
    double initResources = 1.0;         // initial resources

    size_t popSize = 100;               // population size
    size_t nGen = 5;                    // number of generations
    size_t ecoTime = 4;                 // number of ecological steps per generation

    double v = 1.0;                     // initial vigilance level
    double mutRate = 0.01;              // mutation rate
    double mutStep = 0.02;              // mutation step (deviation)
    bool bounded = true;                // is the phenotype bounded between 0 and 1?

    double p = 0.2;                     // basal predation risk

    double eff = 0.9;                   // efficiency in resources extraction
    double competition = 1.0;           // gamma, intra-cell competition for resources

    double residualFertility = 0.0001;  // fertility cannot be zero (poisson distri), so when ind has no resources, its fertility is residual
    double fecundity = 2.0;             // basal fecundity

    double rGrowth = 2.0;               // resources growth rate

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

  template <typename RENG>
  size_t randomRepro(double fertility, RENG& reng)
  {
    auto pdist = std::poisson_distribution<>(fertility);
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

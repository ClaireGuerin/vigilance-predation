#ifndef VIGI_INDIVIDUAL_H
#define VIGI_INDIVIDUAL_H

#include <vector>
#include <algorithm>
#include "Utils.h"

namespace vigi {

  
  class Individual 
  {

  friend class Population;

  public:
    explicit Individual(const Parameter& param);

    template <typename RENG>
    void set_random_coord(size_t edgeSize, RENG& reng)
    {
      coordinates_ = randomCoord(edgeSize, reng);
    }

    template <typename RENG>
    void mutate(const Parameter& param, RENG& reng) {
      isMutant_ = randomBool(param.mutRate, reng);
      if (isMutant_) {
        double deviation = randomDev(param.mutStep, reng);
        double phen = vigilance_ + deviation;
        vigilance_ = (param.bounded) ? std::clamp(phen, 0.0, 1.0) : phen;
      }
    }

    template <typename RENG>
    void explore(const Parameter& param, RENG& reng) {
        Coord unbounded = randomMove(coordinates_, reng);
        coordinates_ = {  std::clamp(unbounded.x, 0, static_cast<int>(param.edgeSize) - 1), 
                          std::clamp(unbounded.y, 0, static_cast<int>(param.edgeSize) - 1) };
    }

    template <typename RENG>
    void survive(const Parameter& param, RENG& reng) {
      double predationRisk = param.p * (1 - vigilance_); // 0 <= p <= 1 is the basal predation risk 
      isAlive_ = randomBool((1 - predationRisk), reng);
    }

    void gather(const Parameter&, double, double);

    template <typename RENG>
    void reproduce(const Parameter& param, RENG& reng) {
      double fertility = param.residualFertility + param.fecundity * storage_;
      nOffspring_ = randomRepro(fertility, reng);
    }

    size_t offspring() const { return nOffspring_; }

  private:
    size_t nOffspring_ = 0;
    double vigilance_;
    double storage_ = 0.0;
    bool isMutant_ = false;
    bool isAlive_ = true;
    Coord coordinates_ = Coord{};

    void cleanSlate() {
      nOffspring_ = 0;
      storage_ = 0.0;
      isMutant_ = false;
      isAlive_ = true;
    }
  };

  Individual::Individual(const Parameter& param) : 
    vigilance_(param.v)
  {
  }

  void Individual::gather(const Parameter& param, double resources, double share) {
    storage_ += param.eff * resources * share; 
  }

}

#endif
#ifndef VIGI_INDIVIDUAL_H
#define VIGI_INDIVIDUAL_H

#include <vector>
#include <algorithm>
#include "Utils.h"
#include "Random.h"

namespace vigi {

  
  class Individual 
  {

  friend class Population;

  public:
    explicit Individual(const Parameter& param);

    template <typename RENG>
    void set_random_coord(RENG& reng, Random& rd)
    {
      coordinates_ = rd.coordinate(reng);
    }

    template <typename RENG>
    void mutate(const Parameter& param, RENG& reng, Random& rd) {
      isMutant_ = rd.boolean(param.mutRate, reng);
      if (isMutant_) {
        double deviation = rd.deviation(reng);
        double phen = vigilance_ + deviation;
        vigilance_ = std::clamp(phen, 0.0, 1.0);
      }
    }

    template <typename RENG>
    void explore(const Parameter& param, RENG& reng, Random& rd) {
        Coord unbounded = rd.move(coordinates_, reng);
        coordinates_ = {  std::clamp(unbounded.x, 0, static_cast<int>(param.edgeSize) - 1), 
                          std::clamp(unbounded.y, 0, static_cast<int>(param.edgeSize) - 1) };
    }

    template <typename RENG>
    void survive(const Parameter& param, RENG& reng, Random& rd) {
      double predationRisk = param.p * (1 - vigilance_); // 0 <= p <= 1 is the basal predation risk 
      isAlive_ = rd.boolean((1 - predationRisk), reng);
    }

    void gather(const Parameter&, double, double);

    template <typename RENG>
    void reproduce(const Parameter& param, RENG& reng, Random& rd) {
      double fertility = /*param.residualFertility + */ param.fecundity * storage_;
      nOffspring_ = fertility ? rd.reproduction(fertility, reng) : 0;
    }

    Individual deliver();

    //size_t offspring() const { return nOffspring_; }

  private:
    size_t nOffspring_ = 0;
    double vigilance_;
    double storage_ = 0.0;
    bool isMutant_ = false;
    bool isAlive_ = true;
    Coord coordinates_ = Coord{};

  };

  Individual::Individual(const Parameter& param) : 
    vigilance_(param.v)
  {
  }

  void Individual::gather(const Parameter& param, double resources, double share) {
    storage_ += param.eff * resources * share; 
  }

  Individual Individual::deliver() 
  {
    Individual offsp = *this;
    offsp.nOffspring_ = 0;
    offsp.storage_ = 0;
    offsp.isMutant_ = false;
    offsp.isAlive_ = true;

    return offsp;
  }

}

#endif
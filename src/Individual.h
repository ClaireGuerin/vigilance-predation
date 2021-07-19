#ifndef VIGI_INDIVIDUAL_H
#define VIGI_INDIVIDUAL_H

#include <vector>
#include "Utils.h"

namespace vigi {

  
  class Individual 
  {
  public:
    explicit Individual(const Parameter& param);

    template <typename RENG>
    void set_random_coord(size_t edgeSize, RENG& reng)
    {
      coordinates_ = randomCoord(edgeSize, reng);
    }

    vigi::Coord coordinates() const { return coordinates_; }

  private:
    double v_;
    bool isMutant_ = false;
    double storage_ = 0;
    bool isAlive_ = true;
    Coord coordinates_ = Coord{};
  };

  Individual::Individual(const Parameter& param) : 
    v_(param.v)
  {
  }

}

#endif
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
      coordinate_ = randomCoord(edgeSize, reng);
    }

  private:
    double v_;
    bool mutant_ = false;
    double storage_ = 0;
    bool isAlive_ = true;
    Coord coordinate_ = Coord{};
  };

  Individual::Individual(const Parameter& param) : 
    v_(param.v)
  {
  }

}

#endif
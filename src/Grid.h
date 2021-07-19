#ifndef VIGI_GRID_H
#define VIGI_GRID_H

#include <vector>
#include "vigi.h"

namespace vigi {

  class Grid
  {
  public:
    explicit Grid(const Parameter& param) :
      dim_(param.edgeSize),
      resources_(param.edgeSize * param.edgeSize, param.initResources),
      share_(param.edgeSize * param.edgeSize, 0.0)
    {
    }

    size_t dim() const { return dim_; }

  private:
    size_t dim_;
    std::vector<double> resources_;
    std::vector<double> share_;
  };

}

#endif
#ifndef VIGI_GRID_H
#define VIGI_GRID_H

#include <vector>
#include "vigi.h"

namespace grid {

  template<typename T>
  class Grid
  {
  public:
    Grid(const size_t edgeSize, const T initFill) :
      dim_(edgeSize),
      fill_(edgeSize * edgeSize, initFill)
      {
      }

    size_t dim() const { return dim_; }

    T read(size_t x, size_t y) {
      // grid fill stored linearly: x = 0, y = 0; x = 0, y = 1; ... x = 0, y = dim; ... x = 1, y = 0; ...
      return fill_[(x * dim_) + y];
    }

    void write(size_t x, size_t y, T fill) {
      fill_[(x * dim_) + y] = fill;
    }

  private:
    size_t dim_;
    std::vector<double> fill_;
  };
}



#endif
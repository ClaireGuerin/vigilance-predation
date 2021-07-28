#ifndef VIGI_GRID_H
#define VIGI_GRID_H

#include <vector>
#include <algorithm>
#include <cassert>
#include "Utils.h"

namespace grd {

  struct Coord
  {
    int x;
    int y;
  };  

  template<typename T>
  class Grid
  {
  public:
    using value_type = T;

    // make it explicit that we are happy with the compiler-generated stuff
    Grid(const Grid&) = default;                      // copy constructor 
    Grid(Grid&&) = default;                           // move constructor
    Grid& operator=(const Grid&) = default;           // assignment
    Grid& operator=(Grid&&) = default;                // move assignment 

    explicit Grid(const size_t edgeSize) :
      dim_(edgeSize),
      fill_(edgeSize * edgeSize)
    {
    }

    Grid(const size_t edgeSize, const value_type& initFill) :
      dim_(edgeSize),
      fill_(edgeSize * edgeSize, initFill)
      {
      }

    size_t dim() const noexcept { return dim_; }
    size_t size() const noexcept { return fill_.size(); }

    void assign(const value_type& val) 
    { // fill the whole vector with val
      std::fill(begin(), end(), val); 
    }

    // When we feed the function with an x and y value

    const value_type& operator()(int x, int y) const
    {
      assert((x >= 0) && (x < dim_) && (y >= 0) && (y < dim_));
      return fill_[dim_ * y + x];   // row-major, we are in C/C++ ;)
    }

    value_type& operator()(int x, int y)
    {
      assert((x >= 0) && (x < dim_) && (y >= 0) && (y < dim_));
      return fill_[dim_ * y + x];   // row-major, we are in C/C++ ;)
    }

    // when we feed the function directly with coordinates

    const value_type& operator()(const Coord& coor) const 
    { 
      return this->operator()(coor.x, coor.y); 
    }

    value_type& operator()(const Coord& coor) 
    { 
      return this->operator()(coor.x, coor.y); 
    }

    // linear access
    const value_type& operator[](size_t lin_idx) const { return fill_[lin_idx]; }
    value_type& operator[](size_t lin_idx) { return fill_[lin_idx]; }

    // the user should be able to access iterators into the linear buffer
    // for use in STL algorithms, range for loop etc.
    auto begin() noexcept { return fill_.begin(); }
    auto begin() const noexcept { return fill_.begin(); }
    auto cbegin() const noexcept { return fill_.cbegin(); }

    auto end() noexcept { return fill_.end(); }
    auto end() const noexcept { return fill_.end(); }
    auto cend() const noexcept { return fill_.cend(); }

  private:
    size_t dim_;
    std::vector<value_type> fill_;
  };
}

#endif
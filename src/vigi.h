#ifndef VIGI_VIGI_H_INCLUDED
#define VIGI_VIGI_H_INCLUDED


namespace vigi {


  struct Parameter
  {
    int edgeSize = 100;                   // grid size
    double /* ? */ initResources = 1.0;   // initial resources

    int N = 1000;                         // population size
    int m = 1;                            // ??
    double v = 1.0;                       // ??
  };


  struct Coord
  {
    int x;
    int y;
  };


}

#endif

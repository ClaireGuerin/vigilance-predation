#include <stdexcept>
#include <iostream>
//#include <vector>
//#include <random>
#include <fstream>
#include "src/Simulation.h"

int main() 
{
  try 
  {
    std::ofstream ofsR("resources_out.txt");
    if (!ofsR.is_open())
    {
      throw std::runtime_error("unable to open resources file");
    }

    std::ofstream ofsV("vigilance_out.txt");
    if (!ofsV.is_open())
    {
      std::cerr << "error: unable to open vigilance file\n";
      exit(EXIT_FAILURE);
    }

    std::ofstream ofsE("exploration_out.txt");
    if (!ofsE.is_open())
    {
      std::cerr << "error: unable to open vigilance file\n";
      exit(EXIT_FAILURE);
    }

    vigi::Simulation sim("build/");
    sim.setup();
    sim.run();
    sim.save(ofsR, ofsV, ofsE);
    
    return 0;
  }
  
  catch (const std::exception& err) 
  {
    std::cerr << "Exception: " << err.what() << '\n';
  }

  catch (...) 
  {
    std::cerr << "Unknown exception\n";
  }
  
  return -1;
}

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
    std::string path = "some-data/";

    std::ofstream ofsR(path + "resources_out.txt");
    if (!ofsR.is_open())
    {
      throw std::runtime_error("unable to open resources file");
    }

    std::ofstream ofsV(path + "vigilance_out.txt");
    if (!ofsV.is_open())
    {
      std::cerr << "error: unable to open vigilance file\n";
      exit(EXIT_FAILURE);
    }

    std::ofstream ofsE(path + "exploration_out.txt");
    if (!ofsE.is_open())
    {
      std::cerr << "error: unable to open vigilance file\n";
      exit(EXIT_FAILURE);
    }

    vigi::Simulation sim(path);
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

#include <stdexcept>
#include <iostream>
//#include <vector>
//#include <random>
#include <cstdlib>
#include <fstream>
#include "src/Simulation.h"

std::string path = "gibberish/"; // default
bool visual = false; // default

void readCommandLine(char* argv[], int pos)
{
  std::string argName = argv[pos];
  if ( argName.compare("-f") ) path = argv[pos + 1];
  else if ( argName.compare("-v") ) 
  {
    visual = static_cast<bool>(argv[pos + 1]);
  }
}

int main(int argc, char* argv[]) 
{
  try 
  {

    if (argc > 1)
    {
      readCommandLine(argv, 1);
      if (argc > 3) readCommandLine(argv, 3);
    }

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

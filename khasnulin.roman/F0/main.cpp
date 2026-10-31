#include "CommandProcessor.hpp"
#include "Model.hpp"
#include "Recommender.hpp"
#include <iostream>

int main()
{
  khasnulin::Learner learner;
  khasnulin::Recommender recommender;
  khasnulin::CommandProcessor proc(learner, recommender);
  proc.run(std::cin, std::cout);
}

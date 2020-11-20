#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <vector>
#include <string>
#include <ctime>
#include <iostream>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
    std::vector<std::string> prev_utilization;
    clock_t time_when_prev_utilization_updated;
    static float LongTermUtilization(std::vector<std::string> utilization);
    static float ShortTermUtilization(std::vector<std::string> utilization, std::vector<std::string> prev_utilization);
};

#endif
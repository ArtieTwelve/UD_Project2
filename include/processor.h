#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <vector>
#include <string>
#include <regex>

class Processor {
 public:
  float Utilization();  

 private:
  std::vector<std::string> getRawUtilization(std::regex regEx);
  std::pair<long,long> getCpuTimes(std::vector<std::string> times);

};

#endif
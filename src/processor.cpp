#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization

// 

float Processor::Utilization() {
     std::vector<std::string> percentages = LinuxParser::CpuUtilization() ;
     std::string top = percentages.front();
     return std::stof(top); 
     
}
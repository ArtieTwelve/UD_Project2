#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;



// Return the system's CPU
Processor& System::Cpu() { 
  Processor proc;
  float cpuUtialization =  proc.Utilization();
  return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() {
   std::vector<int> pids = LinuxParser::Pids();

    for(int pid: pids) { 
     processes_.push_back(getProcInfo(pid));
    }
   return processes_; 
}

// get the procces information this pid
Process System::getProcInfo(int pid) {
  Process proc;
  proc.setPid(pid);
  proc.User();
  proc.CpuUtilization();
  proc.Ram();
  proc.UpTime();
  proc.Command();

  return proc;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { 
  return LinuxParser::Kernel();
}

// Return the system's memory utilization
float System::MemoryUtilization() { 
  return LinuxParser::MemoryUtilization();; 
}

// Return the operating system name
std::string System::OperatingSystem() { 
    return LinuxParser::OperatingSystem();
 }

// Return the number of processes actively running on the system
int System::RunningProcesses() { 
   return LinuxParser::RunningProcesses();
 }

// Return the total number of processes on the system
int System::TotalProcesses() { 
  return  LinuxParser::TotalProcesses(); 
}

// Return the number of seconds since the system started running
long int System::UpTime() { 
  return LinuxParser::UpTime();
 }


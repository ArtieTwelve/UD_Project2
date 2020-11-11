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

// TODO: Return the system's CPU
Processor& System::Cpu() { 
  Processor proc;
  float cpuUtialization =  proc.Utilization();
  return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
   std::vector<int> pids = LinuxParser::Pids();
    //createUserMap(pids);
    createUserMap();
    for(int pid: pids) {
     Process proc = getProcInfo(pid);
     processes_.push_back(proc);
    }
   return processes_; 
}
std::string getUserId(int pid) {

  std::string pidPath = LinuxParser::kProcDirectory+std::to_string(pid)+LinuxParser::kStatusFilename;
  std::ifstream uidstream(pidPath);
  bool found = false;
  std::regex e ("(Uid:)");
  std::string toss,uid;
  if (uidstream.is_open()) {
    while(uidstream >> toss >> uid) {
      if(std::regex_match(toss,e)) {
        found = true;
        break;
      }
      else {
        uidstream.ignore(1000,'\n');
      }
    }   
  }
  if(found)
    return uid;
  else
    return "0";
}
Process System::getProcInfo(int pid) {
  Process proc;
  std::string user,userId;
  proc.setPid(pid);
  userId = getUserId(pid);
  if(userId != "") {
    user = getUserName(userId);
    proc.setUser(user);
  }
  return proc;
}

void System::createUserMap() {
  
  userMap.clear();

  string line,ex,pid;
  char delim = ':';
  string token,name;
  std::vector<std::string> tempVec;

  std::ifstream filestream(LinuxParser::kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream,line)) {
      std::istringstream ss(line);
      while (std::getline(ss,token,delim)) {
          tempVec.push_back(token);
        }
        userMap[tempVec.at(2)] = tempVec.at(0);
        tempVec.clear();
      }
    }

}


std::string System::getUserName(std::string uid) {
  std::string name = userMap[uid];
  return name;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { 
  return LinuxParser::Kernel();
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { 
  float memPercent = LinuxParser::MemoryUtilization();
  return memPercent; 
}

// TODO: Return the operating system name
std::string System::OperatingSystem() { 
    return LinuxParser::OperatingSystem();
 }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return 0; }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return 0; }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return 0; }
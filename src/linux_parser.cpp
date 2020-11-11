#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <chrono>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

std::map<std::string,std::vector<long>> getAllUtilization() {
  std::map<std::string,std::vector<long>> allUtes;
  std::regex e ("(cpu)(.*)");

  std::ifstream cpustream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  std::string cpu,user,nice,system,idle,iowait,irq,softirq,steal,guest,guestnice;
  long usertime,nicetime,idlealltime,systemalltime,virtualltime,totaltime;
  if (cpustream.is_open()) {
    //long prevIdle,prevNonIdle,prevTotalTime;
    
    while(cpustream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guestnice) {
      if(std::regex_match(cpu,e)) {
        // Get the intial times and calcuate the total time
       // std::cout << "Line: " << cpu << std::endl;
        usertime = stol(user) - stol(guest);
        nicetime = stol(nice) - stol(guestnice);
        idlealltime = stol(idle) + stol(iowait);
        systemalltime = stol(system) + stol(irq) + stol(softirq);
        virtualltime = stol(guest) + stol(guestnice);
        totaltime = usertime + nicetime + systemalltime + idlealltime + stol(steal) + virtualltime;

        allUtes[cpu] = {totaltime,idlealltime};
        
      }
    }
    cpustream.close();
  }

  return allUtes;
}


// Get the processor information at time = 0.0s and time += 100ms. Use both times to calculate the 
// CPU utilization for each processing core

vector<string> LinuxParser::CpuUtilization() { 
  std::map<std::string,std::vector<long>> initialTimes, laterTimes;

  initialTimes = getAllUtilization();
  // sleep for 200 milliseconds
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  laterTimes = getAllUtilization();

  std::vector<std::string> percentages;
  std::vector<long> initVec,laterVec;
  std::string thisCpu;
  
  std::map<std::string,std::vector<long>>::iterator it;

  for(it = initialTimes.begin();it != initialTimes.end(); it++) {
    
    thisCpu = it->first;
    initVec = it->second;

    laterVec = laterTimes[thisCpu];
    long totald = laterVec[0] - initVec[0];
    long idled = laterVec[1] - initVec[1];

    std::string percent = std::to_string((totald - idled)/totald);
    percentages.push_back(percent);
  }
	     // fake some data
    percentages.at(0) = "0.54";
    percentages.at(1) = "0.32";
    percentages.at(2) = "0.64";
    percentages.at(4) = "0.74";
  
 
  return percentages;
  //return kernel;

 }


/*  // Get the processor information at time = 0.0s and time += 100ms. Use both times to calculate the 
// CPU utilization for each processing core

vector<string> LinuxParser::CpuUtilization() { 
  std::map<std::string,std::vector<long>> initialTimes, laterTimes;

  std::regex e ("(cpu)(.*)");
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::string cpu,user,nice,system,idle,iowait,irq,softirq,steal,guest,guestnice;
  long usertime,nicetime,idlealltime,systemalltime,virtualltime,totaltime;

  if (stream.is_open()) {
    long prevIdle,prevNonIdle,prevTotalTime;
    
    while(stream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guestnice) {
      if(std::regex_match(cpu,e)) {
        // Get the intial times and calcuate the total time
        std::cout << "Line: " << cpu << std::endl;
        usertime = stol(user) - stol(guest);
        nicetime = stol(nice) - stol(guestnice);
        idlealltime = stol(idle) + stol(iowait);
        systemalltime = stol(system) + stol(irq) + stol(softirq);
        virtualltime = stol(guest) + stol(guestnice);
        totaltime = usertime + nicetime + systemalltime + idlealltime + stol(steal) + virtualltime;

        initialTimes[cpu] = {totaltime,idlealltime};
        
      }
    }
    stream.close();
    // sleep for 200 milliseconds
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

  }
  std::cout << " End of cpus" << std::endl;
  std::vector<std::string> a;
  return a;
  //return kernel;

 } */
// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  std::string line,line2,memTotalTag,memTotal,memFreeTag,memFree;
  float memPercent;
  std::ifstream memstream(LinuxParser::kProcDirectory + LinuxParser::kMeminfoFilename);

  if (memstream.is_open()) {
    std::getline(memstream, line);
    std::istringstream linestream(line);
    linestream >> memTotalTag >> memTotal;
    std::getline(memstream, line2);
    std::istringstream linestream2(line2);
    linestream2 >> memFreeTag >> memFree; 

    memPercent = (stof(memTotal) - stof(memFree))/stof(memTotal);
  }
return memPercent; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { return 0; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }



// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return 0; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }


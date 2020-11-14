#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>
#include <iomanip>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

 using namespace LinuxParser;

// declare the userMap
std::map<std::string,std::string> userMap;

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

// Get the processor information at time = 0.0s and time += 100ms. Use both times to calculate the 
// CPU utilization for each processing core

vector<string> LinuxParser::CpuUtilization() { 
  std::vector<std::string> blank;
  return blank;
 }


// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  std::string line,line2,line3,memTotalTag,memTotal,memFreeTag,memFree, memAvailTag, memAvail;
  float memPercent;
  std::ifstream memstream(LinuxParser::kProcDirectory + LinuxParser::kMeminfoFilename);

  if (memstream.is_open()) {
    std::getline(memstream, line);
    std::istringstream linestream(line);
    linestream >> memTotalTag >> memTotal;

    std::getline(memstream, line2);
    std::istringstream linestream2(line2);
    linestream2 >> memFreeTag >> memFree; 

    std::getline(memstream,line3);
    std::istringstream linestream3(line3);
    linestream3 >> memAvailTag >> memAvail; 
    memPercent = (stof(memTotal) - stof(memAvail))/stof(memTotal);
  }
return memPercent; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
    string uptime, idletime;
    string line;
    std::ifstream stream(kProcDirectory + kUptimeFilename);
    if (stream.is_open()) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> uptime >> idletime;
    }
    return stol(uptime);  
}



// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

float ProcessCpu(int pid) {

  float num = 0.0;
  return num;
}


// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  std::ifstream procstream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  std::string line,procCard,procTotal;
  std::regex e ("(processes)");
  if(procstream.is_open()) {
    while(std::getline(procstream,line)) {
      std::istringstream linestream(line);
      linestream >> procCard >>  procTotal;
      if(std::regex_match(procCard,e)) {
            break;
      }  
    } 
  }
   return std::stoi(procTotal); 
}


// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  std::ifstream procstream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  std::string line,procCard,procRunning = "0";
  std::regex regEx("(procs_running)");
  if(procstream.is_open()) {
    while(std::getline(procstream,line)) {
      std::istringstream linestream(line);
      linestream >> procCard >> procRunning;
      if(std::regex_match(procCard,regEx)) {
        break;
      } 
    }   
  }
  return std::stoi(procRunning); 
 }

 // Get the user name from the process id
string LinuxParser::Uid(int pid) { 
  std::string pidPath = LinuxParser::kProcDirectory+std::to_string(pid)+LinuxParser::kStatusFilename;
  std::ifstream uidstream(pidPath);
  std::regex regEx ("(Uid:)");
  std::string line,toss,uid;
  if (uidstream.is_open()) {
    while(std::getline(uidstream,line)) {
      std::istringstream linestream(line);
      linestream >> toss >> uid;
      if(std::regex_match(toss,regEx)) {
        break;
      }
    }   
  }
  return uid;
 }


std::string LinuxParser::Command(int pid[[maybe_unused]]) { 
    std::string command,line;
    std::ifstream cmdstream(kProcDirectory+std::to_string(pid)+kCmdlineFilename);
    if (cmdstream.is_open()) {
      std::getline(cmdstream, line);
      std::istringstream linestream(line);
      linestream >> command;
    }
    return command;  
 }


string LinuxParser::Ram(int pid) {
  std::string pidPath = kProcDirectory+std::to_string(pid)+kStatusFilename;
  std::ifstream memstream(pidPath);
  std::regex mem ("(VmSize:)");
  std::string line,toss,vmem;
  if (memstream.is_open()) {
    while (std::getline(memstream, line)) {
      std::istringstream linestream(line);
      linestream >> toss >> vmem;
      if(std::regex_match(toss,mem)) {
        break;
      }
    }   
  }
  float fmem = stof(vmem);
  fmem = fmem * 0.001;
  std::stringstream ss;
  ss << std::fixed << std::setprecision(2) << fmem; 
  return ss.str();
}


string LinuxParser::User(int pid) { 
  // if the userMap is empty, fill it up
  if(userMap.empty())
    createUserMap();
  std::string uid = LinuxParser::Uid(pid);
  return userMap[uid]; 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }

// utility methods
// creates a user map. Key: UID, Value: Username
void LinuxParser::createUserMap() {
  
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



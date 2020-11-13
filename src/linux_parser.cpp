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
// Convenience method to get the cpu utilization raw strings. If the regEx only matches ("cpu "), then only the values from the first cpu line of /proc/stat will be returned.
// If it matches ("cpu)(.*) ") then the vector will hold all the values of cpu0 - cpuN. This way, it can be used for both the total system utilization and for each CPU.

std::vector<std::vector<std::string>> getRawUtilization(std::regex regEx) {
  //std::map<std::string,std::vector<long>> allUtes;
 // std::regex e ("(cpu)(.*)");
  std::vector<std::vector<std::string>> utilization;
  std::ifstream cpustream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  std::string cpu,user,nice,system,idle,iowait,irq,softirq,steal,guest,guestnice;
 
  if (cpustream.is_open()) {
    //long prevIdle,prevNonIdle,prevTotalTime;
    
    while(cpustream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guestnice) {
      if(std::regex_match(cpu,regEx)) {
        utilization.push_back( {user,nice,system,idle,iowait,irq,softirq,steal,guest,guestnice});
      }
    }
    cpustream.close();
  }

  return utilization;
}

// unpacks the times vector, breaks out the ints and calculates the total and idle times.
std::pair<long,long> getCpuTimes(std::vector<std::string> times) {

    std::pair<long,long> consolidatedTimes;

    long int idle = std::stoul(times[kIdle_]);  
    long int ioWait = std::stoul(times[kIOwait_]);

    long int user = std::stoul(times[kUser_]);
    long int guest =std::stoul(times[kGuest_]);
    long int nice = std::stoul(times[kNice_]);
    long int guestNice = std::stoul(times[kGuestNice_]);
    long int irq = std::stoul(times[kIRQ_]);
    long int softIrq = std::stoul(times[kSoftIRQ_]);
    long int system = std::stoul(times[kSystem_]);
    long int steal = std::stoul(times[kSteal_]);

    long int totalIdle = idle + ioWait;
    long int totalRunning = user + guest + nice + guestNice + irq + softIrq + system + steal;
    consolidatedTimes.first = totalIdle;
    consolidatedTimes.second = totalRunning;

 return consolidatedTimes;
}

// Get the processor information at time = 0.0s and time += 100ms. Use both times to calculate the 
// CPU utilization for each processing core

vector<string> LinuxParser::CpuUtilization() { //XXXX Check - this should be for all cores, not the main one. 
// Do not implement unless doing the bonus work. Move this to the Processor::Utilization() 

   //std::regex allCpuRegEx ("(cpu)(.*)");
  //std::regex numberedCpuRegEx ("(cpu)(.+)"); 

  // Only matches the top cpu in the file
  std::regex topCpuRegEx ("(cpu)");
  
  std::vector<std::string> utilization;
  std::vector<std::vector<std::string>> nCpus;
  std::vector<std::string> topCpuTimeZero,topCpuTime100ms;
  std::pair<long,long> previousTime, laterTime;
  
  // just get the top cpu line with this regEx
  nCpus = getRawUtilization(topCpuRegEx);
  topCpuTimeZero = nCpus.back();
   // sleep for 100 milliseconds
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  nCpus = getRawUtilization(topCpuRegEx);
  topCpuTime100ms = nCpus.back();

  previousTime = getCpuTimes(topCpuTimeZero);
  laterTime = getCpuTimes(topCpuTime100ms);

  // get the consolidated times
  long int laterIdleTime = laterTime.first;
  long int laterRunningTime = laterTime.second;

  long int prevIdleTime = previousTime.first;
  long int prevRunningTime = previousTime.second;

  long int totalTime = laterIdleTime + laterRunningTime;
  long int prevTotalTime = prevIdleTime + prevRunningTime;

  long int deltaTime = totalTime - prevTotalTime;
  long int deltaIdle = laterIdleTime - prevIdleTime;

  std::string cpu = std::to_string((float) (deltaTime - deltaIdle)/deltaTime);
  utilization.push_back(cpu);
 
  return utilization;
  //return kernel;

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



// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  std::ifstream procstream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  std::string procCard,procTotal;
  std::regex e ("(processes)");
  while(procstream >> procCard >> procTotal) {
  if(std::regex_match(procCard,e)) {
          break;
    } else {
       procstream.ignore(10000,'\n');
    }
  }   
   return std::stoi(procTotal); 
  }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  std::ifstream procstream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  std::string procCard,procRunning = "0";
  std::regex e ("(procs_running)");
  while(procstream >> procCard >> procRunning) {
  if(std::regex_match(procCard,e)) {
          break;
    } else {
       // ignore the rest of the line
       procstream.ignore(10000,'\n');
    }
  }   
   return std::stoi(procRunning); 

 }

 // Get the user name from the process id
string LinuxParser::Uid(int pid) { 
  std::string pidPath = LinuxParser::kProcDirectory+std::to_string(pid)+LinuxParser::kStatusFilename;
  std::ifstream uidstream(pidPath);
  std::regex e ("(Uid:)");
  std::string toss,uid;
  if (uidstream.is_open()) {
    while(uidstream >> toss >> uid) {
      if(std::regex_match(toss,e)) {
        break;
      }
      else {
        // ignore the rest of the line
        uidstream.ignore(10000,'\n');
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
  std::string toss,vmem;
  if (memstream.is_open()) {
    while(memstream >> toss >> vmem) {
      if(std::regex_match(toss,mem)) {
        break;
      }
      else {
        // ignore the rest of the line
        memstream.ignore(10000,'\n');
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



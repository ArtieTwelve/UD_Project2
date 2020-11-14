#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"


using std::string;
using std::to_string;
using std::vector;

// Finished


string Process::User() { return LinuxParser::User(pid); }
string Process::Ram() { return LinuxParser::Ram(pid); }
string Process::Command() { return LinuxParser::Command(pid); }



// TODO: Return this process's ID
int Process::Pid() { return pid; }
void Process::setPid(int pid) {this->pid = pid;}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    util = LinuxParser::ProcessCpu(pid);
    return util;
 }


// Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::UpTime(pid);
 }


// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    return a.util < util; 
}



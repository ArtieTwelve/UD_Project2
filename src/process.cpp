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

// get the user name associated with this process
string Process::User() { return user; }
void Process::setUser(std::string user) {
    this->user = LinuxParser::User(pid);
}


// TODO: Return this process's ID
int Process::Pid() { return pid; }
void Process::setPid(int pid) {this->pid = pid;}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }
void Process::setCpuUtilization(float util) {this->util = util;}


// TODO: Return the command that generated this process
string Process::Command() { return string(); }
void Process::setCommand(std::string command) {this->command = command;}

// TODO: Return this process's memory utilization
string Process::Ram() { return string(); }
void Process::setRam(std::string ram) {this->ram = ram;}




// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return 0; }
void Process::setUpTime(long int uptime) {this->uptime = uptime;}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }



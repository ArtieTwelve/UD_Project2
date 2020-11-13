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
float Process::CpuUtilization() { return 0; }
void Process::setCpuUtilization(float util) {this->util = util;}


// TODO: Return the command that generated this process

void Process::setCommand(std::string command) {this->command = command;}




// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return 0; }
void Process::setUpTime(long int uptime) {this->uptime = uptime;}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }



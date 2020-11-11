#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // TODO: See src/process.cpp
  void setPid(int);
  std::string User();                      // TODO: See src/process.cpp
  void setUser(std::string user);
  std::string Command();                   // TODO: See src/process.cpp
  void setCommand(std::string command);
  float CpuUtilization();                  // TODO: See src/process.cpp
  void setCpuUtilization(float util);
  std::string Ram();                       // TODO: See src/process.cpp
  void setRam(std::string ram);
  long int UpTime();                       // TODO: See src/process.cpp
  void setUpTime(long int uptime);
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  

  


  // TODO: Declare any necessary private members
 private:
  int pid;
  std::string user;
  std::string command;
  float util;
  std::string ram;
  long int uptime;
};

#endif
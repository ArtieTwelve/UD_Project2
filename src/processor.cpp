#include "processor.h"
#include "linux_parser.h"
#include <thread>
#include <chrono>

// Return the aggregate CPU utilization
// The collection of the cpu times and the calculation of the delta is broken up
// into a couple of convenience methods. It seemed better to do the calculation 
// here rather than in LinuxParser and using the "Jiffies" methods

// In order to get a more realistic measure, the code grabs the values, waits
// for 100ms and gets them again. 


float Processor::Utilization() {
  std::regex topCpuRegEx ("(cpu)");
  
  std::vector<std::string> utilization;

  std::vector<std::string> topCpuTimeZero,topCpuTime100ms;
  std::pair<long,long> previousTime, laterTime;
  
  // get the raw values at time zero
  topCpuTimeZero = getRawUtilization(topCpuRegEx);

   // sleep for 100 milliseconds
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  // get the raw values again
  topCpuTime100ms = getRawUtilization(topCpuRegEx);


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

  // convert float to string and then back to float for return
  // this seems an odd way to do it, but any other way 
  // alway returns zero due to rounding.
  std::string cpu = std::to_string((float) (deltaTime - deltaIdle)/deltaTime);
  return stof(cpu);

}

// Convenience method to get the cpu utilization raw strings.  
  
  std::vector<std::string> Processor::getRawUtilization(std::regex regEx) {
  std::vector<std::string> utilization;
  std::ifstream cpustream(LinuxParser::kProcDirectory+LinuxParser::kStatFilename);
  std::string cpu,user,nice,system,idle,iowait,irq,softirq,steal,guest,guestnice;
 
  if (cpustream.is_open()) {

    while(cpustream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guestnice) {
      if(std::regex_match(cpu,regEx)) {
        utilization.push_back(user);
        utilization.push_back(nice);
        utilization.push_back(system);
        utilization.push_back(idle);
        utilization.push_back(iowait);
        utilization.push_back(irq);
        utilization.push_back(softirq);
        utilization.push_back(steal);
        utilization.push_back(guest);
        utilization.push_back(guestnice);
        break;
      }
    }
    cpustream.close();
  }

  return utilization;
}

// unpacks the times vector, breaks out the ints and calculates the total and idle times.
std::pair<long,long> Processor::getCpuTimes(std::vector<std::string> times) {

    std::pair<long,long> consolidatedTimes;

    long int idle = std::stoul(times[LinuxParser::kIdle_]);  
    long int ioWait = std::stoul(times[LinuxParser::kIOwait_]);

    long int user = std::stoul(times[LinuxParser::kUser_]);
    long int guest =std::stoul(times[LinuxParser::kGuest_]);
    long int nice = std::stoul(times[LinuxParser::kNice_]);
    long int guestNice = std::stoul(times[LinuxParser::kGuestNice_]);
    long int irq = std::stoul(times[LinuxParser::kIRQ_]);
    long int softIrq = std::stoul(times[LinuxParser::kSoftIRQ_]);
    long int system = std::stoul(times[LinuxParser::kSystem_]);
    long int steal = std::stoul(times[LinuxParser::kSteal_]);

    long int totalIdle = idle + ioWait;
    long int totalRunning = user + guest + nice + guestNice + irq + softIrq + system + steal;
    consolidatedTimes.first = totalIdle;
    consolidatedTimes.second = totalRunning;

 return consolidatedTimes;
}
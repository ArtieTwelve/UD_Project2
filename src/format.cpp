#include <string>
#include <iomanip>
#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS

string Format::ElapsedTime(long seconds) { 
    int remSeconds, hours, minutes;
    std::string strHours,strMinutes,strSeconds;

 //   hours = seconds / 3600;
 //   minutes = (seconds % 3600) / 60;
 //   remSeconds = seconds % 60;

     hours = seconds/60/60%24;
     minutes = seconds/60%60;
     remSeconds = seconds%60;
    

 //   std::string uptime = std::to_string(hours)+":"+std::to_string(minutes)+":"+std::to_string(remSeconds);
 //   os << std::setfill('0') << std::setw(2) << h << ':' << std::setw(2) << m
 //             << ':' << std::setw(2) << s << '.' << std::setw(3) << ms;
   
   if(hours < 10)
        strHours = "0"+std::to_string(hours);
    else
        strHours = std::to_string(hours);
    if(minutes < 10)
        strMinutes = "0"+std::to_string(minutes);
    else
        strMinutes = std::to_string(minutes);
   if(remSeconds < 10)
        strSeconds = "0"+std::to_string(remSeconds);
    else
        strSeconds = std::to_string(remSeconds);   
   
    
    return strHours+":"+strMinutes+":"+strSeconds;
}
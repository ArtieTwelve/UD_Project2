#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int remSeconds, hours, minutes;

    hours = seconds / 3600;
    minutes = (seconds % 3600) / 60;
    remSeconds = seconds % 60;
    

    std::string uptime = std::to_string(hours)+":"+std::to_string(minutes)+":"+std::to_string(remSeconds);
    
    return uptime;
}
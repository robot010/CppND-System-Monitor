#include <string>
#include <cmath>
#include <sstream>

#include "format.h"

using std::string;
using std::floor; 
using std::stringstream;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) 
{ 
    int hours, mins, secs; 
    stringstream ss; 

    hours = floor(seconds/3600);
    mins = floor(seconds % 3600 / 60);
    secs = seconds - hours*3600 - mins*60;

    ss << hours << ":" << mins << ":" << secs;

    return ss.str();
   
}
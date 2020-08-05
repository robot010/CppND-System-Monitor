#include <string>
#include <cmath>
#include <sstream>

#include "format.h"

using std::string;
using std::floor; 
using std::stringstream;
using std::to_string;

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

    ss << Pad(hours,'0') << ":" << Pad(mins,'0') << ":" << Pad(secs,'0');

    return ss.str();
   
}

// Make sure the output should be 2-digit 
// Insert leading zero in case of single digit. 
string Format::Pad(int s, char c){
    string str = to_string(s);
    str.insert(str.begin(), 2-str.size(), c);
    return str;
}
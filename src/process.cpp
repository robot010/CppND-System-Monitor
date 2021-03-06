#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { 
    return pid_; 
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    CpuUtil_ = LinuxParser::CpuUtilization(pid_);
    return CpuUtil_;
}

// TODO: Return the command that generated this process
string Process::Command() {
    string cmd = LinuxParser::Command(pid_);
    if(cmd.length()>40){
        string sub_string = cmd.substr(40);
        return sub_string+"...";
    }
    return cmd;
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
    return LinuxParser::Ram(pid_);    
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
    return LinuxParser::User(pid_);
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
    return LinuxParser::Total_Elapsed_Time(pid_);
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    return CpuUtil_ > a.CpuUtil_;
}
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 

    std::ifstream stream(kProcDirectory+kMeminfoFilename);
    float res = 0;
    string line_MemTotal, line_MemFree, sMemTotal, sMemFree;
    float MemTotal, MemFree;
  
    if (stream.is_open()){
        std::getline(stream, line_MemTotal);
        std::getline(stream, line_MemFree);
        std::replace(line_MemTotal.begin(), line_MemTotal.end(), ':', ' ');
        std::replace(line_MemFree.begin(), line_MemFree.end(), ':',' ');
        std::istringstream stream_MemTotal(line_MemTotal), stream_MemFree(line_MemFree);
        stream_MemTotal >> sMemTotal >> MemTotal;
        stream_MemFree >> sMemFree >> MemFree;
    }
  
    res = (MemTotal - MemFree) / MemTotal;

    return res; 


}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
    std::ifstream stream(kProcDirectory + kUptimeFilename);
    long res;
    string line_uptime;
    
    if(stream.is_open()){
        std::getline(stream, line_uptime);
        std::istringstream stream_uptime(line_uptime);
        stream_uptime >> res;
    }
    return res;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
    
    string s_pid = std::to_string(pid);
    long utime, stime, cutime, cstime;
    string tmp, line;

    std::ifstream fstream(kProcDirectory+s_pid+kStatFilename);
    if(fstream.is_open()){
        std::getline(fstream, line);
        std::istringstream stream_line(line);
        for(int i=1;i<=17;i++){
            if(i!=14 && i!=15 && i!=16 && i!=17) stream_line >> tmp;
            else if(i==14) stream_line >> utime;
            else if(i==15) stream_line >> stime;
            else if(i==16) stream_line >> cutime;
            else if(i==17) stream_line >> cstime;
        }
    }

  return utime+stime+cutime+cstime;

}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid){
    string s_pid = std::to_string(pid);
    long start_time;
    string tmp, line;

    std::ifstream fstream(kProcDirectory+s_pid+kStatFilename);
    if(fstream.is_open()){
        std::getline(fstream, line);
        std::istringstream stream_line(line);
        for(int i=1;i<=22;i++){
            if(i!=22) stream_line >> tmp;
            else if(i==22) stream_line >> start_time;
        }
    }
    return start_time;
}


float LinuxParser::Total_Elapsed_Time(int pid) {

    long system_uptime = UpTime();
    long startTime = UpTime(pid);
    long Hertz = sysconf(_SC_CLK_TCK);
    long total_pid_elapsed_time = (float)system_uptime - (float)startTime/(float)Hertz;

    return total_pid_elapsed_time;

}

// TODO: Read and return CPU utilization
float LinuxParser::CpuUtilization(int pid) { 

    long total_pid_time_spent = ActiveJiffies(pid);
    float total_time_elapsed = Total_Elapsed_Time(pid);
    long Hertz = sysconf(_SC_CLK_TCK);
    float cpu_usage = (float)total_pid_time_spent / (float)Hertz / total_time_elapsed;

    return cpu_usage;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
    
    std::ifstream fstream(kProcDirectory + kStatFilename);
    int res;
    string line, key;
    int value;
    if (fstream.is_open()){
        while(std::getline(fstream, line)){
            std::istringstream linestream(line);
            linestream >> key >> value;
            if(key == "processes"){
                res = value; 
            }
        }  
    }
    
    return res;                                
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  
    std::ifstream fstream(kProcDirectory + kStatFilename);
    int res;
    string line, key;
    int value;
    if (fstream.is_open()){
        while(std::getline(fstream, line)){
            std::istringstream linestream(line);
            linestream >> key >> value;
            if(key == "procs_running"){
                res = value; 
            }
        }  
    }

    return res; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
    
    std::ifstream fstream(kProcDirectory+std::to_string(pid)+kCmdlineFilename);
    string line;
    if(fstream.is_open()){
        std::getline(fstream, line);
    }
    return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
    
    std::ifstream fstream(kProcDirectory+std::to_string(pid)+kStatusFilename);
    string line, key, res;
    int value;

    if(fstream.is_open()){
        while(std::getline(fstream, line)){
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);
            linestream >> key >> value;
            if(key=="VmSize"){
                res=std::to_string(value/1024);
                break;
            }
        }
    }

    return res;
 }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {  
    
    string s_pid = std::to_string(pid);
    string line, key, value, uid, username;
    
    std::ifstream filestream(kProcDirectory+s_pid+kStatusFilename);
    if(filestream.is_open()){
        while(std::getline(filestream, line)){
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);
            linestream >> key >> value;
            if(key=="Uid"){
                uid = value;
            }
        }
    }

    return uid; 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) 
{ 
    string uid = Uid(pid);
    string username, tmp_uid, line, x;
    std::ifstream fstream(kPasswordPath);
    if(fstream.is_open()){
        while(std::getline(fstream, line)){
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);
            linestream >> username >> x >> tmp_uid;
            if(tmp_uid==uid){
                break;
            }
        }
    }

    return username;
}
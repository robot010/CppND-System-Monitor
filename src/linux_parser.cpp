#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

template <typename T>
T LinuxParser::findValueByKey(std::string const &keyFilter, std::string const &filename) {
  std::string line, key;
  T value;

  std::ifstream stream(kProcDirectory + filename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == keyFilter) {
          return value;
        }
      }
    }
  }
  return value;
};


template <typename T>
T LinuxParser::getValueOfFile(std::string const &filename) {
  std::string line;
  T value;

  std::ifstream stream(kProcDirectory + filename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  return value;
};

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

    string memTotal = filterMemTotalString;
    string memFree = filterMemFreeString;
  
    float MemTotal = findValueByKey<float>(memTotal, kMeminfoFilename);
    float MemFree = findValueByKey<float>(memFree, kMeminfoFilename);
  
    return (MemTotal - MemFree) / MemTotal;

}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
    return getValueOfFile<long>(kUptimeFilename);
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
long LinuxParser::StartTime(int pid){
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
    long startTime = StartTime(pid);
    long Hertz = sysconf(_SC_CLK_TCK);
    long total_pid_elapsed_time = static_cast<float>(system_uptime) 
                                  - static_cast<float>(startTime)/static_cast<float>(Hertz);

    return total_pid_elapsed_time;

}

// TODO: Read and return CPU utilization
float LinuxParser::CpuUtilization(int pid) { 

    long total_pid_time_spent = ActiveJiffies(pid);
    float total_time_elapsed = Total_Elapsed_Time(pid);
    long Hertz = sysconf(_SC_CLK_TCK);
    float cpu_usage = static_cast<float>(total_pid_time_spent) / static_cast<float>(Hertz) / total_time_elapsed;

    return cpu_usage;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
    return findValueByKey<int>(filterProcesses, kStatFilename);               
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
    return findValueByKey<int>(filterRunningProcesses, kStatFilename); 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
    return getValueOfFile<string>(std::to_string(pid)+kCmdlineFilename);
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
    return std::to_string(findValueByKey<int>(filterProcMem, std::to_string(pid) + kStatusFilename)/1024);
 }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {  
    return findValueByKey<string>(filterUID, std::to_string(pid)+kStatusFilename);
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
#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// Keys
const std::string filterMemTotalString("MemTotal:");
const std::string filterMemFreeString("MemFree:");
const std::string filterProcesses("processes");
const std::string filterRunningProcesses("procs_running");
const std::string filterUID("Uid:");
const std::string filterProcMem("VmSize:");

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};

float CpuUtilization(int pid);
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();
float Total_Elapsed_Time(int pid);

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int StartTime(int pid);

// Generic Functions to minimize repetitive code
template <typename T> T findValueByKey(std::string const &keyFilter, 
                                       std::string const &filename);

template <typename T> T getValueOfFile(std::string const &filename);

};  // namespace LinuxParser

#endif
#include "processor.h"


using std::string; 

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    int user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice; 
    string cpu; 
    float res = 0;

    std::ifstream filestream(LinuxParser::kProcDirectory + 
                             LinuxParser::kStatFilename);
    if (filestream.is_open()){
        string line; 
        std::getline(filestream, line);
        std::istringstream linestream(line);

        linestream>>cpu>>user>>nice>>system>>idle>>iowait>>irq>>softirq>>steal>>guest>>guest_nice;

        int usertime = user - guest;
        int nicetime = nice - guest_nice;
        int idlealltime = idle + iowait;
        int systemalltime = system + irq + softirq;
        int virtualtime = guest + guest_nice;
        int totaltime = usertime+nicetime+systemalltime+idlealltime+steal+virtualtime;

        int nonidle = systemalltime + steal + usertime + nicetime + virtualtime;

        res = nonidle/totaltime;

    }
    return res;

}

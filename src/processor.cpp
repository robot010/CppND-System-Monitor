#include "processor.h"
#include <iostream>

using std::string; 

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    float user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice; 
    string cpu; 
    float res = 0;

    std::ifstream filestream(LinuxParser::kProcDirectory + 
                             LinuxParser::kStatFilename);
    if (filestream.is_open()){
        string line; 
        std::getline(filestream, line);
        std::istringstream linestream(line);

        linestream>>cpu>>user>>nice>>system>>idle>>iowait>>irq>>softirq>>steal>>guest>>guest_nice;

        float total_idle = idle + iowait;
        float total_nonidle = user+nice+system+irq+softirq+steal;

        res = (total_nonidle)/(total_idle+total_nonidle);

    }

    return res;

}

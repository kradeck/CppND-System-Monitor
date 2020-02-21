#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;


System::System() :
    operating_system_{LinuxParser::OperatingSystem()},
    kernel_{LinuxParser::Kernel()}
{
    // The parameters loaded in the ctor are constant, and rather
    // they won't change during the program execution (like the OS name).
    // Summarizing we want to read them once.
}

// DONE: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() 
{ 
    std::vector<int> pids{LinuxParser::Pids()};

    for(auto& proc : processes_)
    {
        proc.IsOld(true);
    }

    for(auto pid : pids)
    {
        bool exists{false};
        for(auto& proc : processes_)
        {
            if(proc.Pid() == pid)
            {
                proc.IsOld(false);
                exists = true;
                proc.Update();
                break;
            }
        }

        if(!exists)
        {
            // create the new one
            Process p(pid);
            processes_.push_back(p);
        }        
    }

    std::remove_if(processes_.begin(), processes_.end(), [](const auto& p){ return p.IsOld(); });
    std::sort(processes_.begin(), processes_.end(),[](const auto& l, const auto& r) {return l<r;});
    return processes_; 
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { return kernel_; }

// DONE: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DONE: Return the operating system name
std::string System::OperatingSystem() { return operating_system_; }

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// DONE: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// DONE: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : 
    pid_{pid},
    uid_{LinuxParser::Uid(pid)},
    cmdline_{LinuxParser::Command(pid)}  
{    
    user_ = LinuxParser::User(std::stol(uid_));
    Update();
}

void Process::Update()
{
    ram_mb_ = LinuxParser::Ram(pid_);
    uptime_ = LinuxParser::UpTime(pid_);
    
    long jiffies{LinuxParser::ActiveJiffies(pid_)};
    long sys_uptime{LinuxParser::UpTime()};
    long seconds{sys_uptime - uptime_};
    cpu_ = static_cast<float>(jiffies) / sysconf(_SC_CLK_TCK) / seconds;
}

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() { return cpu_; }

// DONE: Return the command that generated this process
string Process::Command() { return cmdline_; }

// DONE: Return this process's memory utilization
string Process::Ram() { return ram_mb_; }

// DONE: Return the user (name) that generated this process
string Process::User() { return user_; }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const 
{ 
    return  a.cpu_ < this->cpu_; 
}
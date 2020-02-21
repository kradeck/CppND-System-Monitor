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
  string os{}, kernel{}, version{};
  string line{};
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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{
  string mem_total_str{}, mem_total{}, mem_free_str{}, mem_free{};
  string line{};
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);

  std::getline(filestream, line);
  std::istringstream linestream(line);
  linestream >> mem_total_str >> mem_total;

  std::getline(filestream, line);
  linestream.str(line);
  linestream >> mem_free_str >> mem_free;

  float m_total{stof(mem_total)};
  float m_free{stof(mem_free)};
  
  return ((m_total - m_free) / m_total); 
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() 
{ 
  string line{}, uptime{}, idle_time{};
  std::ifstream filestream(kProcDirectory + kUptimeFilename);

  std::getline(filestream, line);
  std::istringstream linestream(line);
  
  linestream >> uptime >> idle_time;
  
  return std::stol(uptime);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() 
{ 
  std::ifstream filestream(LinuxParser::kProcDirectory + 
                           LinuxParser::kStatFilename);

  std::string line{}, tmp{};

  // take only the first line with cpu summation
  std::getline(filestream, line);               
  std::istringstream stream(line);

  std::vector<string> cpu_pack{};

  while(!stream.eof())
  {
    stream >> tmp;
    cpu_pack.push_back(tmp);
  };
  return cpu_pack; 
}

namespace detail{
int ParseProcStat(std::string searched_variable)
{
  std::ifstream filestream(LinuxParser::kProcDirectory + 
                           LinuxParser::kStatFilename);

  string line{}, key{}, value{};

  while (std::getline(filestream, line)) 
  {
    std::istringstream linestream(line);
    while (linestream >> key >> value) 
    {
      if (key == searched_variable) 
      {
        return stof(value);
      }
    }
  }
  return 0.f; // pedantic, probably better to throw an exception
              // in case if the file is crashed
} 
} // namespace detail

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{ 
  return detail::ParseProcStat("processes"); 
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{ 
  return detail::ParseProcStat("procs_running"); 
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) 
{ 
  std::ifstream filestream(LinuxParser::kProcDirectory + 
                           std::to_string(pid) +
                           LinuxParser::kCmdlineFilename);

  string line{};
  std::getline(filestream, line); 
  return line;
}

namespace detail{
string ParseProcPidStatus(const int pid, std::string&& searched_variable)
{
  std::ifstream filestream(LinuxParser::kProcDirectory + 
                           std::to_string(pid) +
                           LinuxParser::kStatusFilename);

  string line{}, key{}, value{};

  // variable names in /proc/[pid]/status contains ':' at the end 
  if(searched_variable.back() != ':')
  {
    searched_variable += ":";
  }

  while (std::getline(filestream, line)) 
  {
    std::istringstream linestream(line);
    while (linestream >> key) 
    {
      if (key == searched_variable) 
      {
        linestream >> value;
        return value;
      }
    }
  }
  return {}; // pedantic, probably better to throw an exception
             // in case if the file is crashed
} 
} // namespace detail

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) 
{ 
  string ram_kb{detail::ParseProcPidStatus(pid, "VmSize")};
  if(ram_kb == "")
    return {};
  float ram_mb{0.001f * stof(ram_kb)};
  return to_string(ram_mb);
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) 
{ 
  return detail::ParseProcPidStatus(pid, "Uid"); 
}

// TODO: Read and return the user associated with a process
string LinuxParser::User(int pid) 
{ 
  std::ifstream filestream(kPasswordPath);

  string line{}, user{}, x{}, pid_str{};
  string proc_pid{to_string(pid)};
  while (std::getline(filestream, line)) 
  {
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    linestream >> user >> x >> pid_str;

    if (proc_pid == pid_str) 
    {
      return user;
    }

  }
  return string(); 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) 
{ 
  std::ifstream filestream(LinuxParser::kProcDirectory + 
                           std::to_string(pid) +
                           LinuxParser::kStatFilename);

  string line{}, value{};

  unsigned time_pos{22}; 
  unsigned iter{0};

  std::getline(filestream, line);
  std::istringstream linestream(line);

  while(iter++ < time_pos)
  {
    linestream >> value;
  } 
  
  float uptime{std::stof(value)/sysconf(_SC_CLK_TCK)};

  return static_cast<long>(uptime);
}
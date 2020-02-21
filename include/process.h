#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);

  int Pid();                               // DONE: See src/process.cpp
  std::string User();                      // DONE: See src/process.cpp
  std::string Command();                   // DONE: See src/process.cpp
  float CpuUtilization();                  // DONE: See src/process.cpp
  std::string Ram();                       // DONE: See src/process.cpp
  long int UpTime();                       // DONE: See src/process.cpp
  bool operator<(Process const& a) const;  // DONE: See src/process.cpp

  bool IsOld() const { return is_old_; }
  void IsOld(bool fresh) { is_old_ = fresh; } 

  void Update();
  // DONE: Declare any necessary private members
 private:
    bool is_old_{false};
    int pid_{};
    std::string uid_{}; // It's good to keep this.
    std::string ram_mb_{};
    std::string user_{};
    std::string cmdline_{};
    long int uptime_{};
    float cpu_{};
};

#endif
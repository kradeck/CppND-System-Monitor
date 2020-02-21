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

  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  bool IsOld() const { return is_old_; }
  void IsOld(bool fresh) { is_old_ = fresh; } 

  void Update();
  // TODO: Declare any necessary private members
 private:
    bool is_old_{false};
    int pid_{};
    std::string uid_{}; // It's good to keep this.
    std::string ram_mb_{};
    std::string user_{};
    std::string cmdline_{};
    long int uptime_{};
};

#endif
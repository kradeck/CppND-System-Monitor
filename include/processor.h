#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <string>

class Processor {
 public:
    float Utilization();  // DONE: See src/processor.cpp

  // Both setters and getters are not provided.
  // We don't want to read the values separately, also we don't want
  // to set any of the cpu value indyvidually.
  // No additional input validation is provided as the numbers in
  // /proc/stat cannot be negative (I quess...).

  // DONE: Declare any necessary private members
 private:
    unsigned int user_{};
    unsigned int nice_{};
    unsigned int system_{};
    unsigned int idle_{};
    unsigned int iowait_{};
    unsigned int irq_{};
    unsigned int softirq_{};
    unsigned int steal_{};
    unsigned int guest_{};
    unsigned int guest_nice_{};

    void _unpack(const std::vector<std::string>& pack);
    float _calculate_utilization() const;
};

#endif
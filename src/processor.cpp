#include "processor.h"

#include "linux_parser.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization()
{
    auto cpu_values{LinuxParser::CpuUtilization()};

    _unpack(cpu_values);
    
    return _calculate_utilization();
}

void Processor::_unpack(const std::vector<std::string>& pack)
{
    unsigned pack_size{11}; // "cpu" + 10 numbers

    if(pack.at(0) != "cpu" || pack.size() != pack_size)
        throw std::runtime_error("missed cpu information");

    user_       = std::stoul(pack.at(1));
    nice_       = std::stoul(pack.at(2));
    system_     = std::stoul(pack.at(3));
    idle_       = std::stoul(pack.at(4));
    iowait_     = std::stoul(pack.at(5));
    irq_        = std::stoul(pack.at(6));
    softirq_    = std::stoul(pack.at(7));
    steal_      = std::stoul(pack.at(8));
    guest_      = std::stoul(pack.at(9));
    guest_nice_ = std::stoul(pack.at(10));
};

float Processor::_calculate_utilization() const
{
    // source: https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
    // without the bonus task, 
    // only the used resources according to the total resources since the boot of the system
    float idle{static_cast<float>(idle_ + iowait_)};
    float non_idle{static_cast<float>(user_ + nice_ + system_ + irq_ + softirq_ + steal_)};
    float total{static_cast<float>(idle + non_idle)};

    return non_idle / total;
}
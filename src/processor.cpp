#include "processor.h"
#include "linux_parser.h"
#include <string>
#include <iostream>
#include <chrono>
#include <thread>


Processor::Processor() : 
  prevIdle_(LinuxParser::IdleJiffies()),
  prevNonIdle_(LinuxParser::ActiveJiffies()),
  prevTotal_(prevIdle_ + prevNonIdle_) {}

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  
  std::vector<long> cpuStats = LinuxParser::CpuUtilization();
  long idleTime = LinuxParser::IdleJiffies();
  long nonIdleTime = LinuxParser::ActiveJiffies(); 
  long total = idleTime + nonIdleTime;
  long deltaIdle = idleTime - prevIdle_;
  long deltaTotal = total - prevTotal_;
  
  //update previous times with current times
  prevIdle_ = idleTime;
  prevNonIdle_ = nonIdleTime;
  prevTotal_ = total;
  
  if (deltaTotal == 0) {
    return 0.0f;
  }
  else {
    float deltaActiveTime = (float)(deltaTotal - deltaIdle);
  	float cpuUsage = deltaActiveTime/deltaTotal;
  	return cpuUsage;
  }
}
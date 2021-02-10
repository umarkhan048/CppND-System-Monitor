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

// TODO: Return the system's CPU
Processor& System::Cpu() {
  
  
  return cpu_; 
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() {
  
  std::string kernel =  LinuxParser::Kernel();
  return kernel; 
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { 
  
  float testFloat = LinuxParser::MemoryUtilization();
  return testFloat; 
}

// TODO: Return the operating system name
std::string System::OperatingSystem() { 
  
  std::string pretty_name = LinuxParser::OperatingSystem();
  return pretty_name; 
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return 0; }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return 0; }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return 0; }
#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Constructor
Process::Process(int processPID) : processPID_(processPID) {}

// TODO: Return this process's ID
int Process::Pid() { 

  return processPID_; 
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
  
  float Hz = (float)sysconf(_SC_CLK_TCK);
  auto processActiveJiffies = LinuxParser::ActiveJiffies(processPID_);
  auto processUpTime = Process::UpTime();
  auto systemUpTime = LinuxParser::UpTime();
  auto seconds = systemUpTime - (processUpTime/Hz);
  float cpu_usage =  (processActiveJiffies/Hz)/seconds;
  return cpu_usage; 
}

// TODO: Return the command that generated this process
string Process::Command() { 
  
  return LinuxParser::Command(processPID_); 
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
  
  return LinuxParser::Ram(processPID_); 
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
  return LinuxParser::User(processPID_); 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
  
  return LinuxParser::UpTime(processPID_); 
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }
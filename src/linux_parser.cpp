#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
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
    filestream.close();
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os;
  string kernel;
  string version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
    stream.close();
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  
  string line;
  string key;
  string value;
  string unit;
  string memTotal;
  string memFree;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {

    while (!filestream.eof()) {
       std::getline(filestream, line);
       std::istringstream linestream(line);
       linestream >> key >> value >> unit;
       if (key == "MemTotal:") {
       memTotal = value;
       }
       
       if (key == "MemFree:") {
       memFree = value;
       }
    }
    filestream.close();
  }
  float memUtil = (std::stof(memTotal) - std::stof(memFree))/std::stof(memTotal);
  return memUtil; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  
  string upTime;
  string idleTime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTime >> idleTime;
    stream.close();
  }

  return std::stol(upTime);
}

// TODO: Read and return the number of jiffies for the system
//long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  string line;
  string value;
  vector<string> lineElements;
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
	while (linestream >> value) {
      lineElements.push_back(value);
    }
    stream.close();
  }
  if (!lineElements.empty()) {
    float utime = std::stof(lineElements[UTIME]);
    float stime = std::stof(lineElements[STIME]);
    float cutime = std::stof(lineElements[CUTIME]);
    float cstime = std::stof(lineElements[CSTIME]);
  	return utime + stime + cutime + cstime;
  }
  else {
    return 0;
  }
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  
  vector<long> cpuStats = CpuUtilization();
  // user + nice + system + irq + softirq + steal
   return cpuStats[USER] + cpuStats[NICE] + cpuStats[SYS] + cpuStats[IRQ] 
     + cpuStats[SOFTIRQ] + cpuStats[STEAL];
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  
  vector<long> cpuStats = CpuUtilization();
  return cpuStats[IDLE] + cpuStats[IOWAIT]; 
}

// TODO: Read and return CPU utilization
vector<long> LinuxParser::CpuUtilization() { 
  
  string key;
  string line;
  string user; 
  string nice; 
  string system;
  string idle;
  string iowait;
  string irq;
  string softirq;
  string steal;
  string guest;
  string guest_nice;
  std::ifstream stream(kProcDirectory + kStatFilename);
  vector<long> processor_stat;
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key >> user >> nice >> system >> idle >> iowait >>
          irq >> softirq >> steal >> guest >> guest_nice;
    stream.close();
  }
  processor_stat = {stol(user), stol(nice), stol(system), stol(idle), 
                   stol(iowait), stol(irq), stol(softirq), stol(steal), 
                   stol(guest), stol(guest_nice)};
  return processor_stat; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  
  string line;
  string key;
  string value;
  int numProc;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {

    while (!stream.eof()) {
       std::getline(stream, line);
       std::istringstream linestream(line);
       linestream >> key >> value;
       if (key == "processes") {
       numProc = stoi(value);
       }
    }
    stream.close();
  }
  
  return numProc;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
    
  string line;
  string key;
  string value;
  int numProcRun;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (!stream.eof()) {
       std::getline(stream, line);
       std::istringstream linestream(line);
       linestream >> key >> value;
       if (key == "procs_running") {
          numProcRun = std::stoi(value);
       }
    }
    stream.close();
  }
  return numProcRun;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    stream.close();
  }
  return line; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  
  string line;
  string key;
  string value;
  string ramUsageKB;
  string ramUsageMB;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
        	ramUsageKB = value;
        }
      }
    }
    stream.close();
  }
  ramUsageMB = std::to_string((stol(ramUsageKB))/1024);
  return ramUsageMB; 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  
  string line;
  string key;
  string value;
  string uid;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      //std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> uid) {
        if (key == "Uid:") {
          value = uid;
        }
      }
    }
    stream.close();
  }
  return value; 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  
  string uid = LinuxParser::Uid(pid);
  string line;
  string key;
  string value;
  string x;
  string user_full;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> value >> x >> key) {
        if (key == uid) {
          user_full = value;
        }
      }
    }
    stream.close();
  }
  return user_full; 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  
  string line;
  string value;
  long upTimePID;
  vector<string> lineElements;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    
	while (linestream >> value) {
      lineElements.push_back(value);
    }
    stream.close();
  }
  upTimePID = UpTime() - stol(lineElements[STARTTIME])/sysconf(_SC_CLK_TCK);
  return upTimePID; 
}
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

#define UTIME     13
#define STIME     14
#define CUTIME    15
#define CSTIME    16
#define STARTTIME 21

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
  string os, kernel, version;
  string line;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  
  string line;
  string key, value, unit;
  string memTotal, memFree;
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
  }
  float memUtil = (std::stof(memTotal) - std::stof(memFree))/std::stof(memTotal);
  return memUtil; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  
  string upTime, idleTime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTime >> idleTime;
  }

  return std::stol(upTime);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  string line, value;
  vector<string> lineElements;
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
	while (linestream >> value) {
      lineElements.push_back(value);
    }
  }
  float utime = std::stof(lineElements[UTIME]);
  float stime = std::stof(lineElements[STIME]);
  float cutime = std::stof(lineElements[CUTIME]);
  float cstime = std::stof(lineElements[CSTIME]);

  return utime + stime + cutime + cstime; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  
  string line, key, value;
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
  }
  
  return numProc;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
    
  string line, key, value;
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
  }
  return line; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  
  string line, key, value, ramUsage;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        ramUsage = value;
      }
    }
  }
      
  return ramUsage; 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  
  string line, key, value, uid;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> uid) {
        if (key == "Uid") {
          uid = value;
        }
      }
    }
  }
  return uid; 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  
  string uid = LinuxParser::Uid(pid);
  string line, key;
  string value, x, user_full;
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
  }
  return user_full; 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  
  string line, value;
  vector<string> lineElements;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    
	while (linestream >> value) {
      lineElements.push_back(value);
    }
  }
  return std::stol(lineElements[21]); 
}
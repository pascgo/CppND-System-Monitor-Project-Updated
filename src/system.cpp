/*
  system cpp file 
  
  Evolution : 
      14 Feb 2023 :  Using emplace_back statement with const 
      14 Feb 2023 :  Using namespace std
      14 Feb 2023 :  Remove of TODO comment 
*/

#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

#include "linux_parser.h"
#include <iostream>

// using directive 
using namespace std;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() {
  processes_.clear();
  
 vector <int>  pids = LinuxParser::Pids();
  
  for (int pid: pids) {
        processes_.emplace_back(pid);
  }
  
  std::sort(processes_.begin(), processes_.end(), [](Process const& a, Process const& b) { return a < b;});
  return processes_;
}

// Return the system's kernel identifier (string)
string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
  
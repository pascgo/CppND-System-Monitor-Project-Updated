/*
  linux_parser.h file 
  
   	14 Feb 2023 : Add Constants for entity declaration
    09-Feb-2023 : Creation 
*/

#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};
  
// Entity 
const std::string filterProcesses("processes");
const std::string filterRunningProcesses("procs_running");
const std::string filterMemTotalString("MemTotal");
const std::string filterMemFreeString("MemFree");
const std::string filterCpu("cpu");
const std::string filterUID("Uid");
const std::string filterProcMem("VmSize"); 
  
// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
  
// process
float CpuUtilization(int pid);
long Jiffies(std::string cpu_number);
long ActiveJiffies(int pid);
long ActiveJiffies(std::string cpu_number);
long IdleJiffies(std::string cpu_number);
//  accessor
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);

};  // namespace LinuxParser

#endif
/*
  linux_parser.cpp file 
  
   	15 Feb 2023 : Add Constants for entity declaration
    14 Feb 2023 : Remove Debug Statement
    14 Feb 2023 : Close Filestream and print file not found in case of 
    14 Feb 2023 : Remove ToDo Statement and some comments
    14 Feb 2023 : Return Statement for uptime
    09-Feb-2023 : Creation 
*/


#include "linux_parser.h"
#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>

// using directive 
using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::all_of;
using std::istringstream;
using std::replace;
using std::stof;
using std::stoi;
using std::istream_iterator;
using std::ifstream;
using std::cout;


#define printVariableNameAndValue(x) cout << "The name of variable ** " << (#x) << " ** and the value of variable is => " <<x<<"\n"


// Read and Return Operating system data from the filesystem
string LinuxParser::OperatingSystem() {
  string value="";
  string line;
  string key;

  string filename = LinuxParser::kOSPath;
  
  ifstream filestream(filename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      replace(line.begin(), line.end(), ' ', '_');
      replace(line.begin(), line.end(), '=', ' ');
      replace(line.begin(), line.end(), '"', ' ');
      istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          replace(value.begin(), value.end(), '_', ' ');
          filestream.close();
          return value;
        }
      }
    }
    filestream.close();
  }else{
    cout << "FILE NOT FOUND \n";    	// Should never happen  
  }										// i.e. no need to output filename
  return value;
}

// Read and Return Kernel data  from the filesystem
string LinuxParser::Kernel() {
  string os="", kernel="", ver="";
  string line;
  string filename = LinuxParser::kProcDirectory + LinuxParser::kVersionFilename;
  ifstream filestream(filename);
  if (filestream.is_open()) {
    getline(filestream, line);
    istringstream linestream(line);
    linestream >> os >> kernel >> ver;
   
    filestream.close();
  }else{
    cout << "FILE NOT FOUND \n";    	// Should never happen  
  }										// i.e. no need to output filename
  return kernel + " " + ver;
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
      if (all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}
// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memTotal = 0.0;
  float memFree = 0.0;
  string line;
  string key;
  string value;
  ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      replace(line.begin(), line.end(), ':', ' ');
      istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == filterMemTotalString ) {
          memTotal = stof(value);
        }
        if (key == filterMemFreeString ) {
          memFree = stof(value);
          break;
        }
      }
    }
    filestream.close();
  }else{
    cout << "FILE NOT FOUND \n";    	// Should never happen  
  }	
  return (memTotal - memFree) / memTotal;
}

// Read and return the system uptime
//
//  Information about system up time exists in the /proc/uptime file. 
//  This file contains two numbers (values in seconds): 
//   - the uptime of the system, including time spent in suspend
//   - the amount of time spent in the idle process.
//
long LinuxParser::UpTime() {
  long int upTime = 0;
  string upTimeStr = "";
  string line;
  string value1;
  string value2;
  string filename = LinuxParser::kProcDirectory + LinuxParser::kUptimeFilename;
  ifstream filestream(filename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      istringstream linestream(line);
      while (linestream >> value1 >> value2) {
        upTimeStr = value1;						// Get the first value 
      }
    }
    upTime = stol(upTimeStr);				// time in second as an integer
    
    filestream.close();
  }else{
    cout << "FILE NOT FOUND "<< filename <<" \n";
  }
  return upTime;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies(string cpu_number) { 
  long jiffies;
    jiffies = LinuxParser::ActiveJiffies(cpu_number) + LinuxParser::IdleJiffies(cpu_number);
  return jiffies;
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(string cpu_number ) {
  long activ_jiffies = 0;
  string line;
  string key;
  vector<long> values_T(8);

  string userS = "", niceS = "", systemS = "", idleS = "", iowaitS = "",
              irqS = "", softirqS = "", stealS = "";

  string filename = LinuxParser::kProcDirectory + LinuxParser::kStatFilename;
  ifstream filestream(filename);
  if (filestream.is_open()) {
        while (getline(filestream, line)) {
               istringstream linestream(line);
               while (linestream >> key >> userS >> niceS >> systemS >> idleS >> iowaitS >> irqS >> softirqS >> stealS) {
                    if (key == cpu_number ) {
                      	// Read Data with float conversion 
         				values_T[LinuxParser::kUser_] = stoi(userS);
         				values_T[LinuxParser::kNice_] = stoi(niceS);
          				values_T[LinuxParser::kSystem_] = stoi(systemS);
          				values_T[LinuxParser::kIdle_] = stoi(idleS);
          				values_T[LinuxParser::kIOwait_] = stoi(iowaitS);
          				values_T[LinuxParser::kIRQ_] = stoi(irqS);
          				values_T[LinuxParser::kSoftIRQ_] = stoi(softirqS);
          				values_T[LinuxParser::kSteal_] = stoi(stealS);
						//Compute active jiffies 
         				 activ_jiffies = long(values_T[LinuxParser::kUser_]
                        +values_T[LinuxParser::kNice_]
                        +values_T[LinuxParser::kSystem_]                        
                        +values_T[LinuxParser::kIRQ_]
                        +values_T[LinuxParser::kSoftIRQ_]
                        +values_T[LinuxParser::kSteal_]);
                    }
               }
        }
        filestream.close();
  }else{
    cout << "FILE NOT FOUND "<< filename <<" \n";
  }
  return activ_jiffies;
}

// Read and return the number of idle jiffies for the system 
long LinuxParser::IdleJiffies(string cpu_number) { 
  long iddle_jiffies = 0;
  string line;
  string key;
  vector<long> values_T(8);

  string userS = "", niceS = "", systemS = "", idleS = "", iowaitS = "",
              irqS = "", softirqS = "", stealS = "";

  string filename = LinuxParser::kProcDirectory + LinuxParser::kStatFilename;
  ifstream filestream(filename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      istringstream linestream(line);
      while (linestream >> key >> userS >> niceS >> systemS >> idleS >>
             iowaitS >> irqS >> softirqS >> stealS) {
        if (key == cpu_number) {
          values_T[LinuxParser::kUser_] = stoi(userS);
          values_T[LinuxParser::kNice_] = stoi(niceS);
          values_T[LinuxParser::kSystem_] = stoi(systemS);
          values_T[LinuxParser::kIdle_] = stoi(idleS);
          values_T[LinuxParser::kIOwait_] = stoi(iowaitS);
          values_T[LinuxParser::kIRQ_] = stoi(irqS);
          values_T[LinuxParser::kSoftIRQ_] = stoi(softirqS);
          values_T[LinuxParser::kSteal_] = stoi(stealS);
                   
          iddle_jiffies = (values_T[LinuxParser::kIdle_] 
                       	+values_T[LinuxParser::kIOwait_]);                             
        }
      }
    }
    filestream.close();
  }else{
      cout << "FILE NOT FOUND \n";
  }
  return iddle_jiffies;
}
        
// Read and return the number of active jiffies for a PID
// 
// long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }
// (14) utime Amount of time that this process has been scheduled in user mode
// (15) stime Amount of time that this process has been scheduled in kernel mode
// (16) cutime Amount of time that this process's waited-for children have been scheduled in user mode
// (17) cstime Amount of time that this process's waited-for children have been scheduled in kernel mode 
//
long LinuxParser::ActiveJiffies(int pid ) {
  long activ_jiffies = 0;
  string line;
  string filename;
  
  filename = LinuxParser::kProcDirectory + to_string(pid)+ LinuxParser::kStatFilename; 
  ifstream filestream(filename);

  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      replace(line.begin(), line.end(), '(', '_');
      replace(line.begin(), line.end(), ')', '_');
      replace(line.begin(), line.end(), '-', '_');
      istringstream linestream(line);
      istream_iterator<string> beg(linestream), end;

      vector<string> vec(beg, end);
      // Compute active jiffies with integer conversion 
      activ_jiffies = (float)(stoi(vec[13])+stoi(vec[14])+stoi(vec[15])+stoi(vec[16]));
 
    }
    filestream.close();
  }else{
      cout << "FILE NOT FOUND \n";
  }
  return activ_jiffies;
}

// Read and return CPU utilization
//
//   https://man7.org/linux/man-pages/man5/proc.5.html
//   
// (22) starttime The time the process started after system boot
//     these values is expressed in clock ticks (divide by sysconf(_SC_CLK_TCK)).
//
float LinuxParser::CpuUtilization(int pid) {
  float cpu_util = 0;
  float cpu_time = 0;
  float cpu_seconds =0;

  long up_time = LinuxParser::UpTime();
  string line;
  string key, value;
  string filename;
  
  filename = LinuxParser::kProcDirectory + to_string(pid)+ LinuxParser::kStatFilename; 
  
  ifstream filestream(filename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      replace(line.begin(), line.end(), '(', '_');
      replace(line.begin(), line.end(), ')', '_');
      replace(line.begin(), line.end(), '-', '_');
      istringstream linestream(line);
      istream_iterator<string> beg(linestream), end;

      vector<string> vec(beg, end);
      
      cpu_time = (float)(stoi(vec[13])+stoi(vec[14])+stoi(vec[15])+stoi(vec[16]))/sysconf(_SC_CLK_TCK);

      cpu_seconds = (float)(up_time) - (float)(stoi(vec[21]))/sysconf(_SC_CLK_TCK);
      cpu_util = cpu_time / cpu_seconds;
    }
    filestream.close();
  }else{
      cout << "FILE NOT FOUND \n";
  }
  return cpu_util;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int totalProcesses = 0;
  string line;
  string key;
  string value;
  ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == filterProcesses) {
          totalProcesses = stoi(value);
          break;
        }
      }
    }
    filestream.close();
  }else{
      cout << "FILE NOT FOUND \n";
  }
  return totalProcesses;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int runningProcesses = 0;
  string line;
  string key;
  string value;
  ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == filterRunningProcesses) {
          runningProcesses = stoi(value);
          break;
        }
      }
    }
    filestream.close();
  }else{
      cout << "FILE NOT FOUND \n";
  }
  return runningProcesses;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line="";
  ifstream filestream(kProcDirectory + to_string(pid) +
                           kCmdlineFilename);
  if (filestream.is_open()) {
    getline(filestream, line);
    filestream.close();
  }else{
      cout << "FILE NOT FOUND \n";
  }
  return line;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  int ram = 0;
  string line;
  string key, value;
  ifstream filestream(kProcDirectory + to_string(pid) +
                           kStatusFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      replace(line.begin(), line.end(), ':', ' ');
      istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize") {
          ram = stoi(value) / 1000;
        }
      }
    }
    filestream.close();
  }else{
      cout << "FILE NOT FOUND \n";
  }
  return to_string(ram);
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string uid = "";
  string line, key, value;
  ifstream filestream(kProcDirectory + to_string(pid) +
                           kStatusFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      replace(line.begin(), line.end(), ':', ' ');
      istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == filterUID) {
          uid = value;
          break;
        }
      }
    }
    filestream.close();
  }else{
      cout << "FILE NOT FOUND \n";
  }
  return uid;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string user = "u";
  string uid_of_pid = Uid(pid);
  string line;
  string first_val, x, sec_val;
  ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      replace(line.begin(), line.end(), ':', ' ');
      istringstream linestream(line);
      while (linestream >> first_val >> x >> sec_val) {
        if (sec_val == uid_of_pid) {
          user = first_val;
          break;
        }
      }
    }
    filestream.close();
  }else{
      cout << "FILE NOT FOUND \n";
  }
  return user;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  long clockTicks = 0;
  string line;
  string key, value;
  ifstream filestream(kProcDirectory + to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      replace(line.begin(), line.end(), '(', '_');
      replace(line.begin(), line.end(), ')', '_');
      replace(line.begin(), line.end(), '-', '_');
      istringstream linestream(line);
      istream_iterator<string> beg(linestream), end;
      vector<string> vec(beg, end);
      clockTicks = stol(vec[21]);
    }
    filestream.close();
  }else{
      cout << "FILE NOT FOUND \n";
  }
  return clockTicks / sysconf(_SC_CLK_TCK);
}
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

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
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
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
  float mTotal;
  float mFree;
  float a;
  string c;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open()) {
    while(std::getline(stream,line)){
    std::istringstream linestream(line);
    while(linestream >> c >> a){
      if(c == "MemTotal:"){
        mTotal = a;
        
      }
      if(c == "MemFree:"){
          mFree = a;
      }
    }
    }
  }
  return (mTotal - mFree) / mTotal; }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()) {
    std::getline(stream,line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return uptime; }


// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();  }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string line,token;
  long utime = 0;
  long stime = 0;
  long cutime = 0;
  long cstime = 0;
  long act_jif;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if(stream.is_open()){
    std::getline(stream,line);
    std::istringstream linestream(line);
    for(int i=1; i<=17; i++){
      linestream >> token;
      if(i==14)
        utime = std::stol(token);
      if(i==15)
        stime = std::stol(token);
      if(i==16)
        cutime = std::stol(token);
      if(i==17)
        cstime = std::stol(token);
    }
    return act_jif = (utime + stime + cutime + cstime);
  
  }
  
  return act_jif = (utime + stime + cutime + cstime); }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> active = LinuxParser::CpuUtilization();
  long act_jif = 0;
  act_jif = std::stol(active[0]) + std::stol(active[1]) + std::stol(active[2]) + std::stol(active[5]) + std::stol(active[6])  + std::stol(active[7]);
  return act_jif;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> idle = LinuxParser::CpuUtilization();
  long idl_jif = 0;
  idl_jif = std::stol(idle[3]) + std::stol(idle[4]);
  return idl_jif;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> token;
  string key,value,line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    std::getline(stream,line);
    std::istringstream linestream(line);
    while(linestream >> key){
      if(key == "cpu"){
        while(linestream >>value){ 
          token.push_back(value);
        }
        return token;
      }
      
    }
    
  }
  return token; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line,key;
  int process;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream,line)){
    std::istringstream linestream(line);
    while(linestream >> key >> process){
      if(key == "processes")
        return process;
        
    }
    }
  }
  return process;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line,key;
  int process;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream,line)){
    std::istringstream linestream(line);
    while(linestream >> key >> process){
      if(key == "procs_running")
        return process;
        
    }
    }
  }
  return process;
}
  
  
   

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if(stream.is_open()){
  std::getline(stream,line);
  }
  
  return line; }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line,key,token;
  std::ifstream stream(kProcDirectory + to_string(pid) + 
kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream,line)){
    std::istringstream linestream(line);
    while(linestream >> key >> token){
      if(key == "VmSize:")
        return to_string(stof(token)/1024.0);
    }
    }
    
  }
  
  return to_string(stoi(token)/1024); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line,key,token;
  std::ifstream stream(kProcDirectory + to_string(pid) + 
kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream,line)){
    std::istringstream linestream(line);
    while(linestream >> key >> token){
      if(key == "Uid:")
        return token;
    }
    }
    
  }
  
  return token;  }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line,token1,token2,token3;
  std::ifstream stream(kPasswordPath);
  string Uid = LinuxParser::Uid(pid);
  if(stream.is_open()){
    while(std::getline(stream,line)){
      std::replace(line.begin(),line.end(),':',' ');
      std::istringstream linestream(line);
      while(linestream >> token1 >> token2 >> token3){
        if(token3 == Uid)
          return token1;
      }
    }
  }
  
  return token1; }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line,token;
  int value = 1;
  long uptime;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if(stream.is_open()){
    std::getline(stream,line);
    std::istringstream linestream(line);
    while(linestream >> token){
      if(value == 22){
        return uptime = UpTime() - std::stol(token) /sysconf(_SC_CLK_TCK);
      }
      value++;
        
    }
  }
  
  return uptime; }

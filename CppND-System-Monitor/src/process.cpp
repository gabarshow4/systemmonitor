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

// TODO: Return this process's ID
Process::Process(int Pid):pid_(Pid){}
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  string line,token;
  long utime = 0;
  long stime = 0;
  long cutime = 0;
  long cstime = 0;
  float starttime = 0.0;
  float total_time;
  float elapsed_time;
  
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid_) + LinuxParser::kStatFilename);
  if(stream.is_open()){
    std::getline(stream,line);
    std::istringstream linestream(line);
    for(int i=1; i<=22; i++){
      linestream >> token;
      if(i==14)
        utime = std::stol(token);
      if(i==15)
        stime = std::stol(token);
      if(i==16)
        cutime = std::stol(token);
      if(i==17)
        cstime = std::stol(token);
      if(i==22)
        starttime = std::stof(token);
    }
  }
  total_time = float(utime + stime + cutime + cstime);
  elapsed_time = float(LinuxParser::UpTime() - (starttime/sysconf(_SC_CLK_TCK)));
  usage = (total_time/sysconf(_SC_CLK_TCK))/elapsed_time;
  return usage;}


// TODO: Return the command that generated this process
string Process::Command() {
  return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { return a.usage < usage; }

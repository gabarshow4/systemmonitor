#include "processor.h"
#include <string>
#include <vector>
#include "linux_parser.h"
using std::string;
using std::vector;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  vector<string> CPU_values = LinuxParser::CpuUtilization();
  string key,value;
  float user = stof(CPU_values[0]);
  float nice = stof(CPU_values[1]);
  float system = stof(CPU_values[2]);
  float idle = stof(CPU_values[3]);
  float iowait = stof(CPU_values[4]);
  float irq = stof(CPU_values[5]);
  float softirq = stof(CPU_values[6]);
  float steal = stof(CPU_values[7]);
  
  float Previdle = previdle + previowait;
  float Idle = idle + iowait;
  float PrevNonidle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal; 
  float Nonidle = user + nice + system + irq + softirq + steal;
  float Prevtotal = Previdle + PrevNonidle;
  float Total = Idle + Nonidle;
  float Total_ = Total - Prevtotal;
  float Idle_ = Idle - Previdle;
  
  float usage = (Total_ - Idle_)/Total_;
  
  prevuser = user;
  prevnice = nice;
  prevsystem = system;
  previrq = irq;
  prevsoftirq = softirq;
  prevsteal = steal;
  previdle = idle;
  previowait = iowait;
  
  return usage; }
  
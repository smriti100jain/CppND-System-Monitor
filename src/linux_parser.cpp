#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include "linux_parser.h"

using std::stof;
using std::stoi;

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
  string os, kernel,version;
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

float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  string value, unit;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  double Total, Used, Buffer, Cache, free_mem, mem_utilized;
  mem_utilized = 0.0;
  if (filestream.is_open()) {
    try{
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');

      std::istringstream linestream(line);
      linestream >> key >> value >> unit;
        if (key == "MemTotal") {
          Total = std::stold(value);
        }
        else if (key == "MemFree"){
          Used = Total-std::stold(value);
        }
        else if (key == "MemAvailable"){
          continue;
        }
        else if (key == "Buffers"){
          Buffer = std::stold(value);
        }
        else if (key == "Cached"){
          Cache = std::stold(value);
        }
        else { break;}
      }
      free_mem = (Total + Buffer + Cache)-Used;
      mem_utilized = ((Total-free_mem)/Total);
    }
          catch (...){mem_utilized= 0.0;}

      }
      return mem_utilized;
    }


long LinuxParser::UpTime() { 
  std::ifstream filestream(kProcDirectory+kUptimeFilename);
  string line, suspend_process_time, idle_process_time;
  long uptime;
  if (filestream.is_open()){
      std::getline(filestream,line);
      std::istringstream linestream(line);
      try {
      linestream >> suspend_process_time >> idle_process_time;
      uptime = std::stol(suspend_process_time);}
      catch (...){uptime= 0;}

    
  }
  return uptime;
}

vector<string> LinuxParser::CpuUtilization() { 
  vector<string> utilization(10);
  std::ifstream filestream(kProcDirectory+kStatFilename);
  string line, key;
  int count = 0;
  if (filestream.is_open()){
      std::getline(filestream,line);
      std::istringstream linestream(line);
      while (linestream >> key){
        if (key == "cpu"){continue;}
        else{
          utilization[count] = key;
          count = count + 1;
        }
      }
  }

  return utilization;
 }
 
int LinuxParser::TotalProcesses() { std::ifstream filestream(kProcDirectory+kStatFilename);
  string line, key, value;
  int num_processes=0;
  if (filestream.is_open()){
    try {
    while (std::getline(filestream,line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key=="processes"){
        
        num_processes = std::stoi(value);

      }
      else{      continue;}
    }
    }
                catch (...){return 0;}
  }
  return num_processes;
}

int LinuxParser::RunningProcesses() { 
  std::ifstream filestream(kProcDirectory+kStatFilename);
  string line, key, value;
  int num_processes=0;
  if (filestream.is_open()){
    try {
    while (std::getline(filestream,line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key=="procs_running"){
        
        num_processes = std::stoi(value);
      }
      else{      continue;}
    }
    }
        catch (...){return 0;}
  }
  return num_processes;
}

float LinuxParser::CpuUtilization(int pid){
  string line, value;
  long utime, stime, cutime, cstime, starttime;// uptime;
  
  //uptime= UpTime();

  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatFilename);
  
  float cpu_usage= 0.0;
  if (filestream.is_open()){
    try{
        std::getline(filestream,line);
        std::istringstream linestream(line);
        for (int i=0;i<=21;i++){
          linestream >> value;
          if (i==13){ utime= stol(value);}
          if (i==14){ stime= stol(value);}
          if (i==15){ cutime= stol(value);}
          if (i==16){ cstime= stol(value);}
          if (i==21){ starttime= stol(value);}
        }
        long total_time = utime + stime;
        total_time =  total_time + cutime + cstime;
        float seconds = starttime;//uptime - (starttime / sysconf(_SC_CLK_TCK));
        if (seconds>0.0){cpu_usage = ((total_time / sysconf(_SC_CLK_TCK)) / seconds);}
    }
            catch (...){cpu_usage= 0.0;}

        
      }
      return cpu_usage;

 }


string LinuxParser::Command(int pid) {std::ifstream filestream(kProcDirectory+to_string(pid)+kCmdlineFilename);
  string line;
  if (filestream.is_open()){
      std::getline(filestream,line);}
        
  return line; }

string LinuxParser::Ram(int pid) { 
  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatusFilename);
  string line, key, value, ram="";
  if (filestream.is_open()){
      while(std::getline(filestream,line)){
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream linestream(line);
        linestream >> key >> value;
        // I have used VmData insteadh of VmSize
        if (key == "VmData") {ram = to_string((long) (0.001*stof(value)));
        break;}
        }
      }
  return ram;
   }


string LinuxParser::Uid(int pid) { 
  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatusFilename);
  string line, key, value, uid;
  if (filestream.is_open()){
      while(std::getline(filestream,line)){
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream linestream(line);
        linestream >> key >> value;
        if (key == "Uid") {uid = value;break;}
        }
      }
  return uid;
 }

 string LinuxParser::User(int pid) { 
  std::ifstream filestream(kPasswordPath);

  string line, x, uid, user="", curr_uid;
  uid = Uid(pid);
  if (filestream.is_open()){
      while(std::getline(filestream,line)){
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream linestream(line);
        linestream >> user >> x >> curr_uid;
        if (curr_uid == uid) {break;}
        }
      }
  return user;
 }



long LinuxParser::UpTime(int pid) { 
  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatFilename);
  string line, value;
  long uptime;
  if (filestream.is_open()){
        std::getline(filestream,line);
        std::istringstream linestream(line);
        try {
        for (int i=0;i<21;i++){
          linestream >> value;
        }
        linestream >> value;
        uptime = (std::stol(value))/sysconf(_SC_CLK_TCK);
        
        
        }
        catch (...){uptime= 0;}
  }
  return uptime; }
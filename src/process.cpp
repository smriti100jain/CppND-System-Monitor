#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <linux_parser.h>
#include "process.h"
using std::stol;
using std::string;
using std::to_string;
using std::vector;
Process::Process(int pid){
    _pid = pid;
    _user = LinuxParser::User(_pid);
    _command = LinuxParser::Command(_pid);
    _ram = LinuxParser::Ram(_pid);;
    _uptime = LinuxParser::UpTime(_pid);
    _cpu_utilization = LinuxParser::CpuUtilization(_pid);;

}
int Process::Pid() { return _pid; }

float Process::CpuUtilization() { return _cpu_utilization;}

string Process::Command() { return  _command;}

string Process::Ram() { return _ram; }

string Process::User() { return _user; }

long int Process::UpTime() { return _uptime; }


bool Process::operator<(Process const& p) const { 
    float u2 = p._cpu_utilization;
    float u1 = _cpu_utilization;
    return (u1<u2)?true:false; 
    }
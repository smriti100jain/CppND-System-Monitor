#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>
#include <iostream>
#include <ctime>
using std::string;
using std::stof;
using std::stoi;
using std::vector;
float Processor::LongTermUtilization(vector<string> utilization){
    float Jiffies = stof(utilization[0]) + stof(utilization[1]) + stof(utilization[2]) + stof(utilization[3]) + stof(utilization[4])+ stof(utilization[5]) + stof(utilization[6]) + stof(utilization[7]);;
    float IdleJiffies = stof(utilization[3]) + stof(utilization[4]) ;
    float ActiveJiffies = Jiffies - IdleJiffies;
    float utilization_value = ActiveJiffies/Jiffies;
    return utilization_value;

}

float Processor::ShortTermUtilization(vector<string> utilization, vector<string> prev_utilization){
    float utilization_value=0.0;
    
    float PrevIdle = stof(prev_utilization[3]) + stof(prev_utilization[4]);
    float Idle = stof(utilization[3]) + stof(utilization[4]);

    float PrevNonIdle = stof(prev_utilization[0]) + stof(prev_utilization[1]) + stof(prev_utilization[2]) + stof(prev_utilization[5]) + stof(prev_utilization[6]) + stof(prev_utilization[7]);
    float NonIdle = stof(utilization[0]) + stof(utilization[1]) + stof(utilization[2]) + stof(utilization[5]) + stof(utilization[6]) + stof(utilization[7]);

    float PrevTotal = PrevIdle + PrevNonIdle;
    float Total = Idle + NonIdle;

    float totald = Total - PrevTotal;
    float idled = Idle - PrevIdle;
    if (totald==idled){utilization_value= 0.0;}
    else{ utilization_value = (totald - idled)/totald;  }

    return utilization_value;

}

// DONe
float Processor::Utilization() { 
    vector<string> utilization = LinuxParser::CpuUtilization();
    bool compute_current_utilization = 1;  // 1 = compute utilization every 1 sec.  AND 0 = long term utilization from start of time.
    float prev_utilization_update_freq = 1; // update prev utilization every 1 second for computing current utilization
    float curr_utilization_value = 1000;
    switch(compute_current_utilization){
        case 0: curr_utilization_value = LongTermUtilization(utilization); break;
        case 1: 
            // updates utilization every 1 seconds (sleep_time)
            if (prev_utilization.empty()){
                prev_utilization = utilization; 
                curr_utilization_value = LongTermUtilization(utilization);;
                time_when_prev_utilization_updated = clock();
	
                }
            else{
                float time_passed_in_secs = ((float)(clock() - time_when_prev_utilization_updated))/CLOCKS_PER_SEC;
                curr_utilization_value = ShortTermUtilization(utilization,prev_utilization);
	            if(time_passed_in_secs>prev_utilization_update_freq){

                    prev_utilization = utilization; 
                    time_when_prev_utilization_updated = clock();
                }
            }
            break;
    }

    return curr_utilization_value;

}
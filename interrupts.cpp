/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 */

#include<interrupts.hpp>
#include<string.h>
#include<stdio.h>

char* numToString(int num);
    
int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/
    int time = 0;
    int savetime = 10;
    float coeff = 1;
    int cpuCycle = 0;
    bool isReader = true; //true is reader, false is writer
    bool firstTime = true;
    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/
    
        
        //-------------------------------------------------
        if(activity == "SYSCALL"){
            duration_intr --;
            auto results = 
intr_boilerplate(time, duration_intr, savetime, vectors);
            
            execution += results.first;
            time = results.second;
           
            if(isReader){
                execution += std::to_string(time) + ", " +std::to_string((int)std::ceil((delays.at(duration_intr))/2))+ ", Get information from reader\n";
            }
            else if(!isReader){
                execution += std::to_string(time) + ", " +std::to_string((int)std::ceil((delays.at(duration_intr))/2))+ ", Give information to printer\n";
            }
            isReader = !isReader;
            time += std::ceil((delays.at(duration_intr))/2);

            execution += std::to_string(time) + ", " +std::to_string((int)std::floor((delays.at(duration_intr))/2))+ ", Call device driver to perform task\n";
            time += std::floor((delays.at(duration_intr))/2);

            execution += std::to_string(time) + ", " +std::to_string(1)+ ", IRET\n";
            time ++;

        }
       
        if(activity == "END_IO"){
            duration_intr--;
            auto results = 
intr_boilerplate(time, duration_intr, savetime, vectors);
            
            execution += results.first;
            time = results.second;

            execution += std::to_string(time) + ", " +std::to_string((int)std::round(delays.at(duration_intr)))+ ", Save information to memory\n";
            time += delays.at(duration_intr);

            execution += std::to_string(time) + ", " +std::to_string(1)+ ", IRET\n";
            time ++;
        }
        //------------------------
        if(activity == "CPU"){
            
            execution += std::to_string( time) +", "+ std::to_string((int)std::round(duration_intr*coeff)) + ", CPU BURST\n";
            time += duration_intr*coeff;
        }

        /************************************************************************/

    }

    input_file.close();

    write_output(execution);
    //./interrupts ../trace.txt ../vector_table.tx ../device_table.txt
    return 0;
}


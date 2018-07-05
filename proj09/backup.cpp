/*
Dillon Stock stockdil@basset
2017-04-03
proj08.student.c

Your Comments
*/

#include <iostream>
#include <iomanip>
#include <string>
#include <string.h>
#include <fstream>
#include <vector>
#include <algorithm>
using std::cout; using std::endl;
using namespace std;


struct process{
public:
    unsigned pid;
    unsigned priority;
    unsigned burstNum;
    unsigned burstTime;
    unsigned blockedTime;
    unsigned arrivalTime = 0;
    
    unsigned currentState = 0;
    unsigned cumulativeTimes[4] = {};
    unsigned remainingBursts = burstNum; // the number of bursts left before process halts
    unsigned remainingDuration = 0; // amount of time before process is blocked/halted
    unsigned turnaround = 0;
    unsigned entered_ready = 0;
    //double normalizedTurnaround = 0;
    //bool haltRequest = false;
    //bool blockRequest = false;
    
    vector<unsigned> ticksNew;
    vector<unsigned> ticksReady;
    vector<unsigned> ticksRunning;
    vector<unsigned> ticksBlocked;
    
    /*
    bool operator<(const process& other) const
    {
        return (this->arrivalTime < other.arrivalTime);
    }
    */
};


// To display all appropriate process statistics before releasing it from memory
void exitDisplay(const process& p1, unsigned clock){
    cout << "PID: " << p1.pid << endl;
    cout << "Priority: " << p1.priority << endl;
    cout << "Number of CPU bursts: " << p1.burstNum << endl;
    cout << "Burst Time (in ticks): " << p1.burstTime << endl;
    cout << "Blocked Time (in ticks): " << p1.blockedTime << endl;
    cout << "Arrival Time (in ticks since start of simulation): " << p1.arrivalTime << endl;
    cout << "Departure time: " << clock << endl;
    cout << "Cumulative time in the New state: " << p1.cumulativeTimes[0] << endl;
    cout << "Cumulative time in the Ready state: "<< p1.cumulativeTimes[1] << endl;
    cout << "Cumulative time in the Running state: "<< p1.cumulativeTimes[2] << endl;    
    cout << "Cumulative time in the Blocked state: "<< p1.cumulativeTimes[3] << endl;
    cout << "Turnaround time: " << p1.turnaround << endl;
    //cout << "Normalized turnaround time: " << p1.normalizedTurnaround << endl;
    //cout << "Normalized Turnaround Time: " << (p1.turnaround/p1.cumulativeTimes[2]) << endl;
    double normalizedTurnaround = (p1.turnaround/double(p1.cumulativeTimes[2]));
    cout << "Normalized Turnaround Time: " << setprecision(3) << normalizedTurnaround << endl;
    
    ////////////////////////////////// Comment out ///////////////////////////////////////////////////////////////////
    /*
    cout << "Ticks spent in New state: "; for(auto& tick : p1.ticksNew) cout << tick << " "; cout << endl;
    cout << "Ticks spent in Ready state: "; for(auto& tick : p1.ticksReady) cout << tick << " "; cout << endl;
    cout << "Ticks spent in Running state: "; for(auto& tick : p1.ticksRunning) cout << tick << " "; cout << endl;
    cout << "Ticks spent in Blocked state: "; for(auto& tick : p1.ticksBlocked) cout << tick << " "; cout << endl;
    */
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    cout << endl;

}




// First compares arrival time
// breaks ties using process ID
bool arrival_compare(const process& p1, const process& p2){
    if(p1.arrivalTime != p2.arrivalTime){
        return p1.arrivalTime < p2.arrivalTime;
    }
    return p1.pid < p2.pid;
}

// First compares priority,
// To break priority ties, we give preference to the process which has been ready the longest
// Finally we break ties using process ID
bool priority_compare(const process& p1, const process& p2){
    if(p1.priority != p2.priority){
        return (p1.priority < p2.priority);
    }
    if(p1.entered_ready != p2.entered_ready){
        return (p1.entered_ready < p2.entered_ready);
    }
    return p1.pid < p2.pid;
}

bool duration_compare(const process& p1, const process& p2){
    return p1.remainingDuration < p2.remainingDuration;
}

// To display all process info for each process in a vector of processes
void display_process_info(vector<struct process>& processes){
    for(const auto& p1 : processes){
        cout << p1.pid << " " << p1.priority << " " << p1.burstNum << " " << p1.burstTime << " " << p1.blockedTime << " " << p1.arrivalTime << endl;
    }
}



int main(int argc, char *argv[], char** variables)
{
    //cout << "argc: " << argc << endl; // *** remove ***
    
    if(argc != 3){
        cout << "Invalid number of arguments... (expected 3) Ending program." << endl;
        return -1;
    }
    
    ifstream infile(argv[2]);
    if(!infile){
        cout << "Invalid file name received... Exiting program" << endl;
        return -1;
    }
    
    string N_error_test = argv[1];
    for(char c : N_error_test){
        if( c - '0' < 0 || c - '0' > 9){
            cout << "Invalid argument for N (expected a positive integer or 0)... quitting program" << endl;
            return 0;
        }
    }
    
    int N = stoi(string(argv[1]), nullptr, 10);
    //cout << "N: " << N << endl; // *** remove
    
    string cpuStr;
    string pcbStr;
    string tickStr;
    unsigned cpuNum;
    unsigned pcbNum;
    unsigned tickNum;
    string alg;
    
    getline(infile,cpuStr);
    getline(infile, pcbStr);
    getline(infile, tickStr);
    getline(infile, alg);
    cout << "Number of CPUs: " << cpuStr << endl;
    cout << "Number of PCBs: " << pcbStr << endl;
    cout << "Length of simulation (in ticks): " << tickStr << endl;
    cout << "Short-term scheduling algorithm: " << alg << endl << endl;
    
    std::transform(alg.begin(), alg.end(), alg.begin(), ::toupper);
    cpuNum = stoi(cpuStr, nullptr, 10);
    pcbNum = stoi(pcbStr, nullptr, 10);
    tickNum = stoi(tickStr, nullptr, 10);
            
    //vector<struct process> all;
    vector<struct process> batch;
    vector<struct process> newProcesses;
    vector<struct process> ready;
    vector<struct process> blocked;
    vector<struct process> running;
    
    struct process p1;
    while(infile >> p1.pid){
        infile >> p1.priority >> p1.burstNum >> p1.burstTime >> p1.blockedTime >> p1.arrivalTime;
        p1.remainingBursts = p1.burstNum;            // *************************8
        batch.push_back(p1);
    }
    
    if(alg != "FCFS" && alg != "PRIORITY"){
        cout << "Invalid placement algorithm selected (expected 'FCFS' or 'Priority')... Exiting program" << endl;
        return -1;
    }
    
    
    std::sort(batch.begin(), batch.end(), arrival_compare);
    
    /* This should not be included for sorting the original batch, as the algorithm
     * is 'non-preemptive' and does not know of a given process until it arrives
    else if(alg == "Priority"){
        //cout << "Sorting using Priority... " << endl;
        std::sort(batch.begin(), batch.end(), priority_compare);
    }
    */
    
    
    //for(unsigned i=1; i<=tickNum; i++){
    unsigned clock = 0;
    while(clock < tickNum){
        //cout << "i: " << i << " -----------------------------" << endl << endl;
        if(!running.empty()){
            //cout << "sorting running processes by remaining duration... " << endl;
            std::sort(running.begin(), running.end(), duration_compare);
            
            //cout << "processing request issued by the process (pid: " << running[0].pid << ") in Running state" << endl;
            //cout << "remaining duration of running[0] : " << running[0].remainingDuration << endl;    
            //cout << "remaining (cpu) bursts of running[0] : " << running[0].remainingBursts << endl;        
            if(running[0].remainingDuration == 0){
                if(running[0].remainingBursts > 0){
                    //cout << "Running process requested BLOCK." << endl;
                    running[0].currentState = 3;
                    //cout << "bursts left : " << running[0].remainingBursts << endl;
                    running[0].remainingDuration = running[0].blockedTime;
                    blocked.push_back(running[0]);
                    running.erase(running.begin());
                    cpuNum++;
                }
            
                else if(running[0].remainingBursts == 0){
                    //cout << "Running process requested HALT." << endl;
                    running[0].turnaround = clock - running[0].arrivalTime;
                    exitDisplay(running[0], clock);
                    //cout << "clock: " << clock << endl;
                    //cout << "running[0].arrivalTime: " << running[0].arrivalTime << endl;
                    //cout << "running[0].turnaround: " << running[0].turnaround << endl;
                    running.erase(running.begin());
                    pcbNum++;
                    cpuNum++;
                }
            }
        }
        
        if(!blocked.empty()){
            //cout << "sorting blocked processes by remaining duration... " << endl;
            std::sort(blocked.begin(), blocked.end(), duration_compare);
            //cout << "checking if process in blocked state should become unblocked..." << endl;
            //cout  << "remaining duration (to stay blocked) : " << blocked[0].remainingDuration << endl;
            
            //if(blocked[0].remainingDuration == 0){
            while(!blocked.empty() && blocked[0].remainingDuration == 0){
                //cout << "unblocking process..." << endl;
                blocked[0].currentState = 1;
                blocked[0].entered_ready = clock;
                ready.push_back(blocked[0]);
                blocked.erase(blocked.begin());
                
            }
        }
        
        if(!batch.empty()){
            //cout << "found a process in the batch, must move to NEW processes" << endl;
            //if(clock == batch[0].arrivalTime){
            while(clock == batch[0].arrivalTime && !batch.empty()){
                //cout << "process (pid: " << batch[0].pid << ") in batch has arrived... adding it to NEW processes (clock: " << clock << ")" << endl;
                batch[0].currentState = 0;
                //batch[0].cumulativeTimes[batch[0].currentState]++;
                newProcesses.push_back(batch[0]);
                batch.erase(batch.begin());
            }
        }
        
        if(pcbNum > 0){
            //if(!newProcesses.empty()){
            while(!newProcesses.empty() && pcbNum > 0){
                //cout << "allocating PCB to new process (pid: " << newProcesses[0].pid << ")" << endl;
                newProcesses[0].currentState = 0;
                ready.push_back(newProcesses[0]);
                newProcesses.erase(newProcesses.begin());
                pcbNum--;
            }
        }
        
        if(!ready.empty()){
            //cout << "ready process found." << endl;
            /*
            if(alg == "FCFS"){
                sort(ready.begin(), ready.end(), arrival_compare);
            }
            */
            
            if(alg == "PRIORITY"){
                sort(ready.begin(), ready.end(), priority_compare);
            }
            
            //if(running.empty() || running.size() < cpuNum){
            while(running.empty() || running.size() < cpuNum){
                //cout << "moving process from ready ---> to running" << endl;
                ready[0].remainingDuration = ready[0].burstTime;
                //cout << "ready[0].remainingBursts : " << ready[0].remainingBursts << endl;
                ready[0].remainingBursts--;
                //cout << "moved process to 'running' with " << ready[0].remainingBursts << " burst left... " << endl; //******
                ready[0].currentState = 1;
                running.push_back(ready[0]);
                ready.erase(ready.begin());
                cpuNum--;
            }
        }
        
        
        for(auto& p : newProcesses){
            p.cumulativeTimes[0]++;
            //cout << "time in new state: " << p.cumulativeTimes[0] << endl;
            p.ticksNew.push_back(clock);
        }
        
        for(auto& p : ready){
            p.cumulativeTimes[1]++;
            //cout << "time in ready state: " << p.cumulativeTimes[1] << endl;
            p.ticksReady.push_back(clock);
        }
        
        for(auto& p : running){
            p.cumulativeTimes[2]++;
            //cout << "time in running state: " << p.cumulativeTimes[2] << endl;
            p.remainingDuration--;
            //cout << "remaining duration (pid: " << p.pid << ") in running state: " << p.remainingDuration << endl;
            p.ticksRunning.push_back(clock);
        }

        for(auto& p : blocked){
            p.cumulativeTimes[3]++;
            //cout << "time in blocked state: " << p.cumulativeTimes[3] << endl;
            p.remainingDuration--;
            //cout << "remaining duration (pid: " << p.pid << ") in blocked state: " << p.remainingDuration << endl;
            p.ticksBlocked.push_back(clock);
        }
        
        
        
        clock++;
        
        
        if(N > 0 && clock % N == 0 && clock != 0){
            cout << "Displaying Process Statuses... (Tick: " << clock << ")" << endl;
            
            cout << "New process IDs: ";
            for(auto& p : newProcesses){
                cout << p.pid << ", ";
            }
            cout << endl;
            
            cout << "Ready process IDs: ";
            for(auto& p : ready){
                cout << p.pid << ", ";
            }
            cout << endl;
            
            cout << "Running process IDs: ";
            for(auto& p : running){
                cout << p.pid << ", ";
            }
            cout << endl;

            cout << "Blocked process IDs: ";
            for(auto& p : blocked){
                cout << p.pid << ", ";
            }
            cout  << endl;
            
            cout << endl;
        }
    }
    
    
    
    
    
    /*
     * To Complete: 
     * Read Requirement Number 5 on the Project 9 description...
     * 'Non-Preemptive FCFS scheduling' versus 'Non-Preemptive Priority scheduling'
     */





    
    /*
    cout << "pid: " << pid << endl;
    cout << "priority: " << priority << endl;
    cout << "burstNum: " << burstNum << endl;
    cout << "burstTime: " << burstTime << endl;
    cout << "blockedTime: " << blockedTime << endl;
    cout << "arrivalTime: " << arrivalTime << endl;
    */
    
    /*
    cout << endl;
    cout << "Displaying parameters... (end of program)" << endl; // ***
    
    
    cout << "pid: " << p1.pid << endl;
    cout << "priority: " << p1.priority << endl;
    cout << "burstNum: " << p1.burstNum << endl;
    cout << "burstTime: " << p1.burstTime << endl;
    cout << "blockedTime: " << p1.blockedTime << endl;
    cout << "arrivalTime: " << p1.arrivalTime << endl;
    cout << "Departure time: " << clock << endl;
    cout << "Cumulative time in the New state: " << p1.cumulativeTimes[0] << endl;
    cout << "Cumulative time in the Ready state: "<< p1.cumulativeTimes[1] << endl;
    cout << "Cumulative time in the Running state: "<< p1.cumulativeTimes[2] << endl;    
    cout << "Cumulative time in the Blocked state: "<< p1.cumulativeTimes[3] << endl;
    */
    
}

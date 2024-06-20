#include <bits/stdc++.h>

using namespace std;

/** Global Constants **/
const string TRACE = "trace";
const string SHOW_STATISTICS = "stats";
const string ALGORITHMS[9] = {"", "FCFS", "SPN", "SRT", "HRRN", "FB-1", "FB-2i"};

bool sortByServiceTime(const tuple<string, int, int> &a, const tuple<string, int, int> &b)
{
    return (get<2>(a) < get<2>(b));
}
bool sortByArrivalTime(const tuple<string, int, int> &a, const tuple<string, int, int> &b)
{
    return (get<1>(a) < get<1>(b));
}

bool descendingly_by_response_ratio(tuple<string, double, int> a, tuple<string, double, int> b)
{
    return get<1>(a) > get<1>(b);
}

bool byPriorityLevel(const tuple<int,int,int>&a,const tuple<int,int,int>&b){
    if(get<0>(a)==get<0>(b))
        return get<2>(a)> get<2>(b);
    return get<0>(a) > get<0>(b);
}
vector<tuple<string, int, int>> processes;
int process_count;
int last_instant;
vector<vector<char>> timeline;
vector<int> finishTime, turnAroundTime;
vector<double> normTurn;
unordered_map<string, int> processToIndex;
string operation;


void clear_timeline()
{
    for(int i=0; i<last_instant; i++)
        for(int j=0; j<process_count; j++)
            timeline[i][j] = ' ';
}

string getProcessName(tuple<string, int, int> &a)
{
    return get<0>(a);
}

int getArrivalTime(tuple<string, int, int> &a)
{
    return get<1>(a);
}

int getServiceTime(tuple<string, int, int> &a)
{
    return get<2>(a);
}

int getPriorityLevel(tuple<string, int, int> &a)
{
    return get<2>(a);
}

double calculate_response_ratio(int wait_time, int service_time)
{
    return (wait_time + service_time) * 1.0 / service_time;
}

void fillInWaitTime()
{
    for (int i = 0; i < process_count; i++)
    {
        int arrivalTime = getArrivalTime(processes[i]);
        for (int k = arrivalTime; k < finishTime[i]; k++)
        {
            if (timeline[k][i] != '*')
                timeline[k][i] = '.';
        }
    }
}


void firstComeFirstServe()
{
    // Sort processes by arrival time
    sort(processes.begin(), processes.end(), sortByArrivalTime);

    int time = getArrivalTime(processes[0]);
    for (int i = 0; i < process_count; i++)
    {
        int processIndex = i;
        int arrivalTime = getArrivalTime(processes[i]);
        int serviceTime = getServiceTime(processes[i]);

        finishTime[processIndex] = (time + serviceTime);
        turnAroundTime[processIndex] = (finishTime[processIndex] - arrivalTime);
        normTurn[processIndex] = (turnAroundTime[processIndex] * 1.0 / serviceTime);

        // Mark timeline with '*' for the duration of the process execution
        for (int j = time; j < finishTime[processIndex]; j++)
            timeline[j][processIndex] = '*';

        // Mark idle time with '.' before the process arrival
        for (int j = arrivalTime; j < time; j++)
            timeline[j][processIndex] = '.';

        time += serviceTime; // Move time forward after scheduling the process
    }
}

void shortestProcessNext()
{
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; // pair of service time and index
    int j = 0;
    for (int i = 0; i < last_instant; i++)
    {
        while(j<process_count && getArrivalTime(processes[j]) <= i){
            pq.push(make_pair(getServiceTime(processes[j]), j));
            j++;
        }
        if (!pq.empty())
        {
            int processIndex = pq.top().second;
            int arrivalTime = getArrivalTime(processes[processIndex]);
            int serviceTime = getServiceTime(processes[processIndex]);
            pq.pop();

            int temp = arrivalTime;
            for (; temp < i; temp++)
                timeline[temp][processIndex] = '.';

            temp = i;
            for (; temp < i + serviceTime; temp++)
                timeline[temp][processIndex] = '*';

            finishTime[processIndex] = (i + serviceTime);
            turnAroundTime[processIndex] = (finishTime[processIndex] - arrivalTime);
            normTurn[processIndex] = (turnAroundTime[processIndex] * 1.0 / serviceTime);
            i = temp - 1;
        }
    }
}
void shortestRemainingTime()
{
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    int j = 0;
    for (int i = 0; i < last_instant; i++)
    {
        while(j<process_count &&getArrivalTime(processes[j]) == i){
            pq.push(make_pair(getServiceTime(processes[j]), j));
            j++;
        }
        if (!pq.empty())
        {
            int processIndex = pq.top().second;
            int remainingTime = pq.top().first;
            pq.pop();
            int serviceTime = getServiceTime(processes[processIndex]);
            int arrivalTime = getArrivalTime(processes[processIndex]);
            timeline[i][processIndex] = '*';

            if (remainingTime == 1) // process finished
            {
                finishTime[processIndex] = i + 1;
                turnAroundTime[processIndex] = (finishTime[processIndex] - arrivalTime);
                normTurn[processIndex] = (turnAroundTime[processIndex] * 1.0 / serviceTime);
            }
            else
            {
                pq.push(make_pair(remainingTime - 1, processIndex));
            }
        }
    }
    fillInWaitTime();
}


void highestResponseRatioNext()
{

    // Vector of tuple <process_name, process_response_ratio, time_in_service> for processes that are in the ready queue
    vector<tuple<string, double, int>> present_processes;
    int j=0;
    for (int current_instant = 0; current_instant < last_instant; current_instant++)
    {
        while(j<process_count && getArrivalTime(processes[j])<=current_instant){
            present_processes.push_back(make_tuple(getProcessName(processes[j]), 1.0, 0));
            j++;
        }
        // Calculate response ratio for every process
        for (auto &proc : present_processes)
        {
            string process_name = get<0>(proc);
            int process_index = processToIndex[process_name];
            int wait_time = current_instant - getArrivalTime(processes[process_index]);
            int service_time = getServiceTime(processes[process_index]);
            get<1>(proc) = calculate_response_ratio(wait_time, service_time);
        }

        // Sort present processes by highest to lowest response ratio
        sort(present_processes.begin(), present_processes.end(), descendingly_by_response_ratio);


        if (!present_processes.empty())
        {
            int process_index = processToIndex[get<0>(present_processes[0])];
            while(current_instant<last_instant && get<2>(present_processes[0]) != getServiceTime(processes[process_index])){
                timeline[current_instant][process_index]='*';
                current_instant++;
                get<2>(present_processes[0])++;
            }
            current_instant--;
            present_processes.erase(present_processes.begin());
            finishTime[process_index] = current_instant + 1;
            turnAroundTime[process_index] = finishTime[process_index] - getArrivalTime(processes[process_index]);
            normTurn[process_index] = (turnAroundTime[process_index] * 1.0 / getServiceTime(processes[process_index]));
        }
    }
    fillInWaitTime();
}
void feedbackQ1()
{
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; //pair of priority level and process index
    unordered_map<int,int>remainingServiceTime; //map from process index to the remaining service time
    int j=0;
    if(getArrivalTime(processes[0])==0){
        pq.push(make_pair(0,j));
        remainingServiceTime[j]=getServiceTime(processes[j]);
        j++;
    }
    for(int time =0;time<last_instant;time++){
        if(!pq.empty()){
            int priorityLevel = pq.top().first;
            int processIndex =pq.top().second;
            int arrivalTime = getArrivalTime(processes[processIndex]);
            int serviceTime = getServiceTime(processes[processIndex]);
            pq.pop();
            while(j<process_count && getArrivalTime(processes[j])==time+1){
                    pq.push(make_pair(0,j));
                    remainingServiceTime[j]=getServiceTime(processes[j]);
                    j++;
            }
            remainingServiceTime[processIndex]--;
            timeline[time][processIndex]='*';
            if(remainingServiceTime[processIndex]==0){
                finishTime[processIndex]=time+1;
                turnAroundTime[processIndex] = (finishTime[processIndex] - arrivalTime);
                normTurn[processIndex] = (turnAroundTime[processIndex] * 1.0 / serviceTime);
            }else{
                if(pq.size()>=1)
                    pq.push(make_pair(priorityLevel+1,processIndex));
                else
                    pq.push(make_pair(priorityLevel,processIndex));
            }
        }
        while(j<process_count && getArrivalTime(processes[j])==time+1){
                pq.push(make_pair(0,j));
                remainingServiceTime[j]=getServiceTime(processes[j]);
                j++;
        }
    }
    fillInWaitTime();
}

void feedbackQ2i()
{
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; //pair of priority level and process index
    unordered_map<int,int>remainingServiceTime; //map from process index to the remaining service time
    int j=0;
    if(getArrivalTime(processes[0])==0){
        pq.push(make_pair(0,j));
        remainingServiceTime[j]=getServiceTime(processes[j]);
        j++;
    }
    for(int time =0;time<last_instant;time++){
        if(!pq.empty()){
            int priorityLevel = pq.top().first;
            int processIndex =pq.top().second;
            int arrivalTime = getArrivalTime(processes[processIndex]);
            int serviceTime = getServiceTime(processes[processIndex]);
            pq.pop();
            while(j<process_count && getArrivalTime(processes[j])<=time+1){
                    pq.push(make_pair(0,j));
                    remainingServiceTime[j]=getServiceTime(processes[j]);
                    j++;
            }

            int currentQuantum = pow(2,priorityLevel);
            int temp = time;
            while(currentQuantum && remainingServiceTime[processIndex]){
                currentQuantum--;
                remainingServiceTime[processIndex]--;
                timeline[temp++][processIndex]='*';
            }

            if(remainingServiceTime[processIndex]==0){
                finishTime[processIndex]=temp;
                turnAroundTime[processIndex] = (finishTime[processIndex] - arrivalTime);
                normTurn[processIndex] = (turnAroundTime[processIndex] * 1.0 / serviceTime);
            }else{
                if(pq.size()>=1)
                    pq.push(make_pair(priorityLevel+1,processIndex));
                else
                    pq.push(make_pair(priorityLevel,processIndex));
            }
            time = temp-1;
        }
        while(j<process_count && getArrivalTime(processes[j])<=time+1){
                pq.push(make_pair(0,j));
                remainingServiceTime[j]=getServiceTime(processes[j]);
                j++;
        }
    }
    fillInWaitTime();
}


void parse()
{
    cout << "Enter the number of processes: ";
    cin >> process_count;
    processes.resize(process_count);

    for (int i = 0; i < process_count; ++i) {
        string name;
        int arrival, burst;
        cout << "Enter process name, arrival time, and burst time for process " << i + 1 << ": ";
        cin >> name >> arrival >> burst;
        processes[i] = make_tuple(name, arrival, burst);
        processToIndex[name] = i;
    }

    cout << "Enter the last instant of simulation: ";
    cin >> last_instant;

    finishTime.resize(process_count);
    turnAroundTime.resize(process_count);
    normTurn.resize(process_count);
    timeline.resize(last_instant, vector<char>(process_count, ' '));

    cout << "Enter the operation (trace/stats): ";
    cin >> operation;

    string algorithm;
    cout << "Enter the scheduling algorithm (FCFS, SPN, SRT, HRRN, FB-1, FB-2i): ";
    cin >> algorithm;

    int quantum = 0;
    if (algorithm.find("RR") != string::npos) {
        cout << "Enter the quantum: ";
        cin >> quantum;
    }

    if (algorithm == "FCFS") {
        firstComeFirstServe();
    } else if (algorithm == "SPN") {
        shortestProcessNext();
    } else if (algorithm == "SRT") {
        shortestRemainingTime();
    } else if (algorithm == "HRRN") {
        highestResponseRatioNext();
    } else if (algorithm == "FB-1") {
        feedbackQ1();
    } else if (algorithm == "FB-2i") {
        feedbackQ2i();
    } else {
        cerr << "Unknown algorithm!" << endl;
        exit(1);
    }

    if (operation == TRACE) {
        // Output the timeline
        for (int i = 0; i < process_count; ++i) {
            cout << get<0>(processes[i]) << ": ";
            for (int j = 0; j < last_instant; ++j) {
                cout << timeline[j][i];
            }
            cout << endl;
        }
    } else if (operation == SHOW_STATISTICS) {
        // Output the statistics
        cout << "Process\tArrival\tBurst\tFinish\tTurnaround\tNormalized Turnaround\n";
        for (int i = 0; i < process_count; ++i) {
            cout << get<0>(processes[i]) << "\t"
                 << get<1>(processes[i]) << "\t"
                 << get<2>(processes[i]) << "\t"
                 << finishTime[i] << "\t"
                 << turnAroundTime[i] << "\t\t"
                 << normTurn[i] << "\n";
        }
    }
}

int main()
{
    parse();
    return 0;
}

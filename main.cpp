#include<bits/stdc++.h>
#include <vector>
#include <algorithm>
#include <queue>
#include <iomanip>
#include <set>

using namespace std;

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int completionTime;
    int waitingTime;
    int turnaroundTime;
    int priority;
};

// Function to calculate FCFS scheduling
float FCFS(vector<Process>& processes) {
    int n = processes.size();
    int currentTime = 0;
    int totalWaitingTime = 0, totalTurnaroundTime = 0;

    for (int i = 0; i < n; i++) {
        if (currentTime < processes[i].arrivalTime) {
            currentTime = processes[i].arrivalTime;
        }
        processes[i].waitingTime = currentTime - processes[i].arrivalTime;
        currentTime += processes[i].burstTime;
        processes[i].completionTime = currentTime;
        processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;

        totalWaitingTime += processes[i].waitingTime;
        totalTurnaroundTime += processes[i].turnaroundTime;
    }

    float avgWaitingTime = (float)totalWaitingTime / n;
    cout << "FCFS Scheduling:" << endl;
    cout << "Average Waiting Time: " << avgWaitingTime << endl;
    cout << "Average Turnaround Time: " << (float)totalTurnaroundTime / n << endl;
    return avgWaitingTime;
}

// Function to calculate SJF (Non-Preemptive) scheduling
float SJFNonPreemptive(vector<Process>& processes) {
    int n = processes.size();
    int currentTime = 0, completed = 0;
    int totalWaitingTime = 0, totalTurnaroundTime = 0;
    vector<bool> isCompleted(n, false);

    while (completed != n) {
        int minBurstTime = INT_MAX, index = -1;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && !isCompleted[i] && processes[i].burstTime < minBurstTime) {
                minBurstTime = processes[i].burstTime;
                index = i;
            }
        }

        if (index != -1) {
            currentTime += processes[index].burstTime;
            processes[index].completionTime = currentTime;
            processes[index].turnaroundTime = processes[index].completionTime - processes[index].arrivalTime;
            processes[index].waitingTime = processes[index].turnaroundTime - processes[index].burstTime;
            isCompleted[index] = true;
            completed++;
            totalWaitingTime += processes[index].waitingTime;
            totalTurnaroundTime += processes[index].turnaroundTime;
        } else {
            currentTime++;
        }
    }

    float avgWaitingTime = (float)totalWaitingTime / n;
    cout << "SJF (Non-Preemptive) Scheduling:" << endl;
    cout << "Average Waiting Time: " << avgWaitingTime << endl;
    cout << "Average Turnaround Time: " << (float)totalTurnaroundTime / n << endl;
    return avgWaitingTime;
}

// Function to calculate SJF (Preemptive) scheduling
float SJFPreemptive(vector<Process>& processes) {
    int n = processes.size();
    int currentTime = 0, completed = 0;
    int totalWaitingTime = 0, totalTurnaroundTime = 0;
    vector<bool> isCompleted(n, false);

    auto cmp = [](Process* a, Process* b) {
        return a->remainingTime > b->remainingTime;
    };
    priority_queue<Process*, vector<Process*>, decltype(cmp)> pq(cmp);

    while (completed != n) {
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && !isCompleted[i]) {
                pq.push(&processes[i]);
                isCompleted[i] = true;
            }
        }

        if (pq.empty()) {
            currentTime++;
            continue;
        }

        Process* currentProcess = pq.top();
        pq.pop();
        currentProcess->remainingTime--;

        if (currentProcess->remainingTime == 0) {
            currentProcess->completionTime = currentTime + 1;
            currentProcess->turnaroundTime = currentProcess->completionTime - currentProcess->arrivalTime;
            currentProcess->waitingTime = currentProcess->turnaroundTime - currentProcess->burstTime;
            totalWaitingTime += currentProcess->waitingTime;
            totalTurnaroundTime += currentProcess->turnaroundTime;
            completed++;
        } else {
            pq.push(currentProcess);
        }
        currentTime++;
    }

    float avgWaitingTime = (float)totalWaitingTime / n;
    cout << "SJF (Preemptive) Scheduling:" << endl;
    cout << "Average Waiting Time: " << avgWaitingTime << endl;
    cout << "Average Turnaround Time: " << (float)totalTurnaroundTime / n << endl;
    return avgWaitingTime;
}

// Function to calculate Round Robin scheduling
float RoundRobin(vector<Process>& processes, int timeQuantum) {
    int n = processes.size();
    int currentTime = 0, completed = 0;
    int totalWaitingTime = 0, totalTurnaroundTime = 0;
    queue<Process*> q;
    set<int> inQueue; // To keep track of processes in the queue

    for (int i = 0; i < n; i++) {
        if (processes[i].arrivalTime == 0) {
            q.push(&processes[i]);
            inQueue.insert(processes[i].id);
        }
    }

    while (completed != n) {
        if (q.empty()) {
            currentTime++;
            for (int i = 0; i < n; i++) {
                if (processes[i].arrivalTime == currentTime && inQueue.find(processes[i].id) == inQueue.end()) {
                    q.push(&processes[i]);
                    inQueue.insert(processes[i].id);
                }
            }
            continue;
        }

        Process* currentProcess = q.front();
        q.pop();
        inQueue.erase(currentProcess->id);

        if (currentProcess->remainingTime > timeQuantum) {
            currentProcess->remainingTime -= timeQuantum;
            currentTime += timeQuantum;
        } else {
            currentTime += currentProcess->remainingTime;
            currentProcess->remainingTime = 0;
            currentProcess->completionTime = currentTime;
            currentProcess->turnaroundTime = currentProcess->completionTime - currentProcess->arrivalTime;
            currentProcess->waitingTime = currentProcess->turnaroundTime - currentProcess->burstTime;
            totalWaitingTime += currentProcess->waitingTime;
            totalTurnaroundTime += currentProcess->turnaroundTime;
            completed++;
        }

        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0 && inQueue.find(processes[i].id) == inQueue.end()) {
                q.push(&processes[i]);
                inQueue.insert(processes[i].id);
            }
        }

        if (currentProcess->remainingTime > 0) {
            q.push(currentProcess);
            inQueue.insert(currentProcess->id);
        }
    }

    float avgWaitingTime = (float)totalWaitingTime / n;
    cout << "Round Robin Scheduling:" << endl;
    cout << "Average Waiting Time: " << avgWaitingTime << endl;
    cout << "Average Turnaround Time: " << (float)totalTurnaroundTime / n << endl;
    return avgWaitingTime;
}

// Function to calculate Priority Scheduling (Non-Preemptive)
float PriorityNonPreemptive(vector<Process>& processes) {
    int n = processes.size();
    int currentTime = 0, completed = 0;
    int totalWaitingTime = 0, totalTurnaroundTime = 0;
    vector<bool> isCompleted(n, false);

    while (completed != n) {
        int highestPriority = INT_MAX, index = -1;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && !isCompleted[i] && processes[i].priority < highestPriority) {
                highestPriority = processes[i].priority;
                index = i;
            }
        }

        if (index != -1) {
            currentTime += processes[index].burstTime;
            processes[index].completionTime = currentTime;
            processes[index].turnaroundTime = processes[index].completionTime - processes[index].arrivalTime;
            processes[index].waitingTime = processes[index].turnaroundTime - processes[index].burstTime;
            isCompleted[index] = true;
            completed++;
            totalWaitingTime += processes[index].waitingTime;
            totalTurnaroundTime += processes[index].turnaroundTime;
        } else {
            currentTime++;
        }
    }

    float avgWaitingTime = (float)totalWaitingTime / n;
    cout << "Priority (Non-Preemptive) Scheduling:" << endl;
    cout << "Average Waiting Time: " << avgWaitingTime << endl;
    cout << "Average Turnaround Time: " << (float)totalTurnaroundTime / n << endl;
    return avgWaitingTime;
}

// Function to calculate Priority Scheduling (Preemptive)
float PriorityPreemptive(vector<Process>& processes) {
    int n = processes.size();
    int currentTime = 0, completed = 0;
    int totalWaitingTime = 0, totalTurnaroundTime = 0;
    vector<bool> isCompleted(n, false);

    auto cmp = [](Process* a, Process* b) {
        if (a->priority == b->priority) return a->arrivalTime > b->arrivalTime;
        return a->priority > b->priority;
    };
    priority_queue<Process*, vector<Process*>, decltype(cmp)> pq(cmp);

    while (completed != n) {
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && !isCompleted[i]) {
                pq.push(&processes[i]);
                isCompleted[i] = true;
            }
        }

        if (pq.empty()) {
            currentTime++;
            continue;
        }

        Process* currentProcess = pq.top();
        pq.pop();
        currentProcess->remainingTime--;

        if (currentProcess->remainingTime == 0) {
            currentProcess->completionTime = currentTime + 1;
            currentProcess->turnaroundTime = currentProcess->completionTime - currentProcess->arrivalTime;
            currentProcess->waitingTime = currentProcess->turnaroundTime - currentProcess->burstTime;
            totalWaitingTime += currentProcess->waitingTime;
            totalTurnaroundTime += currentProcess->turnaroundTime;
            completed++;
        } else {
            pq.push(currentProcess);
        }
        currentTime++;
    }

    float avgWaitingTime = (float)totalWaitingTime / n;
    cout << "Priority (Preemptive) Scheduling:" << endl;
    cout << "Average Waiting Time: " << avgWaitingTime << endl;
    cout << "Average Turnaround Time: " << (float)totalTurnaroundTime / n << endl;
    return avgWaitingTime;
}

int main() {
    int n, timeQuantum;
    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> processes(n);

    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        cout << "Enter arrival time, burst time and priority for process " << i + 1 << ": ";
        cin >> processes[i].arrivalTime >> processes[i].burstTime >> processes[i].priority;
        processes[i].remainingTime = processes[i].burstTime;
    }

    cout << "Enter time quantum for Round Robin scheduling: ";
    cin >> timeQuantum;

    // Make a copy of the original processes for each algorithm to work independently
    vector<Process> processesFCFS = processes;
    vector<Process> processesSJFNonPreemptive = processes;
    vector<Process> processesSJFPreemptive = processes;
    vector<Process> processesRR = processes;
    vector<Process> processesPriorityNonPreemptive = processes;
    vector<Process> processesPriorityPreemptive = processes;

    // Execute each scheduling algorithm and store the average waiting time
    float avgWaitingTimeFCFS = FCFS(processesFCFS);
    float avgWaitingTimeSJFNonPreemptive = SJFNonPreemptive(processesSJFNonPreemptive);
    float avgWaitingTimeSJFPreemptive = SJFPreemptive(processesSJFPreemptive);
    float avgWaitingTimeRR = RoundRobin(processesRR, timeQuantum);
    float avgWaitingTimePriorityNonPreemptive = PriorityNonPreemptive(processesPriorityNonPreemptive);
    float avgWaitingTimePriorityPreemptive = PriorityPreemptive(processesPriorityPreemptive);

    // Find the scheduling algorithm with the minimum average waiting time
    float minWaitingTime = min({avgWaitingTimeFCFS, avgWaitingTimeSJFNonPreemptive, avgWaitingTimeSJFPreemptive, avgWaitingTimeRR, avgWaitingTimePriorityNonPreemptive, avgWaitingTimePriorityPreemptive});

    cout << "The scheduling algorithm with the minimum average waiting time is: ";

    if (minWaitingTime == avgWaitingTimeFCFS) {
        cout << "FCFS" << endl;
    } else if (minWaitingTime == avgWaitingTimeSJFNonPreemptive) {
        cout << "SJF (Non-Preemptive)" << endl;
    } else if (minWaitingTime == avgWaitingTimeSJFPreemptive) {
        cout << "SJF (Preemptive)" << endl;
    } else if (minWaitingTime == avgWaitingTimeRR) {
        cout << "Round Robin" << endl;
    } else if (minWaitingTime == avgWaitingTimePriorityNonPreemptive) {
        cout << "Priority (Non-Preemptive)" << endl;
    } else if (minWaitingTime == avgWaitingTimePriorityPreemptive) {
        cout << "Priority (Preemptive)" << endl;
    }

    return 0;
}


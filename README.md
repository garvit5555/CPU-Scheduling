# CPU Scheduling Simulator

This project is a CPU scheduling simulator that supports various scheduling algorithms. It allows users to input process details and simulate the scheduling to either trace the execution timeline or show statistics of the processes.

## Supported Scheduling Algorithms
1. **First-Come, First-Served (FCFS)**
2. **Shortest Process Next (SPN)**
3. **Shortest Remaining Time (SRT)**
4. **Highest Response Ratio Next (HRRN)**
5. **Feedback Queue 1 (FB-1)**
6. **Feedback Queue 2 with Increasing Quantum (FB-2i)**

## Requirements
- C++11 or later
- Standard Template Library (STL)
### Cloning the Repository

- To clone the repository, use the following command:

  ```bash
  git clone https://github.com/garvit5555/CPU-Scheduling.git
  cd CPU-Scheduling
## Compilation

- To compile the code, use a C++ compiler such as `g++`. Below is an example     
  compilation command:
  ```
  g++ -std=c++11 -o main main.cpp

- Run the compiled executable and follow the prompts to enter the details of the   processes and choose a scheduling algorithm.
  ```
  ./main


- Example
  ```
  Enter the number of processes: 3
  Enter process name, arrival time, and burst time for process 1: P1 0 5
  Enter process name, arrival time, and burst time for process 2: P2 1 3
  Enter process name, arrival time, and burst time for process 3: P3 2 2
  Enter the last instant of simulation: 10
  Enter the operation (trace/stats): trace
  Enter the scheduling algorithm (FCFS, SPN, SRT, HRRN, FB-1, FB-2i): FCFS


- Output
  - Trace
  ```
  
    P1: *****     
    P2:  ***    
    P3:   **
  ```
  - Statistics
  ```
    Process Arrival Burst Finish Turnaround Normalized Turnaround
    P1      0      3     3      3          1.00
    P2      2      6     9      7          1.17
    P3      4      4     13     9          2.25

  
  
## Scheduling Algorithms
  - First-Come, First-Served (FCFS)
    - Processes are scheduled in the order of their arrival times.

  - Shortest Process Next (SPN)
    - The process with the shortest burst time is selected next.

  - Shortest Remaining Time (SRT)
    - The process with the shortest remaining burst time is selected next.

  - Highest Response Ratio Next (HRRN)
    - The process with the highest response ratio is selected next.
      Feedback Queue 1 (FB-1)
    - A feedback queue with a fixed priority level where each process starts at the   highest priority.

  - Feedback Queue 2 with Increasing Quantum (FB-2i)
    - A feedback queue with increasing quantum times. Each subsequent priority level 
      has a quantum twice as long as the previous level.
    
  - ![image](https://github.com/garvit5555/CPU-Scheduling/assets/153619593/df2b0d22-0596-4f11-9f36-5d28b559741f)


## Author
  - Author
    - Garvit(garvitjuneja200@gmail.com)
      

> **Note:**
> This project is a part of Operating Systems and Embedded System Design (OSED) course at KLETU.

# Automated Conveyor Load Detection System
This project implements an Automated Conveyor Load Detection System on a Cortex-M3 32-bit MCU using Keil-RTX, a real-time operating system (RTOS).
The system is designed to monitor conveyor load status and detect faults in real time.

## Key features
- **Task Scheduling**: The system features multiple tasks for handling load detection, status logging, and fault reporting. These tasks operate within the RTOS framework to ensure efficient and responsive system performance.

- **Synchronization Mechanisms**: Semaphores are used for task synchronization, enabling smooth and coordinated operation between concurrent tasks.

- **Event Handling**: Events are utilized for real-time signaling, allowing the system to respond promptly to changes in status or fault conditions.

- **Inter-Process Communication (IPC)**: Mailboxes facilitate communication between tasks, ensuring effective data transfer and coordination across the system.

- **On-chip components**: UART for displaying system status and Timer for generating delays;

- **Off-chip components**: LCD for motor status, DC Motor for simulating conveyor belt operation, and LED for indicating faulty cartons.

## Installation steps

1. - Fork the [repo](https://github.com/codedmachine111/RTOS-cp)
   - Clone the repo to your local machine `git clone https://github.com/codedmachine111/RTOS-cp.git`
   - Change current directory `cd RTOS-cp`
2. Install Keil uVision4 by following [this](https://users.ece.utexas.edu/~valvano/Volume1/KeilInstall.htm) tutorial.
3. Open the project file in Keil uVision4
```bash
./cp_1768.uvproj
```

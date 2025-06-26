# Problem Statements:

**Question 1:**
Using FreeRTOS, create two separate tasks. One listens for input over UART (from the Serial Monitor). Upon receiving a newline character (‘\n’), the task allocates a new section of heap memory (using pvPortMalloc()) and stores the string up to the newline character in that section of heap. It then notifies the second task that a message is ready.

The second task waits for notification from the first task. When it receives that notification, it prints the message in heap memory to the Serial Monitor. Finally, it deletes the allocated heap memory (using vPortFree()).

---

**Question 2:**
Using FreeRTOS, create two separate tasks. One listens for an integer over UART (from the Serial Monitor) and sets a variable when it sees an integer. The other task blinks the onboard LED (or other connected LED) at a rate specified by that integer. In effect, you want to create a multi-threaded system that allows for the user interface to run concurrently with the control task (the blinking LED).

---

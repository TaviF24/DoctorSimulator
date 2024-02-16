# Doctor Simulator

A project which simulates the activity of a medical cabinet using multithreading and synchronization. 

At random times, the patients enter in the cabinet. If there are doctors available, each patient will get a certain doctor where they will stay a random time (each patient has a different consultation time). Otherwise, if there are no doctors available, the remaining patiets will wait untill they will get one.                                 

Structures used:
- Threads: the patients
- Some structures that blocks the patients: the doctors
- Semaphores and mutexes: tools that help the synchronization process
- Queue: here I keep all the available doctors

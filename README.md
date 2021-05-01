* This code is a solution to a known peoblem called Producer Consumer problem.
Given a shared circular buffer, Consumer is trying to consume values from the buffer while the Producer trying to fill this buffer.<br>
* The solution is to synchronize between the two processes and to maintain expected values on the shared buffer.<br>
* It's also allowed to run multiple consumers and multipe producers on the same buffer with different speed of each operation simulated by sleep function.<br>
![alt text](http://pages.cs.wisc.edu/~bart/537/lecturenotes/figures/s6-prodcons.jpg)

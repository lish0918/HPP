本章包含以下内容:<br />
(1) Creating threads<br />
(2) Waiting for a thread to finish<br />
(3) Using the input argument to the thread function for input and/or output<br />
(4) Dividing work between threads<br />
(5) Checking what speedup we get when varying the number of threads used<br />
(6) Using mutex variables to achieve safe access to shared data<br /><br />


(1) Using the return value from a thread function when calling pthread_join().<br />
(2) Using a condition variable to allow a thread to wait until another thread has done something.<br />
(3) Using the second argument to pthread_create() to specify attributes for the created thread.<br />
(4) Creating threads as “joinable” or “detached”.<br />
(5) Parallelizing a few different kinds of computations using Pthreads.<br />
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h> //close function and other
#include <sys/wait.h> //for wait
#include <pthread.h> //for thread and mutex
#include <sys/stat.h> //for stat
#include <fcntl.h> // for file I/O
#include <string.h>
#include <dirent.h> //for DIR function
#include <ctype.h> //toupper, isdigit µî function
#include <signal.h> //for signal
#include <errno.h> //signal¿¡¼­ SIG_ERR
#include <sys/shm.h> //for shared memory system call
#include <sys/ipc.h>  //for shared memory system call
#include <sys/mman.h> //for mmap system call
#include <sys/time.h> //for ThreadUsleep function
#include <semaphore.h> //for POSIX semaphore
#include <sys/sem.h> //for System V semaphore
#include <sys/socket.h> //for socket
#include <netinet/in.h> // for socket
#include <arpa/inet.h> //for socket
#include <netdb.h> //for domain server

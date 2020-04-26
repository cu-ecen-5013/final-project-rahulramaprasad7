#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <mqueue.h>
#include <time.h> 
#include <semaphore.h>
#include "../module/eink_ioctl.h"

#define QUEUE_NAME   "/mqueue"
#define SEM_MUTEX_NAME "/sem-mutex"

#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

sem_t *mutex_sem;

void delay(int number_of_seconds) 
{ 
    // Converting time into milli_seconds 
    int milli_seconds = 1000 * number_of_seconds; 
  
    // Storing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not achieved 
    while (clock() < start_time + milli_seconds) 
        ; 
} 

int main(void)
{
    mqd_t qd_rx;
    struct mq_attr attr;

    if ((mutex_sem = sem_open (SEM_MUTEX_NAME, O_CREAT, 0660, 0)) == SEM_FAILED) {
        perror ("sem_open"); exit (1);
    }

    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    if ((qd_rx = mq_open (QUEUE_NAME, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
        perror ("Server: mq_open (server)");
        exit(-1);
    }
    char in_buffer[MSG_BUFFER_SIZE];
    int i = 0;

    while (1) {
        ++i;

        if (sem_post (mutex_sem) == -1) {
	        perror ("sem_post: mutex_sem"); exit (1);
        }
        // get the oldest message with highest priority
        if (mq_receive (qd_rx, in_buffer, MSG_BUFFER_SIZE, NULL) == -1) {
            perror ("Server: mq_receive");
            exit (1);
        }
        printf ("Server: message %d received: %s\n", i, in_buffer);

        delay(1000);

        if(i > 5)
            break;
    }
    mq_unlink(QUEUE_NAME);
    sem_unlink (SEM_MUTEX_NAME);



    return 0;
}
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
#include <math.h>
#include "../module/eink_ioctl.h"

#define QUEUE_NAME   "/mqueue"
#define SEM_MUTEX_NAME "/sem-mutex"

#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

#define DISP_WHITE 0
#define DISP_BLACK 1

#define DtoR(X) ((X) * M_PI/(double)180.00)

sem_t *mutex_sem;
int fd;

//pos-pos-X-Y-section
void writeSection(int posX, int posY, int width, int height, bool color, uint8_t *section)
{
    int index = posX + width * posY;
    section[index] = color;
}

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

void drawYaw(int yaw)
{
    struct pixelDataIn data;

    data.x = 0;
    data.y = 0;
    data.x1 = 200;
    data.y1 = 200;
    data.partLUT = true;

    uint8_t *sec = malloc(200 * 200);

    char test[] = "Write w/ IOCTL\0";
    data.stringIn = test;
    data.stringLength = strlen(test);


    for(int i = 0; i < 200; i ++)
    {
        for(int j = 0; j < 200; j++)
        {
            writeSection(i, j, 200, 200, DISP_WHITE, sec);
        }
    }

    data.sectionData = sec;
    ioctl(fd, EINKCHAR_IOCWRSECTION, &data);

    bool flipLine = true;
    if(yaw > 180)
    {
        flipLine = false;
        yaw -= 180;
        printf("Line flipped: Yaw: Old: %d New: %d\n", yaw + 180, yaw);
    }

    data.lineLength = 50;
    int x = round(50 * cos(DtoR(yaw)));
    int y = round(50 * sin(DtoR(yaw)));
    data.x = 100;
    data.y = 100;

    if(flipLine)
        data.x1 = x + 100;
    else
        data.x1 = x + 100 - x - x;

    if(flipLine)
        data.y1 = y + 100 - y - y;
    else
        data.y1 = y + 100;

    printf("Line: (%d, %d) Relative: (%d, %d)\n", x, y, data.x1, data.y1);

    if(flipLine)
    {
    //    data.y1 -= ;
    }

    ioctl(fd, EINKCHAR_IOCWRLUT, &data);
    ioctl(fd, EINKCHAR_IOCWRXYLINE, &data);

    free(sec);

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

    fd = open("/dev/einkChar", O_WRONLY);

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

        int yaw = 0, pitch = 0, roll = 0;
        if(sscanf(in_buffer, "Yaw: %4d Pitch: %4d Roll: %4d\0", &yaw, &pitch, &roll) != 3)
        {
            printf("Error in sscanf\n");
        }

        printf("Yaw: %d Pitch %d Roll %d\n", yaw, pitch, roll);
        drawYaw(yaw);

        delay(1000);

        if(i > 5)
            break;
    }
    mq_unlink(QUEUE_NAME);
    sem_unlink (SEM_MUTEX_NAME);
    close(fd);



    return 0;
}
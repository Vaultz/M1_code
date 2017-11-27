#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>           /* For O_* constants */
#include <mqueue.h>

// compiler avec  $ gcc -Wall mqueue_exemple.c -lrt


#define QUEUE_NAME  "/test_queue"
#define MAX_SIZE    1024
#define MSG_STOP    "exit"

enum {READER, WRITER} r_w;

void usage(char* n) {
    fprintf(stderr, "usage : %s [R|W]\n", n);
    exit(1);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        usage(argv[0]);
    } else {
        if (!strcmp(argv[1], "R"))
            r_w = READER;
        else if (!strcmp(argv[1], "W"))
            r_w = WRITER;
        else
            usage(argv[0]);
    }
    
    mqd_t mq;
    struct mq_attr attr;
    char buffer[MAX_SIZE+1];
    
    /* initialize the queue attributes */
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;
    
    /* open/create the mail queue */
    if (r_w == READER)
        mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);
    else
        mq = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0644, &attr);
    assert((mqd_t)-1 != mq);
    
    if (r_w == READER) {
        int must_stop = 0;
        do {
            ssize_t bytes_read;
            
            /* receive the message */
            bytes_read = mq_receive(mq, buffer, MAX_SIZE, NULL);
            assert(bytes_read >= 0);
            
            buffer[bytes_read] = '\0';
            if (!strncmp(buffer, MSG_STOP, strlen(MSG_STOP))) {
                must_stop = 1;
            } else {
                printf("Received: %s", buffer);
                sleep(3);
            }
        } while (!must_stop);
    } else {
        printf("Send to server (enter \"exit\" to stop it):\n");
        
        do {
            printf("> ");
            fflush(stdout);
            
            memset(buffer, 0, MAX_SIZE);
            fgets(buffer, MAX_SIZE, stdin);
            
            /* send the message */
            assert(0 <= mq_send(mq, buffer, MAX_SIZE, 0));
            
        } while (strncmp(buffer, MSG_STOP, strlen(MSG_STOP)));
    }
    
    /* cleanup */
    assert(mq_close(mq) != (mqd_t)-1);
    if (r_w == READER) assert(mq_unlink(QUEUE_NAME) != (mqd_t)-1);
    
    return 0;
}

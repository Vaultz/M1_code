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

typedef struct {
    union sigval sv;
    struct sigevent * se;
} sv_se;


static void                     /* Thread start function */
tfunc(union sigval arg)
{
    sv_se * args = arg.sival_ptr;
    struct mq_attr attr;
    ssize_t bytes_read;
    char *buffer;
    mqd_t mq = *((mqd_t *) args->sv.sival_ptr);
    
    /* Determine max. msg size; allocate buffer to receive msg */
    assert(mq_getattr(mq, &attr) != -1);
    buffer = malloc(attr.mq_msgsize);
    assert(buffer != NULL);
    
    /* receive the message */
    bytes_read = mq_receive(mq, buffer, attr.mq_msgsize, NULL);
    assert(bytes_read >= 0);
    buffer[bytes_read] = '\0';
    if (!strncmp(buffer, MSG_STOP, strlen(MSG_STOP))) {
        free(buffer);
        exit(EXIT_SUCCESS); /* Terminate the process */
    } else {
        printf("Received: %s", buffer);
        fflush(stdout);
    }
    free(buffer);
    assert(mq_notify(mq, args->se) != -1); // set new notification
    puts("end");
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
    struct sigevent se;
    struct mq_attr attr;
    char buffer[MAX_SIZE+1];
    
    /* initialize the queue attributes */
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;
    
    /* open/create the mail queue */
    if (r_w == READER) {
        mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);
        assert((mqd_t)-1 != mq);
        
        /* create the notifyer */
        //se.sigev_notify = SIGEV_SIGNAL;
        //se.sigev_signo = SIGUSR1;
        se.sigev_notify = SIGEV_THREAD;
        se.sigev_notify_function = tfunc;
        se.sigev_notify_attributes = NULL;
        sv_se args;
        args.sv.sival_ptr = &mq;
        args.se = &se;
        se.sigev_value.sival_ptr = (void *) &args;   /* Arg. to thread func. */
        assert(mq_notify(mq, &se) != -1);
    } else {
        mq = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0644, &attr);
        assert((mqd_t)-1 != mq);
    }
    
    if (r_w == READER) {
        while (1) {
            pause();
        }
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

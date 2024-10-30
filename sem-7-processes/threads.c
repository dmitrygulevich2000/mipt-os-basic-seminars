// for gettid()
#define _GNU_SOURCE

#include <pthread.h>
#include <sched.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void log_info(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char log_fmt[4096];
    snprintf(log_fmt, sizeof(log_fmt), "%d(tid: %ld): %s", getpid(), pthread_self(), fmt);
    vprintf(log_fmt, args);

    va_end(args);
}

void do_work(int tasks) {
    for (int i = 0; i < tasks; ++i) {
        // output order will be different
        log_info("done task %d\n", i);
    }
}

void* thread_fun(void* code) {
    log_info("running another thread\n");
    do_work(10);
    log_info("exiting\n");    
}

int main() {
    log_info("starting program\n");

    pthread_t thread;
    int status = pthread_create(&thread, NULL, thread_fun, NULL);
    if (status != 0) {
        fprintf(stderr, "pthread_create: %s", strerror(status));
        exit(1);
    }

    log_info("running main thread\n");
    do_work(10);

    status = pthread_join(thread, NULL);
    if (status != 0) {
        fprintf(stderr, "pthread_join: %s", strerror(status));
        exit(1);
    }
    log_info("thread joined\n");
    log_info("exiting\n");
}


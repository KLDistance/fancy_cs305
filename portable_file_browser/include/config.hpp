#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <list>
#include <queue>

#define DEF_PORT 4096
#define MAX_RECV_BUF 2048
#define MAX_CONCURRENCY_THREAD_NUM 4

extern const unsigned int MAX_VISITOR_NUM = 20;

#endif
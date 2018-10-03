#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include "config.h"
#include "message_queue.h"
#include "http_interpreter.h"

class TCP_Listener
{
private:
    int socket_fd;
    int accept_fd;
    sockaddr_in local_server;
    sockaddr_in remote_addr;
public:
    TCP_Listener(int listen_port);
    int recv_msg();
};

class TCP_Responser
{
private:
    int sending_target_fd;
public:
    TCP_Responser();
    int send_msg();
};

class Task_Manager
{
private:
    int shutter;
    size_t concurrency_num;
    sem_t concurrency_distributer;
    pthread_t listener_thread;
    pthread_t processor_threads[MAX_CONCURRENCY_THREAD_NUM];
    pthread_t responser_thread;

    // accept comming in requests
    static void* listener_thread_proc(void *lp_parameters);
    // process client business
    static void* processor_threads_proc(void *lp_parameters);
    // send processed message or resources
    static void* responser_thread_proc(void *lp_parameters);
public:
    Task_Manager();
    ~Task_Manager();
    // initialize semaphore states
    int init_thread_state(size_t init_concurrency_num);
    // start the threading tasks in the form of thread pool
    int thread_start();
    // increase threading concurrency
    int increase_loading_num(size_t addition_concurrency_num);
    // decrease threading concurrency
    int decrease_loading_num(size_t subtract_concurrency_num);
    // remove all threading concurrency
    int remove_all_loadings();
    // get threading concurrency number
    size_t get_present_concurrency_num();
};

#endif
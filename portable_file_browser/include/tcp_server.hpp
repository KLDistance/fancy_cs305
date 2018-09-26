#include "config.hpp"
#include "message_queue.hpp"

class TCP_Listener
{
private:
    int socket_fd = 0;
    int accept_fd = 0;
    sockaddr_in local_server;
    sockaddr_in remote_addr;
public:
    TCP_Listener(int listen_port);
    int recv_msg();
};

class TCP_Responser
{
private:
    int sending_target_fd = 0;
public:
    TCP_Responser();
    int send_msg();
};

class Task_Manager
{
private:
    pthread_t listener_thread;
    pthread_t processor_threads[MAX_CONCURRENCY_THREAD_NUM];
    pthread_t responser_thread;
public:
    int init_thread_state();
    int thread_start();
};
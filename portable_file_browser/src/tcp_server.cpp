#include "tcp_server.hpp"

TCP_Listener::TCP_Listener(int listen_port)
{
    if((this->socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        fprintf(stderr, "socket() failed.\n");
    }

    memset(&this->local_server, 0, sizeof(this->local_server));
    this->local_server.sin_family = AF_INET;
    this->local_server.sin_addr.s_addr = htonl(INADDR_ANY);
    this->local_server.sin_port = htons(listen_port);

    if(bind(this->socket_fd, (sockaddr*)&this->local_server, sizeof(this->local_server)) < 0)
    {
        fprintf(stderr, "bind() failed.\n");
    }

    if(listen(socket_fd, 10) < 0)
    {
        fprintf(stderr, "listen() failed.\n");
    }
}

int TCP_Listener::recv_msg()
{
    char client_request[MAX_RECV_BUF] = {0};
    memset(client_request, 0, MAX_RECV_BUF);
        
    while(1)
    {
        socklen_t sin_size = sizeof(sockaddr_in);
        if((this->accept_fd = accept(this->socket_fd, (sockaddr*)&this->remote_addr, &sin_size)) == -1)
        {
            fprintf(stderr, "accept() error!\n");
            close(this->accept_fd);
            continue;
        }
        // check connection
        printf("Received a connection from %s\n", (char*)inet_ntoa(remote_addr.sin_addr));

        // receive request from client
        if(read(this->accept_fd, client_request, MAX_RECV_BUF) < 0)
        {
            fprintf(stderr, "read() error!\n");
        }
        else
        {
            Message msg;
            msg.Init(client_request, this->accept_fd);
            recv_msg_list.push(msg);
        }
        // should not close this connection for later reply to the client!
        //close(this->accept_fd);
    }
}

TCP_Responser::TCP_Responser()
{

}

int TCP_Responser::send_msg()
{
    while(1)
    {
        Message sending_msg = send_msg_list.pop();
        if(send(sending_msg.GetAcceptFd(), sending_msg.GetMessageContent(), sending_msg.GetMessageLength(), 0) < 0)
        {
            fprintf(stderr, "send() error!\n");
        }
        else
        {
            sending_msg.Free();
        }
    }
}

int Task_Manager::init_thread_state(size_t init_concurrency_num)
{
    this->shutter = 0;
    this->concurrency_num = init_concurrency_num;
    sem_init(&this->concurrency_distributer, 0, this->concurrency_num);
    return 0;
}

int Task_Manager::thread_start()
{
    size_t i;
    pthread_create(&this->listener_thread, NULL, this->listener_thread_proc, NULL);
    pthread_create(&this->responser_thread, NULL, this->responser_thread_proc, NULL);
    for(i = 0; i < this->concurrency_num; i++)
    {
        pthread_create(&this->processor_threads[i], NULL, this->processor_threads_proc, NULL);
    }
    return 0;
}
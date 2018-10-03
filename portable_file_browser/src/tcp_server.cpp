#include "tcp_server.h"

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

Task_Manager::Task_Manager()
{

}

Task_Manager::~Task_Manager()
{
    
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
    
    // create threads
    pthread_create(&this->listener_thread, NULL, Task_Manager::listener_thread_proc, NULL);
    pthread_create(&this->responser_thread, NULL, Task_Manager::responser_thread_proc, NULL);
    for(i = 0; i < this->concurrency_num; i++)
    {
        pthread_create(&this->processor_threads[i], NULL, Task_Manager::processor_threads_proc, NULL);
    }
    
    // wait for termination or ending of joinable threads
    pthread_join(this->listener_thread, NULL);
    pthread_join(this->responser_thread, NULL);
    for(i = 0; i < this->concurrency_num; i++)
    {
        pthread_join(this->processor_threads[i], NULL);
    }
    return 0;
}

void* Task_Manager::listener_thread_proc(void *lp_parameters)
{
    TCP_Listener *tcp_listener = new TCP_Listener(DEF_PORT);
    while(1)
    {
        tcp_listener->recv_msg();
    }
    delete tcp_listener;
    return (void*)0;
}

void* Task_Manager::processor_threads_proc(void *lp_parameters)
{
    while(1)
    {
        HTTP_Receiving_Message_Header *recv_header = new HTTP_Receiving_Message_Header();
        Message recv_message = recv_msg_list.pop();
        HTTP_Interpreter::GenerateRecvHeader(recv_header, &recv_message);

        // sample message content
        char *message_content = NULL;
        sample_info_provider(&message_content);
        
        // sending message
        Message send_message;
        send_message.Init(message_content, recv_message.GetAcceptFd());
        send_msg_list.push(send_message);

        // release allocated memory
        delete recv_header;
    }
    return (void*)0;
}

void* Task_Manager::responser_thread_proc(void *lp_parameters)
{
    // TODO: multithreading on sending message ought to be updated
    TCP_Responser *tcp_responser = new TCP_Responser();
    while(1)
    {
        tcp_responser->send_msg();
    }
    delete tcp_responser;
    return (void*)0;
}

int Task_Manager::increase_loading_num(size_t addition_concurrency_num)
{

    return 0;
}

int Task_Manager::decrease_loading_num(size_t subtract_concurrency_num)
{

    return 0;
}

int Task_Manager::remove_all_loadings()
{

    return 0;
}

size_t Task_Manager::get_present_concurrency_num()
{
    return this->concurrency_num;
}

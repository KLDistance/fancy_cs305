#ifndef __MESSAGE_QUEUE_HPP__
#define __MESSAGE_QUEUE_HPP__

#include "config.hpp"

class Message
{
private:
    int accept_fd = NULL;
    char* msg = NULL;
    size_t msg_len = 0;
public:
    Message();
    ~Message();
    void Init(const char *m, int accept_fd);
    void Free();
    char* GetMessageContent();
    size_t GetMessageLength();
    int GetAcceptFd();
};

class Msg_List
{
private:
    pthread_mutex_t lock;
    pthread_cond_t cond;
    std::list<Message> msg_list;
public:
    Msg_List();
    ~Msg_List();
    void push(Message m);
    Message pop();
};

// global message list
extern Msg_List recv_msg_list;
extern Msg_List send_msg_list;

#endif
#ifndef __MESSAGE_QUEUE_H__
#define __MESSAGE_QUEUE_H__

#include "config.h"

class Message
{
private:
    int accept_fd;
    char* msg;
    size_t msg_len;
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

extern Msg_List recv_msg_list;
extern Msg_List send_msg_list;

#endif
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

class Recv_Msg_List
{
private:
    pthread_mutex_t lock;
    pthread_cond_t cond;
    std::list<Message> msg_list;
public:
    Recv_Msg_List();
    ~Recv_Msg_List();
    void push(Message m);
    Message pop();
};

class Send_Msg_List
{
private:
    pthread_mutex_t lock;
    pthread_cond_t cond;
    std::list<Message> msg_list;
public:
    Send_Msg_List();
    ~Send_Msg_List();
    void push(Message m);
    Message pop();
};

// global message list
extern Recv_Msg_List recv_msg_list;
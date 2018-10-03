#include "message_queue.h"

Msg_List recv_msg_list;
Msg_List send_msg_list;

Message::Message()
{
    this->accept_fd = 0;
    this->msg = NULL;
    this->msg_len = 0;
}

Message::~Message()
{
    
}

void Message::Init(const char *m, int accept_fd)
{
    unsigned int msg_string_length = strlen(m);
    if(m && msg_string_length > 0)
    {
        this->msg = (char*)malloc(msg_string_length + 1);
        if(this->msg)
        {
            memset(this->msg, 0, msg_string_length + 1);
            strncpy(this->msg, m, msg_string_length + 1);
        }
    }
    this->msg_len = msg_string_length;
    this->accept_fd = accept_fd;
}

void Message::Free()
{
    if(this->msg) free(this->msg);
    if(this->msg_len) this->msg_len = 0;
    if(this->accept_fd) this->accept_fd = 0;
}

char* Message::GetMessageContent()
{
    return this->msg;
}

size_t Message::GetMessageLength()
{
    return this->msg_len;
}

int Message::GetAcceptFd()
{
    return this->accept_fd;
}

Msg_List::Msg_List()
{
    pthread_mutex_init(&this->lock, NULL);
    pthread_cond_init(&this->cond, NULL);
}

Msg_List::~Msg_List()
{

}

void Msg_List::push(Message m)
{
    pthread_mutex_lock(&this->lock);
    this->msg_list.push_back(m);
    pthread_mutex_unlock(&this->lock);
    pthread_cond_signal(&this->cond);
}

Message Msg_List::pop()
{
    int get = 0;
    Message message;
    do
    {
        if(!this->msg_list.empty())
        {
            message = this->msg_list.front();
            this->msg_list.pop_front();
            get = 1;
        }
        else
        {
            pthread_mutex_lock(&this->lock);
            pthread_cond_wait(&this->cond, &this->lock);
            pthread_mutex_unlock(&this->lock);
        }
    } while(!get);
    return message;
}
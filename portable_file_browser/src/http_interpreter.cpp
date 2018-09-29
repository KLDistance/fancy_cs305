#include "http_interpreter.hpp"

HTTP_Sending_Message_Header::HTTP_Sending_Message_Header()
{

}

HTTP_Sending_Message_Header::HTTP_Sending_Message_Header(int http_ver, int conn_stat, int content, int charset, size_t content_len)
{
    this->http_version = http_ver;
    this->connection_state = conn_stat;
    this->content_type = content;
    this->charset_type = charset;
    this->content_length = content_len;
}

HTTP_Sending_Message_Header* HTTP_Sending_Message_Header::GetHeader()
{
    return this;
}

int HTTP_Sending_Message_Header::GetHTTPVersion()
{
    return this->http_version;
}

int HTTP_Sending_Message_Header::GetConnectionState()
{
    return this->connection_state;
}

int HTTP_Sending_Message_Header::GetContentType()
{
    return this->content_type;
}

int HTTP_Sending_Message_Header::GetCharsetType()
{
    return this->charset_type;
}

size_t HTTP_Sending_Message_Header::GetContentLength()
{
    return this->content_length;
}

HTTP_Receiving_Message_Header::HTTP_Receiving_Message_Header()
{

}

HTTP_Receiving_Message_Header::HTTP_Receiving_Message_Header(int request, const char *host, const char *content, size_t content_len)
{

}

int HTTP_Receiving_Message_Header::GetHTTPVersion()
{
    return this->http_version;
}

int HTTP_Receiving_Message_Header::GetRequestType()
{
    return this->request_type;
}

char* HTTP_Receiving_Message_Header::GetHostName()
{
    return this->host_name;
}

char* HTTP_Receiving_Message_Header::GetContentType()
{
    return this->content_type;
}

size_t HTTP_Receiving_Message_Header::GetContentLength()
{
    return this->content_length;
}

int HTTP_Interpreter::GenerateRecvHeader(HTTP_Receiving_Message_Header *recv_header, Message *recv_message)
{
    // "i" is text iter, "j" is dividor iter, "k" is tmp iter
    size_t i, j, k;
    size_t whole_content_size = recv_message->GetMessageLength();
    int error_code = 0;
    char tmp_buf[512] = {0};
    char *message_whole_content = recv_message->GetMessageContent();

    // check request type from client
    for(i = 0, k = 0; i < whole_content_size; i++)
    {
        
    }

    // check validity of http protocol version
    for(i = 0, j = 0; i < whole_content_size; i++)
    {
        if(message_whole_content[i] == ' ') j++;
        if(j == 2)
        {
            i++;
            break;
        }
    }
    for(k = i;k < whole_content_size; k++)
    {
        if(message_whole_content[k] == '\r') break;
    }
    memset(tmp_buf, 0, 512);
    strncpy(tmp_buf, (message_whole_content + i), k - i);
    recv_header->GetHTTPVersion = HTTP_Interpreter::HTTPVersionInList(tmp_buf);

    return 0;
}

int RequestTypeInList(const char *request_section)
{
    if(!strncmp(request_section, "HEAD", strlen("HEAD")))
    {
        return REQUEST_HEAD;
    }
    else if(!strncmp(request_section, "GET", strlen("GET")))
    {
        return REQUEST_GET;
    }
    else if(!strncmp(request_section, "POST", strlen("POST")))
    {
        return REQUEST_POST;
    }
    else
    {
        return REQUEST_MESS_UP;
    }
}

int HTTP_Interpreter::HTTPVersionInList(const char *protocol_section)
{
    if(!strncmp(protocol_section, "HTTP/1.0", strlen("HTTP/1.0")))
    {
        return HTTP_VERSION_1_0;
    }
    else if(!strncmp(protocol_section, "HTTP/1.1", strlen("HTTP/1.1")))
    {
        return HTTP_VERSION_1_1;
    }
    else if(!strncmp(protocol_section, "HTTP/2.0", strlen("HTTP/2.0")))
    {
        return HTTP_VERSION_2_0;
    }
    else
    {
        return HTTP_ILLEGAL_VERSION;
    }
}


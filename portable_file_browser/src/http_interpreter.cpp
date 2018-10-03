#include "http_interpreter.h"

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
    this->http_version = HTTP_VERSION_1_0;
    this->request_type = REQUEST_HEAD;
    this->content_length = 0;

    // clear string buffer
    memset(this->default_file, 0, MAX_DEFAULT_FILE_LEN);
    memset(this->host_name, 0, MAX_HOST_NAME_LEN);
    memset(this->content_type, 0, MAX_CONTENT_TYPE_LEN);
}

HTTP_Receiving_Message_Header::HTTP_Receiving_Message_Header(int request, const char *host, const char *content, size_t content_len)
{
    HTTP_Receiving_Message_Header();
}

int HTTP_Receiving_Message_Header::SetHTTPVersion(int version)
{
    this->http_version = version;
    return 0;
}

int HTTP_Receiving_Message_Header::SetRequestType(int request_type)
{
    this->request_type = request_type;
    return 0;
}

int HTTP_Receiving_Message_Header::SetHostName(const char *host_name)
{
    strncpy(this->host_name, host_name, strlen(host_name));
    return 0;
}

int HTTP_Receiving_Message_Header::SetContentType(const char *content_type)
{
    strncpy(this->content_type, content_type, strlen(content_type));
    return 0;
}

int HTTP_Receiving_Message_Header::SetDefaultRequestLocation(const char *path)
{
    strncpy(this->default_file, path, strlen(path));
    return 0;
}

int HTTP_Receiving_Message_Header::SetUserAgent(const char *user_agent)
{
    strncpy(this->user_agent, user_agent, strlen(user_agent));
    return 0;
}

int HTTP_Receiving_Message_Header::SetAcceptInfo(const char *accept_info)
{
    strncpy(this->accept_info, accept_info, strlen(accept_info));
    return 0;
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
    size_t i, j, k;
    int state_code = 0;
    char *tmp_location_ptr = NULL;
    char *content_message = recv_message->GetMessageContent();
    size_t content_length = recv_message->GetMessageLength();

    // reserve info buffer with 1024 bytes
    char tmp_buf[MAX_LOCAL_TMP_BUF];
    memset(tmp_buf, 0, MAX_LOCAL_TMP_BUF);

    // extract info of first line of request

    // request type
    for(i = 0, j = 0, k = 0; i < content_length; i++)
    {
        if(content_message[i] == ' ') break;
    }
    strncpy(tmp_buf, content_message, i - j);
    state_code = HTTP_Interpreter::RequestTypeInList(tmp_buf);
    CHECK_STATE(state_code);
    recv_header->SetHTTPVersion(state_code);
    memset(tmp_buf, 0, MAX_LOCAL_TMP_BUF);

    // default term or page of target host
    j = ++i;
    for(;i < content_length; i++)
    {
        if(content_message[i] == ' ') break;
    }
    strncpy(tmp_buf, content_message, i - j);
    recv_header->SetDefaultRequestLocation(tmp_buf);
    memset(tmp_buf, 0, MAX_LOCAL_TMP_BUF);

    // http version
    j = ++i;
    for(;i < content_length; i++)
    {
        if(content_message[i] == '\r' || content_message[i] == ' ') break;
    }
    strncpy(tmp_buf, content_message, i - j);
    state_code = HTTP_Interpreter::HTTPVersionInList(tmp_buf);
    CHECK_STATE(state_code);
    recv_header->SetHTTPVersion(state_code);
    memset(tmp_buf, 0, MAX_LOCAL_TMP_BUF);

    // server host name
    j = ++i;
    k = HTTP_Interpreter::HTTPHeaderKMP(
        content_message,
        "Host",
        tmp_buf,
        &tmp_location_ptr,
        i
    );
    recv_header->SetHostName(tmp_buf);
    memset(tmp_buf, 0, k < MAX_LOCAL_TMP_BUF ? k : MAX_LOCAL_TMP_BUF);

    // user agent
    j = ++i;
    k = HTTP_Interpreter::HTTPHeaderKMP(
        content_message, 
        "User-Agent",
        tmp_buf,
        &tmp_location_ptr,
        i
    );
    recv_header->SetUserAgent(tmp_buf);
    memset(tmp_buf, 0, k < MAX_LOCAL_TMP_BUF ? k : MAX_LOCAL_TMP_BUF);

    // accept info
    j = ++i;
    k = HTTP_Interpreter::HTTPHeaderKMP(
        content_message,
        "Accept",
        tmp_buf,
        &tmp_location_ptr,
        i
    );
    recv_header->SetAcceptInfo(tmp_buf);
    memset(tmp_buf, 0, k < MAX_LOCAL_TMP_BUF ? k : MAX_LOCAL_TMP_BUF);

    // TODO: there is some content of body arrives at server from client, but currently I do not handle it

    return 0;
}

int HTTP_Interpreter::GenerateSendHeader(HTTP_Sending_Message_Header *send_header, Message *send_message)
{
    
    return 0;
}

int HTTP_Interpreter::GenerateSendMessage(HTTP_Receiving_Message_Header *recv_header, Message *recv_message, Message **new_send_message)
{
    if(*new_send_message) 
    {
        delete *new_send_message;
        *new_send_message = NULL;
    }
    // we use a sample page to pass back to user
    char *content_body = NULL;

    sample_info_provider(&content_body);
    

    return 0;
}

int HTTP_Interpreter::RequestTypeInList(const char *request_section)
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

size_t HTTP_Interpreter::HTTPHeaderKMP(
    const char *message_content, 
    const char *target_content,
    char *content_out_buf,
    char **to_target_ptr_in_content,
    size_t start_index
    )
{
    size_t i, j, k;
    size_t message_length = strlen(message_content);
    size_t target_message_length = strlen(target_content);

    // clear pass in message buffer container
    memset(content_out_buf, 0, MAX_LOCAL_TMP_BUF);

    // start KMP patterning
    for(i = start_index, j = 0, k = 0; i < message_length; i++)
    {
        if(message_content[i] == target_content[0])
        {
            if(target_message_length == 1)
            {
                k = 1;
                break;
            }
            for(j = 1; j < target_message_length; j++)
            {
                if(target_content[j] != message_content[i + j]) break;
                else if(j == target_message_length - 1) k = 1;
            }
            i += j;
            if(k) break;
        }
        else if(
            message_content[i] == '\r' &&
            message_content[i + 1] == '\n' &&
            message_content[i + 2] == '\r' &&
            message_content[i + 3] == '\n'
            )
            {
                // points to next body
                *to_target_ptr_in_content = (char*)(message_content + i + 4);
                return 0xffffffffffffffff;
            }
    }
    if(k)
    {
        for(;i < message_length; i++)
        {
            if(message_content[i] != ':' || message_content[i] != ' ') break;
        }
        for(j = i; j < message_length; j++)
        {
            if(
                message_content[j] == '\r' || 
                message_content[j] == ';' || 
                message_content[j] == ' ' || 
                message_content[j] == '\n'
                ) 
                break;
        }
        strncpy(content_out_buf, (char*)(message_content + i), j - i);
        *to_target_ptr_in_content = (char*)(message_content + i);
        return j - i;
    }
    else
    {
        *to_target_ptr_in_content = NULL;
        return 0;
    }
}


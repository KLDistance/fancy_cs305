#ifndef __HTTP_INTERPRETER_HPP__
#define __HTTP_INTERPRETER_HPP__

#include "config.hpp"
#include "message_queue.hpp"

class HTTP_Receiving_Message_Header
{
private:
    // default configuration
    int http_version = HTTP_VERSION_1_0;
    int request_type = REQUEST_HEAD;
    char host_name[MAX_HOST_NAME_LEN];
    char content_type[MAX_CONTENT_TYPE_LEN];
    size_t content_length = 0;
public:
    HTTP_Receiving_Message_Header();
    HTTP_Receiving_Message_Header(int request, const char *host, const char *content, size_t content_len);
    int GetHTTPVersion();
    int GetRequestType();
    char* GetHostName();
    char* GetContentType();
    size_t GetContentLength();
};

class HTTP_Sending_Message_Header
{
private:
    // default configuration
    int http_version = HTTP_VERSION_1_0;
    int connection_state = CONNECTION_CLOSE;
    int content_type = FILE_TYPE_TEXT_HTML;
    int charset_type = CHARSET_UTF8;
    size_t content_length = 0;
public:
    HTTP_Sending_Message_Header();
    HTTP_Sending_Message_Header(int http_ver, int conn_stat, int content, int charset, size_t content_len);
    int GetHTTPVersion();
    int GetConnectionState();
    int GetContentType();
    int GetCharsetType();
    int GetContentLength();
    HTTP_Sending_Message_Header* GetHeader();
};

class HTTP_Interpreter
{
private:
    static int RequestTypeInList(const char *request_section);
    static int HTTPVersionInList(const char *protocol_section);
    static char* HTTPHeaderKMP(const char *message_content, size_t start_index = 0);
public:
    static int GenerateRecvHeader(HTTP_Receiving_Message_Header *recv_header, Message *recv_message);
    static int GenerateSendHeader(HTTP_Sending_Message_Header *send_header, Message *send_message);
};

#endif
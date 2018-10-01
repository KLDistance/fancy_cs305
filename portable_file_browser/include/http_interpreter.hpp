#ifndef __HTTP_INTERPRETER_HPP__
#define __HTTP_INTERPRETER_HPP__

#include "config.hpp"
#include "message_queue.hpp"
#include "content_provider.hpp"

#define CHECK_STATE(code){if(!(code)){fprintf(stderr, "error code %d\n", (code)); return -1;}} 

class HTTP_Receiving_Message_Header
{
private:
    // default configuration

    // request http version
    int http_version;
    // request type: GET, HEAD, POST and so on
    int request_type;
    // request entry file, for instance, index.php
    char default_file[MAX_DEFAULT_FILE_LEN];
    // local server name
    char host_name[MAX_HOST_NAME_LEN];
    // request content type
    char content_type[MAX_CONTENT_TYPE_LEN];
    // client used agent type
    char user_agent[MAX_USER_AGENT_LEN];
    // accept information
    char accept_info[MAX_ACCEPT_INFO];
    // content length
    size_t content_length;
public:
    // TODO: change the declaration of this object to be heap-only, and stack-forbidden.
    HTTP_Receiving_Message_Header();
    HTTP_Receiving_Message_Header(int request, const char *host, const char *content, size_t content_len);
    
    // set methods
    int SetHTTPVersion(int version);
    int SetRequestType(int request_type);
    int SetHostName(const char *host_name);
    int SetContentType(const char *content_type);
    int SetDefaultRequestLocation(const char *path);
    int SetUserAgent(const char *user_agent);
    int SetAcceptInfo(const char *accept_info);

    // get methods
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
    
    // http version of response side
    int http_version;
    // connection state
    int connection_state;
    // response content type
    int content_type;
    // content encoding type
    int charset_type;
    // response content length
    size_t content_length;
public:
    // TODO: change the declaration of this object to be heap-only, and stack-forbidden.
    HTTP_Sending_Message_Header();
    HTTP_Sending_Message_Header(int http_ver, int conn_stat, int content, int charset, size_t content_len);
    
    // get methods
    int GetHTTPVersion();
    int GetConnectionState();
    int GetContentType();
    int GetCharsetType();
    size_t GetContentLength();
    HTTP_Sending_Message_Header* GetHeader();
};

class HTTP_Interpreter
{
protected:
    HTTP_Interpreter();
    ~HTTP_Interpreter();
public:
    static int GenerateRecvHeader(HTTP_Receiving_Message_Header *recv_header, Message *recv_message);
    static int GenerateSendHeader(HTTP_Sending_Message_Header *send_header, Message *send_message);
    
    // generate the sending message according to the recv header and content provider machine
    static int GenerateSendMessage(HTTP_Receiving_Message_Header *recv_header, Message *recv_message, Message **new_sending_message);

    // check whether request type in list
    static int RequestTypeInList(const char *request_section);
    // check whether http version in list
    static int HTTPVersionInList(const char *protocol_section);
    // regular compare the string using KMP opt and return with the length of the content
    // if 0xffffffffffffffff return, it means the ptr turns to next body
    static size_t HTTPHeaderKMP(
        const char *message_content, 
        const char *target_content,
        char *content_out_buf,
        char **to_target_ptr_in_content,
        size_t start_index = 0
    );
};

#endif
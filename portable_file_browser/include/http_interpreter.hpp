#ifndef __HTTP_INTERPRETER_HPP__
#define __HTTP_INTERPRETER_HPP__

#include "config.hpp"
#include "message_queue.hpp"

#define CHECK_STATE(code) (                             \
    if (!(code)) {                                      \
        fprintf(stderr, "server error %d\n", code);     \
        return 0;                                       \
    }                                                   \
)

class HTTP_Receiving_Message_Header
{
private:
    // default configuration
    int http_version = HTTP_VERSION_1_0;
    int request_type = REQUEST_HEAD;
    char default_file[MAX_DEFAULT_FILE_LEN];
    char host_name[MAX_HOST_NAME_LEN];
    char content_type[MAX_CONTENT_TYPE_LEN];
    size_t content_length = 0;
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
    int http_version = HTTP_VERSION_1_0;
    int connection_state = CONNECTION_CLOSE;
    int content_type = FILE_TYPE_TEXT_HTML;
    int charset_type = CHARSET_UTF8;
    size_t content_length = 0;
public:
    // TODO: change the declaration of this object to be heap-only, and stack-forbidden.
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
protected:
    HTTP_Interpreter();
    ~HTTP_Interpreter();
public:
    static int GenerateRecvHeader(HTTP_Receiving_Message_Header *recv_header, Message *recv_message);
    static int GenerateSendHeader(HTTP_Sending_Message_Header *send_header, Message *send_message);

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
        int start_index = 0
    );
};

#endif
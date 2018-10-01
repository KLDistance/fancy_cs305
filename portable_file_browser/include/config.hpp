#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string>
#include <list>
#include <queue>

// notifier for parameters
#define IN 
#define OUT

// basic configuration of server IO
#define DEF_PORT 80

// recv message buffer size
#define MAX_RECV_BUF 2048
// local access tmp buffer size
#define MAX_LOCAL_TMP_BUF 1024
// host name buffer size
#define MAX_HOST_NAME_LEN 256
// content type/name buffer size
#define MAX_CONTENT_TYPE_LEN 256
// default file buffer size
#define MAX_DEFAULT_FILE_LEN 128
// user agent buffer size
#define MAX_USER_AGENT_LEN 128
// accept information buffer size
#define MAX_ACCEPT_INFO 128

// send message buffer size
#define MAX_SEND_BUF 8192

#define MAX_CONCURRENCY_THREAD_NUM 4

// HTTP configuration and interpreting options

// HTTP version
#define HTTP_ILLEGAL_VERSION 0
#define HTTP_VERSION_1_0 1
#define HTTP_VERSION_1_1 2
#define HTTP_VERSION_2_0 3

// request type
#define REQUEST_MESS_UP 0
#define REQUEST_HEAD 1
#define REQUEST_GET 2
#define REQUEST_POST 3

// connection strategy
#define CONNECTION_CLOSE 1
#define CONNECTION_KEEP_ALIVE 2

// connection status
#define STATUS_OK 200
#define STATUS_BAD_REQUEST 400
#define STATUS_FORBIDDEN 403
#define STATUS_NOT_FOUND 404
#define STATUS_PAYLOAD_TOO_LARGE 413

// transmission file type
#define FILE_TYPE_TEXT_HTML 1

// default charset
#define CHARSET_UTF8 1
#define CHARSET_UNICODE 2
#define CHARSET_GB2312 3
#define CHARSET_GBK 4

// control the max concurrency number
extern const unsigned int MAX_VISITOR_NUM = 20;

#endif
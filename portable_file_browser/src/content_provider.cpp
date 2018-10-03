#include "content_provider.h"

int sample_info_provider(char **message_content)
{
    if(*message_content)
    {
        free(*message_content);
        *message_content = NULL;
    }

    char *file_body_content = NULL;
    
    FILE *pFile = fopen("../dep/sample.html", "r");
    size_t file_size = 0;

    if(!pFile)
    {
        fprintf(stderr, "sample.html file lost.\n");
        return -1;
    }
    fseek(pFile, 0, SEEK_END);
    file_size = ftell(pFile);
    rewind(pFile);

    file_body_content = (char*)malloc(file_size + 1);
    if(!file_body_content)
    {
        fprintf(stderr, "memory allocation failed.\n");
        fclose(pFile);
        return -1;
    }
    memset(file_body_content, 0, file_size + 1);

    fread(file_body_content, file_size, 1, pFile);
    fclose(pFile);
    
    // add headers
    *message_content = (char*)malloc(file_size + 2001);
    memset(*message_content, 0, file_size + 2001);
    sprintf(
        *message_content,
        "                                               \
        HTTP/1.0 200 OK\r\n                             \
        Connection: close\r\n                           \
        Server: portable_file_browser/0.0.1\r\n         \
        Content-Type: text/html; charset=utf-8\r\n      \
        Content-Length: %lu\r\n\r\n                     \
        %s\r\n\r\n                                      \
        ",
        file_size,
        file_body_content
    );
    
    free(file_body_content);

    return 0;
}
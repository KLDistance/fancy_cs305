#ifndef __CONTENT_PROVIDER_H__
#define __CONTENT_PROVIDER_H__

#include "config.h"

extern int sample_info_provider(char **message_content);

// define the single file with multifunctions

// state: file is in buffer instead of in disk
#define FP_STATE_IN_BUFFER                  0x00000001
// state: file is in occupy and cannot be writen by other thread or proc
#define FP_STATE_IN_OCCUPY                  0x00000002
// state: file writing permission
#define FP_STATE_WRITE_PERMITTED            0x00000004
// state: is native file, not received 
#define FP_STATE_RESIDENTAL                 0x00000008
// state: 

class Binary_Sheet
{
private:
    // file name
    char *binary_sheet_name;
    // file size
    size_t binary_sheet_size;
    // file state
    uint binary_sheet_state;
};

// define the file pages content
class Page_Layout
{
private:
    // history data current list of binary-file array
    std::list<Binary_Sheet*> sheet_bundle_layers;
    // history data current list of array size
    std::list<size_t> sheet_size_layers;
public:
    Page_Layout();
    ~Page_Layout();
};

// define the selective browser
class Server_Page_Provider
{
private:
    //
public:
    Server_Page_Provider();
};

#endif
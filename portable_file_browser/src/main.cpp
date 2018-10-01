#include "tcp_server.hpp"

int main()
{
    // create task manager
    Task_Manager *task_manager = new Task_Manager();
    task_manager->init_thread_state(4);
    
    // run threads
    task_manager->thread_start();

    // clear allocated memory
    delete task_manager;
    
    return 0;
}
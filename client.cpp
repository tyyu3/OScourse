/*
Щербаков Игорь
Вариант №4

Клиент. Создать очередь сообщений. Передать в эту очередь список активных процессов,
присутствующих в системе, не управляемых терминалами.
*/
#include "shared.hpp"
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <iostream>
#include <cstring>

int main()
{
    try {


    std::cout << now_time() << "\tclient start\n";
    std::string process_list = capture_cmd_out("ps -o pid= -o cmd= --deselect ");
    std::cout << now_time() << "\tprocess list ready, size is " << process_list.size() << "\n\t" <<"max message size is " << MSGSZ << "\n";
    int mq_id = create_or_open_mq();
    std::cout << now_time() << "\tqueue open\n";
    msgbuf_t msg;
    msg.mtype = 3;
    if (process_list.size() >= sizeof(msg.mtext))
    {
        std::cout << now_time() << "\tno msg sent, size too big!\n";
        return 0;
    }
    else
    {
        strcpy(msg.mtext, process_list.c_str());
    }
    if (msgsnd(mq_id, &msg, sizeof(msg.mtext), IPC_NOWAIT) == -1)
    {
        std::perror("msgsnd error");
        throw std::runtime_error("msgsnd error");
    }
    else
    {
        std::cout << now_time() << " msg sent\n";
    }
    }  catch  (const std::exception& e)
    {
      std::cerr << e.what() << std::endl;
    }
    return 0;
}

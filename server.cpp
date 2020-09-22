/*
Щербаков Игорь
Вариант №4

Сервер. Выбрать из очереди сообщений, созданной клиентом, последнее сообщение.
Определить в полученном списке процессов те процессы, которые находятся в оперативной памяти,
а также общее число сообщений в очереди. Записать в стандартный
файл вывода эти данные, после чего удалить очередь сообщений.
*/

#include "shared.hpp"
#include <sys/types.h>
#include <iostream>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sstream>

void delete_mq(int mq_id)
{
    if (msgctl(mq_id, IPC_RMID, nullptr) == -1)
    {
        std::perror("msgctl delete mq error");
        throw std::runtime_error("msgctl delete mq error");
    }
}

class MessageQueue
{
public:
    MessageQueue() : id(create_or_open_mq()) {}

    ~MessageQueue()
    {
        try
        {
            delete_mq(id);
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    const int id;
};

int main()
{
    try {
    std::cout << now_time() << "\tserver start\n";
    MessageQueue queue;
    std::cout << now_time() << "\tqueue open\n";
    msgbuf_t msg;
    if (msgrcv(queue.id, &msg, sizeof(msg.mtext), 3, 0) == -1)
    {
        std::perror("msgrcv error");
        throw std::runtime_error("msgrcv error");
    }
    std::string rcvd(msg.mtext);
    std::cout << now_time() << "\tprocess list recieved\n";
    //std::cout<< rcvd<<"\n";
    std::string pid, name, cmd;
    std::stringstream is(rcvd);
    std::cout<<"PROCESSES IN RAM:\n";
    while (is >> pid >> name)
    {
        cmd = "ps -p " + pid + " -o rss="; //determine resident set size
       // std::cout << cmd <<"\n";
        if(stoi(capture_cmd_out(cmd))!=0)
        {
            std::cout << name <<"\n";
        }
        else
        {
            //std::cout << "Not! " << name <<"\n";
        }
        is.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //ignore params
    }

    struct msqid_ds ds;

    if (msgctl(queue.id, IPC_STAT, &ds) == -1)
    {
      perror ("msgctl error");
      throw std::runtime_error("msgctl error");
    }
    std::cout <<"NUMBER OF MESSAGES IN QUEUE:\n" << ds.msg_qnum << "\n";

    }  catch (const std::exception& e) {
       std::cerr << e.what() << std::endl;
    }
    return 0;
}

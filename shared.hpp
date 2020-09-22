#pragma once

#include <sys/types.h>
#include <string>
#include <ctime>

std::string now_time();


constexpr std::size_t MSGSZ = 1024*7;
struct msgbuf_t
{
    long mtype;
    char mtext[MSGSZ];
};


key_t get_key();

std::string capture_cmd_out(const std::string& cmd);

int create_or_open_mq();

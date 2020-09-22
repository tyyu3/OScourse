#include "shared.hpp"

#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#include <fcntl.h>//creat()

#include <string>
#include <stdexcept>
#include <array>

constexpr int PROJECT_ID = 1337;
const std::string PATH = "/tmp/lab_mq";

std::string now_time()
{
    time_t currentTime;
    struct tm *localTime;

    time( &currentTime );                   // Get the current time
    localTime = localtime( &currentTime );  // Convert the current time to the local time
    std::string res ="[" + std::to_string(localTime->tm_min) + ":" + std::to_string(localTime->tm_sec) + "]";
    return res;
}

key_t get_key()
{

    creat(PATH.c_str(), 0600);
    key_t key = ftok(PATH.c_str(), PROJECT_ID);
    if (key == -1)
    {
        std::perror("Cannot get a key with ftok");
        throw std::runtime_error("Cannot get a key with ftok");
    }
    return key;
}

int create_or_open_mq()
{
    key_t key = get_key();
    int mq_id = msgget(key, IPC_CREAT | 0600);
    if (mq_id == -1)
    {
        throw std::runtime_error("MQ creation or open error");
    }

    return mq_id;
}

std::string capture_cmd_out(const std::string& cmd)
{
    std::string data;

    const int max_buffer = 4096;
    std::array<char, max_buffer> buffer;

    std::string new_cmd = cmd/* + " 2>&1"*/;

    FILE* pipe = popen(new_cmd.c_str(), "r");
    if (pipe != nullptr)
    {
        while (fgets(buffer.data(), max_buffer, pipe) != nullptr) {
            data += buffer.data();
        }
        pclose(pipe);
    }
    return data;
}

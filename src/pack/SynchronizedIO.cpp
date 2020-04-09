#include "archiver.h"
#include "SynchronizedIO.h"
using namespace std;

int SynchronizedIO::read(int id, char *buff)
{
    // printf("entry: %d | last_reader: %d | next: %d\n", id, last_reader, next_id(last_reader));
    while (id != next_id(last_reader))
    {
        // printf("cycle: %d\n", id);
        this_thread::sleep_for(chrono::microseconds(100));
    }
    // printf("in: %d\n", id);
    ifs->read(buff, BUFF_SIZE);
    last_reader = id;
    // printf("out: %d\n", id);
    return (ifs->gcount());
}

int SynchronizedIO::write(int id, vector<char> *buffer)
{
    while (id != next_id(last_writer))
    {
        this_thread::sleep_for(chrono::microseconds(100));
    }
    ofs->write(buffer->data(), buffer->size());
    last_writer = id;
    return (0);
}
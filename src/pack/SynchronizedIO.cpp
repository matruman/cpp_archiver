#include "archiver.h"
#include "SynchronizedIO.h"
using namespace std;

int SynchronizedIO::read(int id, char *buff)
{
    while (id != next_id(last_reader))
    {
        this_thread::sleep_for(chrono::microseconds(100));
    }
    ifs->read(buff, BUFF_SIZE);
    last_reader = id;
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
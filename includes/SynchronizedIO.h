#ifndef SYNCHRONISED_IO_H
# define SYNCHRONISED_IO_H

#include "archiver.h"
using namespace std;

class SynchronizedIO
{
    private:
    ifstream *ifs;
    ofstream *ofs;
    volatile int last_reader;
    volatile int last_writer;
    mutex locker;
    int next_id(int id, int last)
    {
        return id == MAX_THREAD - 1 ? 0 : id + 1;
    }
    
    public:
    SynchronizedIO(ifstream *ifs, ofstream *ofs)
    {
        this->ifs = ifs;
        this->ofs = ofs;
        last_reader = MAX_THREAD;
        last_writer = MAX_THREAD;
    }
    int read(int id, char *buff);
    int write(int id, vector<char> *buffer);
};

#endif
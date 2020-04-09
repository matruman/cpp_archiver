#ifndef PARALLEL_COMPRESSOR_H
# define PARALLEL_COMPRESSOR_H

# include "archiver.h"
# include "Compressor.h"

using namespace std;

class ParallelCompressor
{
    private:
    string *files;
    thread threads[MAX_THREAD];
    Compressor *compressors[MAX_THREAD];
    
    public:
    ParallelCompressor(string *files)
    {
        this->files = files;
    }
    void run();

};

#endif
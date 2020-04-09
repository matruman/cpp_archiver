#include "archiver.h"
#include "ParallelCompressor.h"

void run_thread(Compressor *compressor)
{
    compressor->run();
}

void    ParallelCompressor::run()
{
    ifstream ifs;
    ofstream ofs;
    ifs.open(files[0], ios::binary);
    ifs.seekg(0, std::ifstream::end);
    size_t length = ifs.tellg();
    ifs.seekg(0, std::ifstream::beg);
    int blocks_count = length / BUFF_SIZE;
    if (length % BUFF_SIZE > 0)
        blocks_count++;
    std::string name = files[0];
    ofs.open("archive.compress", ios::binary);
    short len = name.length();
    const char * cname = name.c_str();
    vector<char> buffer;
    putShort(&buffer, len);
    buffer.insert(buffer.end(), cname, cname + len);
    putInt(&buffer, blocks_count);
    ofs.write(buffer.data(), buffer.size());
    SynchronizedIO io(&ifs, &ofs);
    for(int i = 0; i < MAX_THREAD; i++)
    {
        compressors[i] = new Compressor(&io, i);
        threads[i] = thread(run_thread, compressors[i]);
    }
    ifs.close();
    ofs.close();
}
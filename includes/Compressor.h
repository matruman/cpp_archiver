#ifndef COMPRESSOR_H
# define COMPRESSOR_H

# include "archiver.h"

class Compressor
{
private:
    Overlap             overlaps[BUFF_SIZE];
    std::string         *files;
    char                buff[BUFF_SIZE];
    bool                busy[BUFF_SIZE];
    std::vector<char>   buffer;
    int                 lengthIn;
    int                 lengthOut;

public:
    Compressor(std::string *files)
    {
        std::fill(buff, buff + BUFF_SIZE, 0);
        std::fill(busy, busy + BUFF_SIZE, 0);
        std::fill(overlaps, overlaps + BUFF_SIZE, Overlap());
        this->files = files;
    }
    void run();
    void readBlock();
    void getOverlaps();
    int getOverlap(int origin, int pos);
    int getEmpty(int start);
    void setBusy(Overlap overlap);
    int getEmptyLength(int start);
    void setNewPositions();
    int insertEmptyBytes(int start);
    void output(std::ofstream *ofs);
};

#endif
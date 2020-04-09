#ifndef COMPRESSOR_H
# define COMPRESSOR_H

# include "archiver.h"
# include "Overlap.h"
# include "SynchronizedIO.h"


class Compressor
{
private:
    Overlap             overlaps[BUFF_SIZE];
    std::string         *files;
    char                buff[BUFF_SIZE];
    bool                busy[BUFF_SIZE];
    std::vector<char>   buffer;
    int                 id;
    SynchronizedIO      *io;
    int                 lengthIn;
    int                 lengthOut;

public:
    Compressor(SynchronizedIO *io, int id)
    {
        std::fill(buff, buff + BUFF_SIZE, 0);
        std::fill(busy, busy + BUFF_SIZE, 0);
        std::fill(overlaps, overlaps + BUFF_SIZE, Overlap());
        this->io = io;
        this->id = id;
    }
    void run();
    void getOverlaps();
    int getOverlap(int origin, int pos);
    int getEmpty(int start);
    void setBusy(Overlap overlap);
    int getEmptyLength(int start);
    void setNewPositions();
    int insertEmptyBytes(int start);
    void output();
};

#endif
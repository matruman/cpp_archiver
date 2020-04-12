#ifndef OVERLAP_H
# define OVERLAP_H

# include "archiver.h"

class Overlap
{
private:
    int position;
    int length;
    int addr;
    bool origin;
    int end;
    int newPosition;

public:

    Overlap()
    {
        this->position = 0;
        this->length = 0;
        this->addr = 0;
        this->origin = 0;
        newPosition = 0;
        end = 0;
    }
    Overlap(int position, int length, int addr, bool origin) 
    {
        this->position = position;
        this->length = length;
        this->addr = addr;
        this->origin = origin;
        end = position + length;
    }
    int getPosition() {return position;}
    int getLength() {return length;}
    int getAddr() {return addr;}
    bool isOrigin() {return origin;}
    int getEnd(){return end;}
    int getNewPosition() {return newPosition;}
    void setPosition(int position) {this->position = position;}
    void setLength(int length) {
        this->length = length;
        end = position + length;
    }
    void setAddr(int addr) {this->addr = addr;}
    void setOrigin(bool origin) {this->origin = origin;}
    void setNewPosition(int newPosition) {this->newPosition = newPosition;}
};

#endif

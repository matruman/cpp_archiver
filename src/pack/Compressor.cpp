#include "archiver.h"
#include "Compressor.h"
#include "Overlap.h"

void     Compressor::run()
{
    do {
        lengthIn = io->read(id, buff);
        if (lengthIn == 0)
            break;
        getOverlaps();
        setNewPositions();
        output();
        std::fill(buff, buff + BUFF_SIZE, 0);
        std::fill(busy, busy + BUFF_SIZE, 0);
        std::fill(overlaps, overlaps + BUFF_SIZE, Overlap());
        buffer.clear();
    } while (lengthIn == BUFF_SIZE);
}

void  Compressor::output() 
{
    short value;
    putInt(&buffer, lengthOut);

    for(int i = 0; i < lengthIn; i++) {

        if (overlaps[i].isSet()) {
            if (overlaps[i].isOrigin()) {
                buffer.push_back((char)overlaps[i].getLength());
                buffer.insert(buffer.end(), buff + overlaps[i].getPosition(),
                buff + overlaps[i].getPosition() + overlaps[i].getLength());
                i += overlaps[i].getLength() - 1;
            }
            else {
                buffer.push_back(-(char)overlaps[i].getLength());
                value = (short)overlaps[overlaps[i].getAddr()].getNewPosition();
                putShort(&buffer, value);
                i += overlaps[i].getLength() - 1;
            }
        }
        else {
            i += insertEmptyBytes(i) - 1;
        }
    }
    io->write(id, &buffer);
}

void    Compressor::getOverlaps() 
{
    int i = 0;
    int j;
    int length;
    int tmp;
    while (i < lengthIn - 2 * MIN_OVERLAP)
    {
        if (busy[i]) {
            i = getEmpty(i);
            continue;
        }
        j = i + 127;
        length = 0;
        while (j < lengthIn - MIN_OVERLAP) {
            if (busy[j]) {
                j = getEmpty(j);
                continue;
            }
            if ((tmp = getOverlap(i, j)) >= MIN_OVERLAP) {
                if (tmp > length) {
                    if (length != 0)
                        overlaps[i].setLength(tmp);
                    length = tmp;
                }
                if (!overlaps[i].isSet())
                    overlaps[i] = Overlap(i, length, -1, true);
                setBusy(overlaps[j] = Overlap(j, tmp, i, false));
                j += tmp;
                continue;
            }
            j++;
        }
        if (overlaps[i].isSet()) {
            setBusy(overlaps[i]);
            i += overlaps[i].getLength();
        }
        i++;
    }
}

int     Compressor::insertEmptyBytes(int start) {

        int count;
        int tmp;

        count = 0;
        while (start + count < lengthIn && !overlaps[start + count].isSet())
            count++;

        int blocks = count % MAX_BYTE > 0 ? count / MAX_BYTE + 1 : count / MAX_BYTE;
        while (blocks > 1)
        {
            buffer.push_back(MAX_BYTE);
            buffer.insert(buffer.end(), buff + start, buff + start + MAX_BYTE);
            start += MAX_BYTE;
            blocks--;
        }
        if (blocks == 1)
        {
            tmp = count % MAX_BYTE > 0 ? count % MAX_BYTE : MAX_BYTE;
            buffer.push_back((char)tmp);
            buffer.insert(buffer.end(), buff + start, buff + start + tmp);
        }
        return count;
    }

void    Compressor::setNewPositions() 
{
    int shift = 0;
    int tmp;

    for (int i = 0; i < lengthIn; i++) 
    {
        
        if (!overlaps[i].isSet()) {
            tmp = getEmptyLength(i);
            i += tmp - 1;
            shift += tmp % MAX_BYTE > 0 ? tmp / MAX_BYTE + 1 : tmp / MAX_BYTE;
            shift += tmp;
        }
        else if (overlaps[i].isSet() && !overlaps[i].isOrigin()) {
            overlaps[i].setNewPosition(shift + 1);
            shift += 1 + LENGTH_ADDRESS;
            i =  overlaps[i].getEnd() - 1;
        }
        else if (overlaps[i].isSet() && overlaps[i].isOrigin()) {
            overlaps[i].setNewPosition(shift + 1);
            shift += 1 + (overlaps[i].getLength());
            i = overlaps[i].getEnd() - 1;
        }
    }
    lengthOut = shift;
}

int     Compressor::getEmptyLength(int start) 
{
    int i = 0;
    while (i + start < lengthIn && !overlaps[i + start].isSet()) 
    {
        ++i;
    }
    return i;
}

void    Compressor::setBusy(Overlap overlap) {
    for (int i = overlap.getPosition(); i < overlap.getEnd(); ++i)
    busy[i] = true;
}

int     Compressor::getEmpty(int start)
{
    int i = start;
    while (i < lengthIn && busy[i])
        i++;
    return i;
}

int     Compressor::getOverlap(int origin, int pos)
{
    int i = 0;
    while (i < 127 && i + pos < lengthIn && !busy[origin + i]
    && !busy[pos + i] && buff[origin + i] == buff[pos + i])
        i++;
    return i;
}

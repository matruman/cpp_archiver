#include "archiver.h"

void    rev_endian(void *val, int size)      
{
    char    *p;   
    char    tmp;  
    int  i;

    p = (char *)val;
    i = 0;
    while (i < size / 2)
    {
        tmp = p[size - i - 1];
        p[size - i - 1] = p[i];
        p[i] = tmp;
        i++;
    }
}

void putShort(std::vector<char> *buffer, short val)
{
    rev_endian(&val, 2);
    buffer->insert(buffer->end(), (char *)(&val), (char *)(&val) + 2);
}

void putInt(std::vector<char> *buffer, int val)
{
    rev_endian(&val, 4);
    buffer->insert(buffer->end(), (char *)(&val), (char *)(&val) + 4);
}

void     Compressor::run()
{
    std::ifstream ifs;
    std::ofstream ofs;
    ifs.open(files[0]);
    ifs.seekg(0, std::ifstream::end);
    size_t length = ifs.tellg();
    ifs.seekg(0, std::ifstream::beg);
    int blocks_count = length / BUFF_SIZE;
    if (length % BUFF_SIZE > 0)
        blocks_count++;
    std::string name = files[0];
    ofs.open("archive.compress", std::ios::binary);
    short len = name.length();
    const char * cname = name.c_str();
    putShort(&buffer, len);
    buffer.insert(buffer.end(), cname, cname + len);
    putInt(&buffer, blocks_count);
    for(int i = 0; i < buffer.size(); i++)
    {
        printf("%hhx ", buffer[i]);
    }
    printf("\n");
    do {
        ifs.read(buff, BUFF_SIZE);
        lengthIn = ifs.gcount();
        getOverlaps();
        setNewPositions();
        for (int i =0; i < lengthIn; i++)
        {
            if (overlaps[i].isSet())
                printf("pos: %d | end: %d | length: %d | addr: %hd\n", overlaps[i].getNewPosition(),
                overlaps[i].getEnd(), overlaps[i].getLength(), overlaps[i].getAddr());
        }
        output(&ofs);
        std::fill(buff, buff + BUFF_SIZE, 0);
        std::fill(busy, busy + BUFF_SIZE, 0);
        std::fill(overlaps, overlaps + BUFF_SIZE, Overlap(0,0,0,0));
        buffer.clear();
    } while (ifs.gcount() == BUFF_SIZE);
    ifs.close();
    ofs.close();
}

void  Compressor::output(std::ofstream *ofs) 
{
    int s1, s2;
    short value;
    putInt(&buffer, lengthOut);
    printf("lout: %d\n", lengthOut);

    for(int i = 0; i < lengthIn; i++) {

        if (overlaps[i].isSet()) {
            if (overlaps[i].isOrigin()) {
                buffer.push_back((char)overlaps[i].getLength());
                printf("LENGTH DIR: %d | written: ", overlaps[i].getLength());
                s1 = buffer.size();
                buffer.insert(buffer.end(), buff + overlaps[i].getPosition(),
                buff + overlaps[i].getPosition() + overlaps[i].getLength());
                s2 = buffer.size();
                printf("%d\n", s2 -s1);
                i += overlaps[i].getLength() - 1;
            }
            else {
                buffer.push_back(-(char)overlaps[i].getLength());
                printf("LENGTH IND: %d\n", overlaps[i].getLength());
                value = (short)overlaps[overlaps[i].getAddr()].getNewPosition();
                putShort(&buffer, value);
                i += overlaps[i].getLength() - 1;
            }
        }
        else {
            i += insertEmptyBytes(i) - 1;
        }
    }
    ofs->write(buffer.data(), buffer.size());
}

void    Compressor::getOverlaps() 
{
    int i = 0;
    while (i < lengthIn - 2 * MIN_OVERLAP)
    {
        if (busy[i]) {
            i = getEmpty(i);
            continue;
        }
        int j = i + 127;
        int length = 0;
        int tmp;
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

        int s1, s2;

        s1 = buffer.size();

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
        s2 = buffer.size();
        printf("EMPTY: %d | written %d\n", count, s2 -s1);
                // if (start > 5)
                // {
                //     for(int k = 26; k > 0; k--)
                //     printf("%hhx ", buffer.data()[buffer.size() - k]);
                //     exit(0);
                // }
        return count;
    }

void    Compressor::setNewPositions() {

        int shift = 0;
        int tmp;

        for (int i = 0; i < lengthIn; i++) {
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
    while (i < 127 && i + origin < lengthIn && i + pos < lengthIn && !busy[origin + i]
    && !busy[pos + i] && buff[origin + i] == buff[pos + i])
        i++;
    return i;
}

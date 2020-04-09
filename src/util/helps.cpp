#include "archiver.h"
#include "Compressor.h"
#include "SynchronizedIO.h"

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

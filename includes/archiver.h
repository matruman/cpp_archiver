#ifndef ARCHIVER_H
# define ARCHIVER_H

# define BUFF_SIZE 4096
# define MAX_THREAD 4
# define MAX_BYTE 127
# define LENGTH_ADDRESS 2
# define MIN_OVERLAP 5

# include <mutex>
# include <thread>
# include <chrono>
# include <ctime>
# include <cassert>
# include <stdio.h>
# include <iostream>
# include <string>
# include <cstring>
# include <vector>
# include <fstream>
# include <chrono>

void    putShort(std::vector<char> *buffer, short val);
void    putInt(std::vector<char> *buffer, int val);
void    rev_endian(void *val, int size);

#endif
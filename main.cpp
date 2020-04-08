#include "archiver.h"

int main()
{
    std::string files[1]{"Makefile"}; 
    Compressor compressor(files);
    compressor.run();
    return (0);
}

/*
std::ifstream ifs;
    ifs.open("Makefile");
    ifs.seekg(0, std::ifstream::end);
    size_t length = ifs.tellg();
    ifs.seekg(0, std::ifstream::beg);
    char buff[128];
    std::vector<char> buffer;
    do {
        ifs.read(buff, 128);
        buffer.insert(buffer.end(), buff, buff + ifs.gcount());
        std::cout << ifs.gcount() << "\n";
    } while (ifs.gcount() == 128);
    buffer.push_back(0);
    char *str = buffer.data();
    std::string cstr = std::string(str);

    std::cout << str;

    std::cout << "Size: " << buffer.size() << "\n";
    std::cout << "Length: " << length << "\n";

    return (0);
    */
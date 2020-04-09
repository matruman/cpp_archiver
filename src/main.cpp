#include "archiver.h"
#include "ParallelCompressor.h"
using namespace std;

int main()
{
    std::string files[1]{"IMG.jpg"};
    int t1, t2;
    t1 = clock();
    ParallelCompressor compressor(files);
    compressor.run();
    t2 = clock();
    printf("Time: %d\n", t2 - t1);
    return (0);
}

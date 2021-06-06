
#include "stream/file_stream.h"
#include "stream/memory_stream.h"
#include "test_case_factory.h"

BEG_NSP_DDM

TEST(test_stream, memory_stream)
{
    memory_stream* pDogStream = new memory_stream(100);
    s8 buff[128] = { 'a' };
    buff[127] = 0;
    pDogStream->write((u8*)buff, 128);
    memory_stream stream1 = (*pDogStream);
    stream1.seek(1, 0);
    stream1.write((u8*)buff, 127);
    stream1.resize(200);
    stream1.resize(1025);
    stream1.resize(0x40000000);
    delete pDogStream;
}

TEST(test_stream, FileStream)
{
    i_ddstream* pDogStream = new file_stream(_DDT("filestream.txt"));
    char buff[] = "hello file stream";
    pDogStream->write((u8*)buff, sizeof(buff));
    pDogStream->write((u8*)buff, sizeof(buff));
    s64 size = pDogStream->size(); size;
    char buff1[500] = { 0 };
    pDogStream->readalla((u8*)buff1);
    delete pDogStream;
}
END_NSP_DDM

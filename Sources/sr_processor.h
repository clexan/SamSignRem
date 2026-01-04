#ifndef SR_PROCESSOR_H
#define SR_PROCESSOR_H

#include "backend.h"
#include <QFile>
#include <QByteArray>

class SrProcessor
{
public:
    SrProcessor();

    void processFile(string base_name);
    void fillPositions(const QByteArray &pattern);
    void replaceBytes(const QByteArray &replacement);
    void printAscii(int index, int len);
    void printHex(int index, int len);
    void rwUntilPosition(int64_t position);
    void findPattInBlock(const QByteArray &block, const QByteArray &pattern);

    QFile img_file;
    QFile raw_file;
    QByteArray f_buffer;
    int64_t  curr_pos;
    vector<int64_t> positions;
};

#endif // SR_PROCESSOR_H

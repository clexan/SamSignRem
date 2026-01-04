#include "sr_processor.h"

SrProcessor::SrProcessor()
{

}

void SrProcessor::processFile(string base_name)
{
    sr_mkdir("patched");

    QString full_path = QString::fromStdString(sr_getCurrentPath()) + "/lz4/" + QString::fromStdString(base_name);
    QString raw_path = QString::fromStdString(sr_getCurrentPath()) + "/patched/" + QString::fromStdString(base_name);
   
    img_file.setFileName(full_path);
    raw_file.setFileName(raw_path);
   
    if(!img_file.open(QIODevice::ReadOnly))
    {
        cout << "Error: cannot open file: " << full_path.toStdString() << endl;
        return;
    }

    if (!raw_file.open(QIODevice::WriteOnly))
    {
        cout << "Error: cannot create file: " << raw_path.toStdString() << endl;
        img_file.close();
        return;
    }

    cout << "Processing " << base_name << endl;

    QByteArray pattern = "SignerVer02";
    pattern.append(3, '\0');

    const char replaceData[] = {
           0x53, 0x69, 0x67, 0x6E, 0x65, 0x72, 0x56, 0x65, 0x72,
           0x30, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x38,
           0x37, 0x32, 0x38, 0x39, 0x37, 0x34, 0x52, 0x00, 0x00,
           0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x33, 0x34, 0x36,
           0x45, 0x58, 0x58, 0x55, 0x34, 0x41, 0x57, 0x47, 0x38,
           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
           0x00, 0x32, 0x30, 0x32, 0x33, 0x30, 0x37, 0x33, 0x31,
           0x31, 0x34, 0x34, 0x37, 0x30, 0x38, 0x00, 0x00, 0x53,
           0x4D, 0x2D, 0x41, 0x33, 0x34, 0x36, 0x45, 0x5F, 0x53,
           0x45, 0x41, 0x5F, 0x50, 0x48, 0x4C, 0x5F, 0x4D, 0x4B,
           0x45, 0x59, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
           0x00, 0x00, 0x00, 0x00, 0x53, 0x52, 0x50, 0x56, 0x4C,
           0x30, 0x32, 0x41, 0x30, 0x30, 0x34, 0x00, 0x00, 0x00,
           0x00, 0x00, 0x53, 0x52, 0x50, 0x56, 0x4C, 0x30, 0x32,
           0x41, 0x30, 0x30, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00
        };

        QByteArray rep = QByteArray::fromRawData(replaceData, sizeof(replaceData));

    fillPositions(pattern);
    replaceBytes(rep);

    raw_file.close();
    img_file.close();
}

void SrProcessor::replaceBytes(const QByteArray &replacement)
{
    img_file.seek(0);
    curr_pos = 0;
    int pos_len = positions.size();

    for( int i=0 ; i<pos_len ; i++ )
    {
        rwUntilPosition(positions[i]);
        printAscii(i, replacement.length());
        printHex(i, replacement.length());

        raw_file.write(replacement);
        curr_pos += replacement.length();

        img_file.seek(curr_pos);
    }

    while(true)
    {
        QByteArray buffer = img_file.read(SR_BLOCK_SIZE);
        if (buffer.isEmpty()) break;

        raw_file.write(buffer);
    }
}

void SrProcessor::fillPositions(const QByteArray &pattern)
{
    positions.clear();
    int counter = 0;
    curr_pos = 0;
// removed the manual malloc from f_buffer - use local qbytearray
    while(true)
    {
        QByteArray block = img_file.read(SR_BLOCK_SIZE + SR_BUFFER_MARGIN);

        if (block.isEmpty()) break;
        findPattInBlock(block, pattern);

        if (block.size() < SR_BLOCK_SIZE)
        {
            break;
        }

        counter++;
        curr_pos = counter * SR_BLOCK_SIZE - SR_BUFFER_MARGIN;

        if (curr_pos < 0) curr_pos = 0;

        img_file.seek(curr_pos); // use seek() bc _fseeki64 breaks on linux 
    }
}

void SrProcessor::findPattInBlock(const QByteArray &block, const QByteArray &pattern)
{
    int start_pos = 0;
    while(true)
    {
        int index = block.indexOf(pattern, start_pos);
        if( index == -1 )
        {
            break;
        }

        start_pos = index + 1;
        int64_t absoluteIndex = index + curr_pos;

        if (std::find(positions.begin(), positions.end(), absoluteIndex) == positions.end())
    {
        cout << "found " << absoluteIndex << " " << curr_pos << "\n";
        positions.push_back(absoluteIndex); 
    }
    }
}

void SrProcessor::printAscii(int index, int len)
{
    int64_t original_pos = img_file.pos();
    
    if (img_file.seek(positions[index])) {
        QByteArray buffer = img_file.read(len);
        
        cout << "[" << index << "/" <<  positions[index] << "] Str: ";
        for( int i=0 ; i<buffer.size() ; i++ )
        {
            unsigned char c = buffer[i];
            if( c > 31 && c < 127 )
                printf("%c", c);
            else
                printf(".");
        }
        cout << endl;
    }
    
    img_file.seek(original_pos);
}

void SrProcessor::printHex(int index, int len)
{
    int64_t original_pos = img_file.pos();

    if (img_file.seek(positions[index])) {
        QByteArray buffer = img_file.read(len);

        cout << "[" << index << "] Hex: ";
        int printLen = std::min((int)buffer.size(), 20);
        
        for( int i=0 ; i<printLen ; i++ )
        {
            printf("0x%x ", (unsigned char)buffer[i]);
        }
        cout << endl;
    }

    img_file.seek(original_pos);
}

// read from file and write to the output till the input position
void SrProcessor::rwUntilPosition(int64_t position)
{
    while( curr_pos < position )
    {
        int64_t to_read = SR_BLOCK_SIZE;
        if( curr_pos + SR_BLOCK_SIZE > position )
        {
            to_read = position - curr_pos;
        }

        QByteArray buffer = img_file.read(to_read);
        if (buffer.isEmpty()) break;
        
        raw_file.write(buffer);
        curr_pos += buffer.size();
    }
}

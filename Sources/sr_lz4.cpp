void sr_lz4Decode()
{
    sr_mkdir("lz4");

    vector<string> lz4s = sr_findlz4s();
    int lz4_len = lz4s.size();
    
    cout << "Decoding " << lz4_len << " lz4 files..." << endl;

    for( int i=0 ; i<lz4_len ; i++ )
    {
        QString inputFile = QString::fromStdString(lz4s[i]);
        // Remove .lz4 extension for the output filename
        QString outputFile = "lz4/" + inputFile.left(inputFile.length() - 4);
        
        // Use QProcess instead of system()
        QString program = "lz4";
        QStringList arguments;
        // -d: decompression, -f: overwrite output without prompting
        arguments << "-d" << "-f" << inputFile << outputFile;

        QProcess process;
        process.start(program, arguments);
        process.waitForFinished();
        
        if (process.exitCode() != 0) {
            cout << "Failed to decode: " << inputFile.toStdString() << endl;
        }
    }
}

void sr_lz4Code()
{
    sr_mkdir("tar");

    vector<string> imgs = sr_findImgs("patched");
    int imgs_len = imgs.size();

    cout << "Encoding " << imgs_len << " patched files..." << endl;

    for( int i=0 ; i<imgs_len ; i++ )
    {
        QString inputFile = "patched/" + QString::fromStdString(imgs[i]);
        QString outputFile = "tar/" + QString::fromStdString(imgs[i]) + ".lz4";

        QString program = "lz4";
        QStringList arguments;
        // -f: overwrite output
        arguments << "-f" << inputFile << outputFile;

        QProcess process;
        process.start(program, arguments);
        process.waitForFinished();
        
        if (process.exitCode() != 0) {
             cout << "Failed to encode: " << inputFile.toStdString() << endl;
        }
    }
}

vector<string> sr_findlz4s()
{
    vector<string> ret;
    QDir dir = QDir::current();

    // Set filters for .lz4 files
    QStringList filters;
    filters << "*.lz4";
    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);

    QFileInfoList list = dir.entryInfoList();
    for (const QFileInfo &fileInfo : list) {
        ret.push_back(fileInfo.fileName().toStdString());
    }

    return ret;
}
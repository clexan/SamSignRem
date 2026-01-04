#include "backend.h"

// List all .img in current dir except recovery.img
vector<string> sr_findImgs(string dirname)
{
    vector<string> ret;
    
    QDir dir = QDir::current();
    
    if (!dir.cd(QString::fromStdString(dirname))) {
        cout << "Directory not found: " << dirname << endl;
        return ret;
    }

    QStringList filters;
    filters << "*.img" << "*.bin";
    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);

    QFileInfoList list = dir.entryInfoList();
    for (const QFileInfo &fileInfo : list) {
        string filename = fileInfo.fileName().toStdString();
        
        if (isValidImage(filename)) {
            ret.push_back(filename);
        }
    }

    return ret;
}

string sr_getCurrentPath()
{
    return QDir::currentPath().toStdString();
}

int isValidImage(string name)
{
    QString qName = QString::fromStdString(name);
    
    if (qName == "recovery.img" || qName == "recovery.bin") {
        return 0;
    }

    if (qName.endsWith(".img") || qName.endsWith(".bin")) {
        return 1;
    }

    return 0;
}

void sr_mkdir(string path)
{
    QDir dir;
    if (!dir.mkpath(QString::fromStdString(path))) {
        cout << "Failed to create directory: " << path << endl;
    }
}

string sc_getLastDirectoryName(string address)
{
    QDir dir(QString::fromStdString(address));
    return dir.dirName().toStdString();
}
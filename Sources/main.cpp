#include "sr_processor.h"
#include "sr_lz4.h"
#include "sr_tar.h"
#include <QCoreApplication>

void processLz4Stream(QString outputName)
{
    sr_lz4Decode();

    vector<string> imgs = sr_findImgs("lz4");
    int len = imgs.size();

    SrProcessor processor;
    for( int i=0 ; i<len ; i++ )
    {
        processor.processFile(imgs[i]);
    }

    sr_lz4Code();

    sr_tar(outputName);
}

int main(int argc, char *argv[])
{
    cout << "Samsung Sign Remover V0.5.2\n";

    QDir currentDir = QDir::current();
    QStringList filters;
    // Look for standard Samsung archive patterns
    filters << "AP_*.tar.md5" << "AP_*.tar" << "BL_*.tar.md5" << "BL_*.tar";
    currentDir.setNameFilters(filters);
    QFileInfoList archiveList = currentDir.entryInfoList();

    if (archiveList.isEmpty()) {
        // --- LEGACY MODE ---
        cout << "No AP/BL archives found. Running in Legacy Mode (current folder)..." << endl;
        
        // Emulate old behavior: derive name from current folder
        string currentPath = sr_getCurrentPath();
        string dirName = sc_getLastDirectoryName(currentPath);
        QString outputName = QString::fromStdString(dirName) + ".tar";
        
        processLz4Stream(outputName);
    } 
    else {
        // --- AUTOMATED ---
        cout << "Found " << archiveList.size() << " archive(s). Processing..." << endl;
        
        for (const QFileInfo &fi : archiveList) {
            QString originalPath = currentDir.absolutePath();
            QString archiveName = fi.fileName();
            
            // Generate clean output name
            QString cleanName = archiveName;
            if (cleanName.endsWith(".md5")) cleanName.chop(4);
            if (cleanName.endsWith(".tar")) cleanName.chop(4);
            
            QString outputName = cleanName + "_Patched.tar";
            QString tempDirName = "temp_" + cleanName;
            
            cout << "\n>>> Processing: " << archiveName.toStdString() << endl;

            // Extract to temp dir
            if (!sr_untar(fi.absoluteFilePath(), tempDirName)) {
                continue;
            }
            
            if (!QDir::setCurrent(tempDirName)) {
                cout << "Error accessing temp dir: " << tempDirName.toStdString() << endl;
                continue;
            }
            
            processLz4Stream(outputName);
            
            // Move files back to root
            QString resultPath = QDir::current().absoluteFilePath(outputName);
            QString targetPath = originalPath + "/" + outputName;
            
            if (QFile::exists(targetPath)) {
                QFile::remove(targetPath);
            }
            
            if (QFile::rename(resultPath, targetPath)) {
                cout << "Saved: " << targetPath.toStdString() << endl;
            } else {
                cout << "Failed to move output file to root." << endl;
            }
            
            // Cleanup crew
            QDir::setCurrent(originalPath); // Step out
            QDir tempDir(tempDirName);
            if (tempDir.removeRecursively()) {
                cout << "Cleaned up temp directory." << endl;
            } else {
                cout << "Warning: Failed to delete temp directory." << endl;
            }
        }
    }

    cout << "\nAll operations complete." << endl;
    return 0;
}
#include "sr_tar.h"

void sr_tar()
{
    // Determine output tar filename based on the current dir name
    string currentPath = sr_getCurrentPath();
    string dirName = sc_getLastDirectoryName(currentPath);
    QString tarName = QString::fromStdString(dirName) + ".tar";
    
    cout << "Creating archive: " << tarName.toStdString() << endl;

    // To run tar inside the 'tar' subdir so the files 
    // are added without the 'tar/' prefix in the archive path
    QDir tarDir("tar");
    if (!tarDir.exists()) {
        cout << "Error: 'tar' directory does not exist." << endl;
        return;
    }

    // tar -cf <archive_name> *.lz4
    QString program = "tar";
    QStringList arguments;
    arguments << "-cf" << tarName << "*.lz4"; // Wildcard might not expand in QProcess arguments depending on implementation

    // QProcess doesn't expand wildcards like *.lz4 automatically
    // We want to list files manually which
    // is safer and better for cross platform     
    QStringList files;
    QStringList filters;
    filters << "*.lz4";
    tarDir.setNameFilters(filters);
    QFileInfoList fileList = tarDir.entryInfoList();
    
    for (const QFileInfo &fi : fileList) {
        files << fi.fileName();
    }
    
    if (files.isEmpty()) {
        cout << "No .lz4 files found to archive." << endl;
        return;
    }
    
    // reset args to: -cf ../ArchiveName.tar file1.lz4 file2.lz4 ...
    arguments.clear();
    arguments << "-cf" << "../" + tarName;
    arguments.append(files);

    QProcess process;
    process.setWorkingDirectory(tarDir.absolutePath());
    process.start(program, arguments);
    process.waitForFinished();

    if (process.exitCode() == 0) {
        cout << "Successfully created " << tarName.toStdString() << endl;
    } else {
        cout << "Error creating tar archive. Exit code: " << process.exitCode() << endl;
        cout << process.readAllStandardError().toStdString() << endl;
    }
}
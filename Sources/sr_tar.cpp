#include "sr_tar.h"

bool sr_untar(QString archive, QString dest)
{
    QDir dir;
    if (!dir.exists(dest)) {
        dir.mkpath(dest);
    }

    cout << "Extracting " << archive.toStdString() << " to " << dest.toStdString() << "..." << endl;

    QProcess process;
    QString program = "tar";
    QStringList arguments;
    
    // -xf <archive> -C <dest>
    arguments << "-xf" << archive << "-C" << dest;

    process.start(program, arguments);
    process.waitForFinished();

    if (process.exitCode() == 0) {
        cout << "Extraction successful." << endl;
        return true;
    } else {
        cout << "Error extracting archive. Exit code: " << process.exitCode() << endl;
        cout << process.readAllStandardError().toStdString() << endl;
        return false;
    }
}

void sr_tar(QString outputFilename)
{
    cout << "Creating archive: " << outputFilename.toStdString() << endl;

    QDir tarDir("tar");
    if (!tarDir.exists()) {
        cout << "Error: 'tar' directory does not exist." << endl;
        return;
    }

    QString program = "tar";
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
    
    QStringList arguments;
    // FIX: Force 'ustar' format for Odin compatibility
    arguments << "--format=ustar"; 
    arguments << "-cf" << "../" + outputFilename;
    arguments.append(files);

    QProcess process;
    process.setWorkingDirectory(tarDir.absolutePath());
    process.start(program, arguments);
    process.waitForFinished();

    if (process.exitCode() == 0) {
        cout << "Successfully created " << outputFilename.toStdString() << endl;
    } else {
        cout << "Error creating tar archive. Exit code: " << process.exitCode() << endl;
        cout << process.readAllStandardError().toStdString() << endl;
    }
}
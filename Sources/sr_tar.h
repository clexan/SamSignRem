#ifndef SR_TAR_H
#define SR_TAR_H

#include "backend.h"
#include <QProcess>

void sr_tar(QString outputFilename);

bool sr_untar(QString archive, QString dest);

#endif // SR_TAR_H

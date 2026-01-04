#ifndef BACKEND_H
#define BACKEND_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <QDir>
#include <QString>
#include <QFileInfo>

#define SR_BUFFER_MARGIN 100
#define SR_BLOCK_SIZE    1e6
// Calc from -> "SignerVer02.....65165289R.......A346EXXU2AWE2...";
#define SR_REPLACE_SIZE  48

using namespace std;

string sr_getCurrentPath();
string sc_getLastDirectoryName(string address);
vector<string> sr_findImgs(string dirname);
void sr_mkdir(string path);
int  isValidImage(string name);

#endif // BACKEND_H

# SamSignRem

A cross-platform tool (Linux & Windows) that removes `SignerVer02` signatures from Samsung firmware. This fixes `SW REV. CHECK FAIL` errors when downgrading firmware

### Requirements
* **Build Tools:** Qt 5 (`qtbase-devel`), C++ Compiler (`g++`), Make.
* **Runtime Tools:** `lz4` and `tar` (must be installed and in your system PATH)

## Build
```bash
qmake-qt5 SignRemover.pro  # Use 'qmake' on Windows
make
```
Usage
Patch AP:

Unpack your device firmware's AP file (.tar.md5) into a clean folder.

Place the SignRemover binary in that folder and run it.

The tool automatically decompresses, patches, and repacks the files into a new .tar archive.

Rename the output file to AP_Patched.tar and clean the folder from the leftovers.

Patch BL (Bootloader):

Clear the folder (or use a new one).

Unpack the BL file (.tar.md5) and run SignRemover again.

Rename this output file to BL_Patched.tar.

Flash:

Load the patched AP and BL (CP / and CSC) files into their respective slots in Odin (Windows) or Heimdall (Linux) and flash normally.

Disclaimer
Use at your own risk. Modifying firmware partitions may void your warranty or brick your device. Always backup your data.

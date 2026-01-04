# SamSignRem

A cross-platform tool (Linux & Windows) that removes `SignerVer02` signatures from Samsung firmware. This fixes `SW REV. CHECK FAIL` errors when downgrading firmware

### Requirements
* **Build Tools:** Qt 5 (`qtbase-devel`), C++ Compiler (`g++`), Make.
* **Runtime Tools:** `lz4` and `tar` (must be installed and in your system PATH)

## Build
```bash
# Make a build directory
mkdir build && cd build

# Generate the Makefile
qmake-qt5 ../SignRemover.pro  # or 'qmake' on Windows

# Build
make -j$(nproc)
```
Usage
1. Automated Use
The tool should now automatically detect, extract, patch, and repack your firmware archives

Place your unzipped firmware archives (AP_*.tar.md5 and BL_*.tar.md5 (along with CSC/CP)) in the same folder as the SignRemover executable

Run the tool:

```bash
./SignRemover
```

The tool will automatically:

Detect the archives.

Extract them to temporary working directories.

Patch the files to remove signatures.

Repack them as AP_..._Patched.tar and BL_..._Patched.tar.

Clean up temporary files.

2. Legacy Mode (Manual)
If the automatic mode fails or you prefer to do it manually, you can use the standard legacy method:

Unpack AP: Extract the contents of your AP_*.tar.md5 file (the .lz4 files) into a clean folder.

Run Tool: Place the SignRemover binary in that folder and run it.

It will decompress, patch, and repack the files into a .tar archive named after the folder.

Rename: Rename the output file to AP_Patched.tar and delete the leftover extracted files.

Repeat for BL: Clear the folder, extract the BL_*.tar.md5 contents, and run the tool again.

Flashing
Load the generated *_Patched.tar files (AP and BL) along with your original CP and CSC files into their respective slots in Odin (Windows) or Heimdall (Linux) and flash normally.

Disclaimer
Use at your own risk. Modifying firmware partitions may void your warranty or brick your device. Always backup your data.a.

#include <vector>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <ctime>
#include <sstream>

#ifndef FAT12_H_
#define FAT12_H_

using namespace std;

enum class Permission {
    READ = 1,
    WRITE = 2,
    READ_WRITE = READ | WRITE
};

typedef struct
{
    /* data */
}BootBlock;

// Contains Meta Data about File System
typedef struct
{
    size_t RootPosition;
    size_t FileCount;
    size_t DirectoryCount;
    int MagicNumber;
    size_t Blocksize;
    size_t BlockCount;
    size_t BlockCountFree;
}SuperBlock;

// Contains A single block in the File System
typedef struct
{
    size_t BlockNumber;
    ssize_t NextBlockPointer;
    vector<char> Data;
}Block;

// Represents a single file in File System
typedef struct
{
    string Name;
    size_t Size;
    string CreationTime;
    string CreationDate;
    string ModificationTime;
    string ModificationDate;
    string Password;
    Permission Permission;
}File;

struct DirectoryEntry
{
    string Name;
    vector<File> Files;
    string CreationTime;
    string CreationDate;
    string ModificationTime;
    string ModificationDate;
};

// Represents a single directory in File System
struct Directory
{
    size_t EntryCount;
    string DirectoryPath;
    vector<DirectoryEntry> SubDirectories;
};

// File System itself
typedef struct 
{
    BootBlock BootBlock;
    SuperBlock SuperBlock;
    vector<Block> Blocks;
    Directory rootDirectory;
}FileSystem;


class FileSystemCreator {
public:
    FileSystemCreator(size_t blockSizeKB, const std::string& fileName)
        : blockSize(blockSizeKB * 1024), fileSystemFile(fileName, std::ios::binary | std::ios::out) {
        if (!fileSystemFile) {
            throw std::runtime_error("Could not create file system file.");
        }
        initializeFileSystem();
    }

    void createFileSystem() {
        writeBootBlock();
        writeSuperBlock();
        writeRootDirectory();
        writeEmptyBlocks();
    }

private:
    size_t blockSize;
    const size_t totalSize = 4 * 1024 * 1024; // 4 MB
    std::ofstream fileSystemFile;
    FileSystem fileSystem;

    void initializeFileSystem() {
        size_t blockCount = totalSize / blockSize;
        fileSystem.SuperBlock = {
            .RootPosition = 0,
            .FileCount = 0,
            .DirectoryCount = 1,
            .MagicNumber = 0x12345678, // Example magic number
            .Blocksize = blockSize,
            .BlockCount = blockCount,
            .BlockCountFree = blockCount - 1
        };

        fileSystem.Blocks.resize(blockCount);
        for (size_t i = 0; i < blockCount; ++i) {
            fileSystem.Blocks[i].BlockNumber = i;
            fileSystem.Blocks[i].NextBlockPointer = -1; // No next block initially
            fileSystem.Blocks[i].Data.resize(blockSize, 0); // Initialize block data with zeros
        }

        fileSystem.rootDirectory = {
            .EntryCount = 0,
            .DirectoryPath = "/",
        };
    }

    void writeBootBlock() {
        fileSystemFile.write(reinterpret_cast<char*>(&fileSystem.BootBlock), sizeof(BootBlock));
    }

    void writeSuperBlock() {
        fileSystemFile.write(reinterpret_cast<char*>(&fileSystem.SuperBlock), sizeof(SuperBlock));
    }

    void writeRootDirectory() {
        fileSystemFile.write(reinterpret_cast<char*>(&fileSystem.rootDirectory), sizeof(Directory));
        for (const auto& entry : fileSystem.rootDirectory.SubDirectories) {
            fileSystemFile.write(reinterpret_cast<const char*>(&entry), sizeof(DirectoryEntry));
        }
    }

    void writeEmptyBlocks() {
        for (const auto& block : fileSystem.Blocks) {
            fileSystemFile.write(reinterpret_cast<const char*>(&block.BlockNumber), sizeof(block.BlockNumber));
            fileSystemFile.write(reinterpret_cast<const char*>(&block.NextBlockPointer), sizeof(block.NextBlockPointer));
            fileSystemFile.write(block.Data.data(), block.Data.size());
        }
    }

    string currentTime() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        stringstream ss;
        ss << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec;
        return ss.str();
    }

    string currentDate() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        stringstream ss;
        ss << 1900 + ltm->tm_year << "-" << 1 + ltm->tm_mon << "-" << ltm->tm_mday;
        return ss.str();
    }
};

class Fat12Table
{
    public:

    //Constructor
    Fat12Table();
    
    //Destructor
    ~Fat12Table();

    private:

};


#endif // FAT12_H_
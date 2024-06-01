#include "../include/fat12.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <ctime>
using namespace std;

int main(int argc, char* argv[]) {

    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <block size in KB> <file name>" << endl;
        return 1;
    }

    size_t blockSizeKB = stoi(argv[1]);
    string fileName = argv[2];

    try {
        FileSystemCreator fsCreator(blockSizeKB, fileName);
        fsCreator.createFileSystem();
        cout << "File system created successfully in " << fileName << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
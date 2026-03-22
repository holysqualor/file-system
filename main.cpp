#include <iostream>

#include "Disk.h"

using namespace std;

int main() {
    FSObject *dir = new Directory();
    cout << dir->getType() << endl;
    cout << (dir->isDirectory() ? "Directory" : "File") << endl;
    delete dir;

    File file("file");
    FSObject &obj = file;
    cout << (obj.isDirectory() ? "Directory" : "File") << endl;
    return 0;
}

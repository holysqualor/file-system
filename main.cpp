#include <iostream>

#include "Console.h"

using namespace std;

int main() {
    /*FSObject *dir = new Directory();
    cout << dir->getType() << endl;
    cout << (dir->isDirectory() ? "Directory" : "File") << endl;
    delete dir;

    File file("file");
    FSObject &obj = file;
    cout << (obj.isDirectory() ? "Directory" : "File") << endl;*/

    while(Console::run()) {}
    return 0;
}

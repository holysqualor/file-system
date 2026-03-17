#include <iostream>

#include "Disk.h"

using namespace std;

int main() {
    Disk disk;
    disk.touch("file.txt");
    disk.ls();
    disk.echo("file.txt", "hello");
    disk.cat("file.txt");
    return 0;
}

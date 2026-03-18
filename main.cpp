#include <iostream>

#include "Disk.h"

using namespace std;

int main() {
    Disk A;
    A.touch("file.txt");
    Disk B = A;
    A.rm("file.txt");
    cout << "A" << endl;
    A.ls();
    cout << "B" << endl;
    B.ls();
    return 0;
}

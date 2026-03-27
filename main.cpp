#include <iostream>

#include "Console.h"

using namespace std;

int main() {
    try {
        Disk disk("disk.vdi");
        while(Console::process(disk)) {}
    } catch(const char *err) {
        std::cerr << "fatal error: " << err << std::endl;
        return 1;
    }
    return 0;
}

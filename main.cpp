#include <iostream>

#include "FSObject.h"
#include "SpaceManager.h"
#include "Journal.h"

using namespace std;

int main() {
    FSObject file("file.txt", false);
    SpaceManager sm(512);
    Journal journal;
    return 0;
}

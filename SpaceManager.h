#ifndef SPACEMANAGER_H_INCLUDED
#define SPACEMANAGER_H_INCLUDED

#include <cstddef>

class SpaceManager {
private:
    size_t diskSize, freeSpace;

public:
    SpaceManager(size_t size = 4096);
    size_t getFreeSpace();
    bool alloc(size_t size);
    bool release(size_t size);
};

#endif // SPACEMANAGER_H_INCLUDED

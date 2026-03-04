#include "SpaceManager.h"

SpaceManager::SpaceManager(size_t size) : diskSize{size}, freeSpace{size} {}

size_t SpaceManager::getFreeSpace() {
    return freeSpace;
}

bool SpaceManager::alloc(size_t size) {
    if(freeSpace < size)
        return false;
    freeSpace -= size;
    return true;
}

bool SpaceManager::release(size_t size) {
    if(freeSpace + size > diskSize)
        return false;
    freeSpace += size;
    return true;
}

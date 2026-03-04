#ifndef FSOBJECT_H_INCLUDED
#define FSOBJECT_H_INCLUDED

#include <iostream>
#include <string>
#include <cstdint>

class FSObject {
private:
    std::string name;
    uint8_t attributes;

public:
    FSObject(const std::string& name, uint8_t attributes);
    FSObject(const std::string& name, bool type);
    const std::string& getName() const;
    bool isDir();
    bool isHidden();
    bool canRead();
    bool canWrite();
    bool canExecute();
    void chmod(bool r, bool w, bool x);
    void rename(const std::string& name);
};

#endif // FSOBJECT_H_INCLUDED

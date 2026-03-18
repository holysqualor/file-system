#ifndef FSOBJECT_H_INCLUDED
#define FSOBJECT_H_INCLUDED

#include <iostream>
#include <string>
#include <cstdint>

class FSObject {
private:
    std::string name;
    uint8_t attributes;

protected:
    FSObject(const std::string& name, uint8_t type);

public:
    static const uint8_t
        DIRECTORY = 0b10000000,
        FILE = 0b00000000,
        HIDDEN = 0b00001000,
        READ = 0b00000100,
        WRITE = 0b00000010,
        EXECUTE = 0b00000001;

    virtual ~FSObject() = default;
    virtual FSObject *clone() = 0;
    const std::string& getName() const;
    bool isDirectory() const;
    bool isFile() const;
    bool isHidden() const;
    bool canRead() const;
    bool canWrite() const;
    bool canExecute() const;
    void chmod(uint8_t mode);
    void rename(const std::string& name);
};

#endif // FSOBJECT_H_INCLUDED

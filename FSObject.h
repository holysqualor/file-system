#ifndef FSOBJECT_H_INCLUDED
#define FSOBJECT_H_INCLUDED

#include <iostream>
#include <string>
#include <cstdint>

class FSObject {
private:
    std::string name;
    uint8_t mode;

protected:
    FSObject(const std::string& name, uint8_t mode);

public:
    static const uint8_t HIDDEN = 0b1000, READ = 0b0100, WRITE = 0b0010, EXECUTE = 0b0001;

    virtual ~FSObject() = default;
    virtual FSObject *clone() = 0;
    virtual bool isDirectory() const = 0;
    virtual bool isFile() const = 0;

    std::string getType() const;

    virtual const std::string& getName() const final;
    virtual bool isHidden() const final;
    virtual bool canRead() const final;
    virtual bool canWrite() const final;
    virtual bool canExecute() const final;
    virtual void chmod(uint8_t mode) final;

    void rename(const std::string& name);
};

#endif // FSOBJECT_H_INCLUDED

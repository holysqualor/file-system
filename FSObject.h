#ifndef FSOBJECT_H_INCLUDED
#define FSOBJECT_H_INCLUDED

#include <iostream>
#include <string>
#include <cstdint>
#include <fstream>

class FSObject {
private:
    std::string name;
    uint8_t mode;

protected:
    FSObject(const std::string& name, uint8_t mode);
    static void readstr(std::ifstream &src, std::string &str);
    static void writestr(std::ofstream &dest, std::string &str);

public:
    static const uint8_t READ = 0b0100, WRITE = 0b0010, EXECUTE = 0b0001;

    static FSObject *load(std::ifstream &src);

    virtual ~FSObject() = default;
    virtual FSObject *clone() = 0;
    virtual bool isDirectory() const = 0;
    virtual bool isFile() const = 0;
    virtual void save(std::ofstream &dest);

    virtual const std::string& getName() const final;
    virtual bool canRead() const final;
    virtual bool canWrite() const final;
    virtual bool canExecute() const final;
    virtual void chmod(uint8_t mode) final;
    virtual void display() const final;
    virtual void rename(const std::string& name) final;
};

#endif // FSOBJECT_H_INCLUDED

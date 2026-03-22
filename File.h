#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include "FSObject.h"

#include <iostream>
#include <string>
#include <cstdint>

class File : public FSObject {
private:
    std::string content;

public:
    File(const std::string& name, const std::string& content);
    File(const std::string& name);
    File(const File& other) = default;
    ~File() = default;

    FSObject *clone() override;
    bool isDirectory() const override;
    bool isFile() const override;

    std::string getType() const;

    std::string read();
    void write(const std::string& name);

    friend std::ostream& operator<<(std::ostream& os, const File& file);

    friend std::istream& operator>>(std::istream& is, File& file);
};

#endif // FILE_H_INCLUDED

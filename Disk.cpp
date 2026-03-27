#include "Disk.h"

#include <sstream>
#include <cctype>

const uint32_t Disk::VDI = 0x2a45f0e0;

Disk::Workspace Disk::parse(std::string& path) {
    if(path.empty())
        return {NULL, ""};
    if(*(path.end() - 1) == '/')
        return {root, "."};
    Workspace ws = {current, ""};
    std::stringstream stream(path);
    std::string name;
    while(std::getline(stream, name, '/') && ws.directory) {
        if(stream.eof())
            ws.target = name;
        else ws.directory = name.empty() ? root : ws.directory->getDirectory(name);
    }
    return ws;
}

Disk::user_type Disk::login() {
    std::string name, password;
    while(true) {
        std::cout << "Login: ";
        std::getline(std::cin, name);
        std::cout << "Password: ";
        std::getline(std::cin, password);
        if(name == "root" && password == "1234")
            return ROOT;
        if(name == "user" && password == "0000")
            return USER;
        std::cout << "Incorrect username or password. Try again" << std::endl;
    }
    return NONE;
}

Disk::Disk(const std::string &filename) {
    this->filename = filename;
    std::ifstream image(filename, std::ios_base::in | std::ios_base::binary);
    if(image.is_open()) {
        uint32_t header = 0;
        image.read((char*)&header, sizeof(header));
        if(header != VDI)
            throw "Invalid file format!";
        root = (Directory*)FSObject::load(image);
        image.close();
    } else root = new Directory();
    current = root;
    user = login();
}

Disk::Disk(const Disk& other) {
    root = new Directory(*other.root);
    current = root;
}

Disk::Disk(Disk&& other)  {
    root = other.root;
    current = other.current;
    other.root = NULL;
    other.current = NULL;
}

Disk& Disk::operator=(const Disk& other) {
    delete root;
    root = new Directory(*other.root);
    current = root;
    return *this;
}

Disk::~Disk() {
    std::ofstream image(filename, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
    if(image.is_open()) {
        image.write((char*)&VDI, sizeof(VDI));
        root->save(image);
        image.close();
    }
    delete root;
}

Disk::status Disk::touch(std::vector<std::string> &args) {
    if(args.empty())
        return TOO_FEW_ARGUMENTS;
    for(std::string &path : args) {
        Workspace ws = parse(path);
        if(ws.empty())
            return BAD_PATH;
        if(ws.directory->get(ws.target))
            return ALREADY_EXISTS;
        if(!ws.directory->canWrite())
            return ACCESS_DENIED;
        ws.directory->add(new File(ws.target));
    }
    return SUCCESS;
}

Disk::status Disk::mkdir(std::vector<std::string> &args) {
    if(args.empty())
        return TOO_FEW_ARGUMENTS;
    for(std::string &path : args) {
        Workspace ws = parse(path);
        if(ws.empty())
            return BAD_PATH;
        if(ws.directory->get(ws.target))
            return ALREADY_EXISTS;
        if(!ws.directory->canWrite())
            return ACCESS_DENIED;
        ws.directory->add(new Directory(ws.target));
    }
    return SUCCESS;
}

Disk::status Disk::rm(std::vector<std::string> &args) {
    if(args.empty())
        return TOO_FEW_ARGUMENTS;
    for(std::string &path : args) {
        Workspace ws = parse(path);
        if(ws.empty())
            return BAD_PATH;
        if(!ws.directory->canWrite())
            return ACCESS_DENIED;
        FSObject *object = ws.directory->get(ws.target);
        if(!object)
            return NOT_FOUND;
        if(object->isDirectory() && current->has((Directory*)object))
            return BAD_OPERATION;
        ws.directory->erase(object);
        delete object;
    }
    return SUCCESS;
}

Disk::status Disk::cd(std::vector<std::string> &args) {
    if(args.empty()) {
        current = root;
        return SUCCESS;
    }
    if(args.size() > 1)
        return TOO_MANY_ARGUMENTS;
    Workspace ws = parse(args[0]);
    if(ws.empty())
        return BAD_PATH;
    FSObject *directory = ws.directory->get(ws.target);
    if(!directory)
        return NOT_FOUND;
    if(!directory->isDirectory())
        return NOT_DIRECTORY;
    if(!directory->canExecute())
        return ACCESS_DENIED;
    current = (Directory*)directory;
    return SUCCESS;
}

Disk::status Disk::ls(std::vector<std::string> &args) {
    if(args.size() > 1)
        return TOO_MANY_ARGUMENTS;
    if(args.empty()) {
        if(!current->canRead())
            return ACCESS_DENIED;
        current->show();
        return SUCCESS;
    }
    Workspace ws = parse(args[0]);
    if(ws.empty())
        return BAD_PATH;
    FSObject *directory = ws.directory->get(ws.target);
    if(!directory)
        return NOT_FOUND;
    if(!directory->isDirectory())
        return NOT_DIRECTORY;
    if(!directory->canRead())
        return ACCESS_DENIED;
    ((Directory*)directory)->show();
    return SUCCESS;
}

void Disk::banner() const {
    std::cout << "[" << (user == ROOT ? "root" : "user") << "@localhost: " << current->getName() << (user == ROOT ? "]# " : "]$ ");
}

Disk::status Disk::chmod(std::vector<std::string> &args) {
    if(user != ROOT)
        return ACCESS_DENIED;
    if(args.size() < 2)
        return TOO_FEW_ARGUMENTS;
    uint8_t mode = 0;
    if(args[0].length() != 1 || !isdigit(args[0][0]) || (mode = args[0][0] - '0') > 7)
        return INVALID_ARGUMENT;
    for(auto arg = args.begin() + 1; arg != args.end(); arg++) {
        Workspace ws = parse(*arg);
        if(ws.empty())
            return BAD_PATH;
        FSObject *obj = ws.directory->get(ws.target);
        if(!obj)
            return NOT_FOUND;
        obj->chmod(mode);
    }
    return SUCCESS;
}

Disk::status Disk::cat(std::vector<std::string> &args) {
    if(args.empty())
        return TOO_FEW_ARGUMENTS;
    for(std::string &path : args) {
        Workspace ws = parse(path);
        if(ws.empty())
            return BAD_PATH;
        FSObject *file = ws.directory->get(ws.target);
        if(!file)
            return NOT_FOUND;
        if(!file->isFile())
            return NOT_FILE;
        if(!file->canRead())
            return ACCESS_DENIED;
        if(!((File*)file)->read().empty())
            std::cout << ((File*)file)->read() << std::endl;
    }
    return SUCCESS;
}

Disk::status Disk::echo(std::vector<std::string> &args) {
    if(args.empty())
        return TOO_FEW_ARGUMENTS;
    if(args[0][0] != '\"')
        return INVALID_ARGUMENT;
    size_t last = 0;
    while(last < args.size() && *(args[last].end() - 1) != '\"')
        last++;
    if(last == args.size())
        return INVALID_ARGUMENT;
    std::string content;
    for(size_t i = 0; i < last; i++)
        content.append(args[i] + " ");
    content.append(args[last]);
    content.erase(content.begin());
    content.erase(content.end() - 1);
    if(last == args.size() - 1) {
        std::cout << content << std::endl;
        return SUCCESS;
    }
    for(size_t i = last + 1; i < args.size(); i++) {
        Workspace ws = parse(args[i]);
        if(ws.empty())
            return BAD_PATH;
        if(!ws.directory->canWrite())
            return ACCESS_DENIED;
        FSObject *file = ws.directory->get(ws.target);
        if(!file) {
            ws.directory->add(new File(ws.target, content));
            continue;
        }
        if(!file->isFile())
            return NOT_FILE;
        if(!file->canWrite())
            return ACCESS_DENIED;
        ((File*)file)->write(content);
    }
    return SUCCESS;
}


Disk::status Disk::mv(std::vector<std::string> &args) {
    if(args.size() != 2)
        return args.size() < 2 ? TOO_FEW_ARGUMENTS : TOO_MANY_ARGUMENTS;
    Workspace left = parse(args[0]), right = parse(args[1]);
    if(left.empty() || right.empty())
        return BAD_PATH;
    FSObject *a = left.directory->get(left.target), *b = right.directory->get(right.target);
    if(!a)
        return NOT_FOUND;
    if(!b) {
        if(left.directory == right.directory && !b) {
            a->rename(right.target);
            return SUCCESS;
        }
        return BAD_PATH;
    }
    if(!b->isDirectory())
        return NOT_DIRECTORY;
    Directory *dest = (Directory*)b;
    if(a->isDirectory() && (current->has((Directory*)a) || dest->has((Directory*)a)))
        return BAD_OPERATION;
    if(!dest->canWrite())
        return ACCESS_DENIED;
    if(dest->get(left.target))
        return ALREADY_EXISTS;
    left.directory->erase(a);
    dest->add(a);
    return SUCCESS;
}

Disk::status Disk::cp(std::vector<std::string> &args) {
    if(args.size() != 2)
        return args.size() < 2 ? TOO_FEW_ARGUMENTS : TOO_MANY_ARGUMENTS;
    Workspace src = parse(args[0]);
    if(src.empty())
        return BAD_PATH;
    FSObject *target = src.directory->get(src.target);
    if(!target)
        return NOT_FOUND;
    for(auto path = args.begin() + 1; path != args.end(); path++) {
        Workspace ws = parse(*path);
        if(ws.empty())
            return BAD_PATH;
        FSObject *obj = ws.directory->get(ws.target);
        if(obj && !obj->isDirectory())
            return NOT_DIRECTORY;
        Directory *dest = obj ? (Directory*)obj : ws.directory;
        std::string name = obj ? target->getName() : ws.target;
        if(target->isDirectory() && dest->has((Directory*)target))
            return BAD_OPERATION;
        if(!dest->canWrite())
            return ACCESS_DENIED;
        if(dest->get(name))
            return ALREADY_EXISTS;
        FSObject *targetCopy = target->clone();
        targetCopy->rename(name);
        dest->add(targetCopy);
    }
    return SUCCESS;
}

Disk::status Disk::logout(std::vector<std::string> &args) {
    if(!args.empty())
        return TOO_MANY_ARGUMENTS;
    user = login();
    return SUCCESS;
}

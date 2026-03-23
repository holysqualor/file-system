#include "Disk.h"

#include <sstream>

Disk::Workspace Disk::parse(std::string& path) {
    if(path.empty())
        return {NULL, ""};
    if(*(path.end() - 1) == '/')
        return {root, "."};
    Workspace ws = {current, ""};
    std::stringstream stream(path);
    std::string name;
    while(std::getline(stream, name, '/') && ws.directory) {
        if(!stream.eof())
            ws.directory = name.empty() ? root : ws.directory->getDirectory(name);
        else
            ws.target = name;
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

Disk::Disk() : root{new Directory()}, current{root} {
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
    delete root;
}

Disk::status Disk::touch(std::vector<std::string> &args) {
    if(args.empty())
        return TOO_FEW_ARGUMENTS;
    for(std::string &path : args) {
        Workspace ws = parse(path);
        if(!ws.directory || ws.target.empty())
            return BAD_PATH;
        if(ws.directory->find(ws.target))
            return ALREADY_EXISTS;
        if(!ws.directory->canWrite())
            return ACCESS_DENIED;
        ws.directory->addFile(ws.target);
    }
    return SUCCESS;
}

Disk::status Disk::mkdir(std::vector<std::string> &args) {
    if(args.empty())
        return TOO_FEW_ARGUMENTS;
    for(std::string &path : args) {
        Workspace ws = parse(path);
        if(!ws.directory || ws.target.empty())
            return BAD_PATH;
        if(ws.directory->find(ws.target))
            return ALREADY_EXISTS;
        if(!ws.directory->canWrite())
            return ACCESS_DENIED;
        ws.directory->addDirectory(ws.target);
    }
    return SUCCESS;
}

Disk::status Disk::rm(std::vector<std::string> &args) {
    if(args.empty())
        return TOO_FEW_ARGUMENTS;
    for(std::string &path : args) {
        Workspace ws = parse(path);
        if(!ws.directory || ws.target.empty())
            return BAD_PATH;
        if(!ws.directory->canWrite())
            return ACCESS_DENIED;
        FSObject *obj = ws.directory->find(ws.target);
        if(!obj)
            return NOT_FOUND;
        if(obj->isDirectory() && !current->isindp((Directory*)obj))
            return BAD_OPERATION;
        ws.directory->rm(ws.target);
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
    if(!ws.directory || ws.target.empty())
        return BAD_PATH;
    Directory *directory = ws.directory->getDirectory(ws.target);
    if(!directory)
        return NOT_FOUND;
    if(!directory->canExecute())
        return ACCESS_DENIED;
    current = directory;
    return SUCCESS;
}

Disk::status Disk::ls(std::vector<std::string> &args) {
    if(args.empty()) {
        if(!current->canRead())
            return ACCESS_DENIED;
        current->ls();
        return SUCCESS;
    }
    for(std::string &path : args) {
        Workspace ws = parse(path);
        if(!ws.directory || ws.target.empty())
            return BAD_PATH;
        FSObject *obj = ws.directory->find(ws.target);
        if(!obj)
            return NOT_FOUND;
        obj->display();
    }
    return SUCCESS;
}

Disk::status Disk::pwd(std::vector<std::string> &args) {
    if(!args.empty())
        return TOO_MANY_ARGUMENTS;
    current->pwd();
    std::cout << std::endl;
    return SUCCESS;
}

void Disk::banner() const {
    std::cout << "[" << (user == ROOT ? "root" : "user") << "@localhost: " << current->getName() << (user == ROOT ? "]# " : "]$ ");
}

Disk::status Disk::chmod(std::vector<std::string> &args) {
    if(args.size() < 2)
        return TOO_FEW_ARGUMENTS;
    uint8_t mode =
        (args[0].find('r') == std::string::npos ? 0 : FSObject::READ) |
        (args[0].find('w') == std::string::npos ? 0 : FSObject::WRITE) |
        (args[0].find('x') == std::string::npos ? 0 : FSObject::EXECUTE) |
        (args[0].find('h') == std::string::npos ? 0 : FSObject::HIDDEN);
    for(size_t i = 1; i < args.size(); i++) {
        Workspace ws = parse(args[i]);
        if(!ws.directory || ws.target.empty())
            return BAD_PATH;
        FSObject *obj = ws.directory->find(ws.target);
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
        if(!ws.directory || ws.target.empty())
            return BAD_PATH;
        File *file = ws.directory->getFile(ws.target);
        if(!file)
            return NOT_FOUND;
        if(!file->canRead())
            return ACCESS_DENIED;
        std::cout << file->read() << std::endl;
    }
    return SUCCESS;
}

Disk::status Disk::echo(std::vector<std::string> &args) {
    if(args.empty())
        return TOO_FEW_ARGUMENTS;
    if(args[0][0] != '\"')
        return INVALID_ARGUMENT;
    size_t last = 0;
    while(last < args.size() && *(args[last].end() - 1) != '\"') last++;
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
        if(!ws.directory || ws.target.empty())
            return BAD_PATH;
        if(!ws.directory->canWrite())
            return ACCESS_DENIED;
        FSObject *file = ws.directory->find(ws.target);
        if(!file) {
            ws.directory->addFile(ws.target, content);
            continue;
        }
        if(file->isDirectory())
            return NOT_FOUND;
        if(!file->canWrite())
            return ACCESS_DENIED;
        ((File*)file)->write(content);
    }
    return SUCCESS;
}


Disk::status Disk::mv(std::vector<std::string> &args) {
    #define halt(err) do {\
        left.directory->push(a);\
        return err;\
    } while(false);
    if(args.size() != 2)
        return args.size() < 2 ? TOO_FEW_ARGUMENTS : TOO_MANY_ARGUMENTS;
    Workspace left = parse(args[0]), right = parse(args[1]);
    if(!left.directory || left.target.empty() || !right.directory || right.target.empty())
        return BAD_PATH;
    FSObject *a = left.directory->pop(left.target), *b = right.directory->find(right.target);
    if(!a)
        return NOT_FOUND;

    if(left.directory == right.directory && !b) {
        a->rename(right.target);
        halt(SUCCESS);
    }
    if(!b)
        halt(BAD_PATH);

    if(b->isFile())
        halt(NOT_FOUND);

    Directory *dest = (Directory*)b;
    if(dest->find(left.target))
        halt(ALREADY_EXISTS);

    dest->push(a);
    return SUCCESS;
}

Disk::status Disk::logout(std::vector<std::string> &args) {
    if(!args.empty())
        return TOO_MANY_ARGUMENTS;
    user = login();
    current = root;
    return SUCCESS;
}

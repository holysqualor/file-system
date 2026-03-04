#ifndef JOURNAL_H_INCLUDED
#define JOURNAL_H_INCLUDED

#include <iostream>
#include <vector>
#include <string>

class Journal {
private:
    std::vector<std::string> logs;

public:
    Journal() = default;
    void logAction(const std::string& action, bool status);
    void printLogs();
};

#endif // JOURNAL_H_INCLUDED

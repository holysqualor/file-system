#include "Journal.h"
#include <ctime>

void Journal::logAction(const std::string& action, bool status) {
    time_t curTime = time(NULL);
    std::string log(ctime(&curTime));
    log.pop_back();
    logs.push_back(log + ": " + action + " | " + (status ? "SUCCESS" : "ERROR"));
}

void Journal::printLogs() {
    for(std::string log : logs)
        std::cout << log << std::endl;
}

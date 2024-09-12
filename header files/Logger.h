#ifndef __Logger_H
#define __Logger_H

#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <mutex>
#include <string>


// Enum to represent log levels
enum LogLevel { DEBUG, INFO, WARNING, ERROR, CRITICAL };

class Logger {
public:
    Logger(const std::string& filename);
    ~Logger();
    void log(LogLevel level, const std::string& message); // Logs a message to log file 

private:
    std::ofstream m_log_file;  // File stream for the log file
    std::mutex m_log_mutex;    // Mutex for logFile thread-safe access
 
    std::string levelToString(LogLevel level);  // Converts log level to string for output
};

#endif
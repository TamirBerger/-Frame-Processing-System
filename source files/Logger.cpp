#include "Logger.h"


// Ctor: Opens the log file in append mode
Logger::Logger(const std::string& filename) {
    m_log_file.open(filename, std::ios::app);
    if (!m_log_file.is_open()) {
        std::cerr << "Error opening log file." << std::endl;
    }
}

// Dtor: Closes the log file
Logger::~Logger() {
    m_log_file.close();
}

// Logs a message with a given log level
void Logger::log(LogLevel level, const std::string& message) {
    // Get current local time
    time_t now = time(0);
    tm timeinfo;
    localtime_r(&now, &timeinfo);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &timeinfo);

    std::ostringstream logEntry;
    logEntry << "[" << timestamp << "] " << levelToString(level) << ": " << message << std::endl;

    std::lock_guard<std::mutex> guard(m_log_mutex);
    // Critical section entry
    std::cout << logEntry.str(); // Output to console

    // Output to log file
    if (m_log_file.is_open()) {
        m_log_file << logEntry.str();
        m_log_file.flush();  // Ensure immediate write to file
    }
    else {
        std::cerr << "Error: Logger: Log file isn't open" << std::endl;
    }
    // End of critical section
}

// Converts log level to a string for output
std::string Logger::levelToString(LogLevel level) {
    switch (level) {
    case DEBUG: return "DEBUG";
    case INFO: return "INFO";
    case WARNING: return "WARNING";
    case ERROR: return "ERROR";
    case CRITICAL: return "CRITICAL";
    default: return "UNKNOWN";
    }
}
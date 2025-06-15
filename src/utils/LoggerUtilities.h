#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>
#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>

enum class LogLevel
{
    SFINFO,
    SFWARNING,
    SFERROR,
    SFDEBUG
};

class Logger {
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void log(const std::string& message, LogLevel level = LogLevel::SFINFO) {
        std::lock_guard<std::mutex> lock(logMutex);
        std::string timestamp = getTimestamp();
        std::string levelStr = levelToString(level);
        std::string formatted = "[" + timestamp + "] [" + levelStr + "] " + message;

        if (logFile.is_open()) {
            logFile << formatted << std::endl;
        }

        if (consoleOutputEnabled) {
            std::cout << formatted << std::endl;
        }
    }

    void setLogFile(const std::string& filename) {
        std::lock_guard<std::mutex> lock(logMutex);
        if (logFile.is_open()) {
            logFile.close();
        }
        logFile.open(filename, std::ios::app);
        if (!logFile) {
            std::cerr << "[Logger] Failed to open log file: " << filename << std::endl;
        }
    }

    void enableConsoleOutput(bool enable) {
        consoleOutputEnabled = enable;
    }

private:
    Logger() {
        setLogFile("application.log");
    }

    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::string getTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm buf{};
#if defined(_WIN32)
        localtime_s(&buf, &in_time_t);
#else
        localtime_r(&in_time_t, &buf);
#endif
        std::ostringstream oss;
        oss << std::put_time(&buf, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    std::string levelToString(LogLevel level) {
        switch (level) {
        case LogLevel::SFINFO: return "INFO";
        case LogLevel::SFWARNING: return "WARNING";
        case LogLevel::SFERROR: return "ERROR";
        case LogLevel::SFDEBUG: return "DEBUG";
        default: return "UNKNOWN";
        }
    }

    std::ofstream logFile;
    std::mutex logMutex;
    bool consoleOutputEnabled = true;
};

#define LOGCTX(msg, level) \
    Logger::getInstance().log(std::string(__FILE__) + ":" + std::to_string(__LINE__) + " " + (msg), (level))

#define LOGINFO(msg)    Logger::getInstance().log((msg), LogLevel::SFINFO)
#define LOGWARNING(msg) LOGCTX((msg), LogLevel::SFWARNING)
#define LOGERROR(msg)   LOGCTX((msg), LogLevel::SFERROR)
#define LOGDEBUG(msg)   LOGCTX((msg), LogLevel::SFDEBUG)

#endif // LOGGER_H

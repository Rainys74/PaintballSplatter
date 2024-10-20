#pragma once

#include <sstream>
#include <string>

#include "Core.h"

#ifdef PS_PLATFORM_WINDOWS
#include "Windows.h"
#endif

#ifdef PS_PLATFORM_LINUX
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define PURPLE  "\033[35m" //"\e[35m"
#endif

#ifdef PS_PLATFORM_WINDOWS
#define RESET   7
#define RED     12
#define GREEN   10
#define YELLOW  14
#define BLUE    9
#define PURPLE  5
#define DARKBLUE 1
#define LIGHTBLUE 3
#define GRAY    8
#endif

#ifdef PS_PLATFORM_WINDOWS
#define COLORTYPE   int
//#define PTRORRF     * //&
#endif
#ifdef PS_PLATFORM_LINUX
#define COLORTYPE   std::string
#define *     *
#endif

#define INFO_COLOR      RESET
#define LOG_COLOR       GREEN
#define WARN_COLOR      YELLOW
#define ERROR_COLOR     PURPLE
#define CRITICAL_COLOR  RED

namespace Engine
{
class Logger
{
public:
    // Constructor to set the output stream
    Logger(std::ostream& outputStream) : logStream(outputStream) {}

    // Overload the << operator to append log messages
    template <typename T>
    Logger& operator<<(const T& data) 
    {
        logStream << data;
        return *this;
    }

    // Overload the << operator for manipulators (e.g., std::endl)
    Logger& operator<<(std::ostream& (*manip)(std::ostream&)) 
    {
        manip(logStream);
        return *this;
    }

    // Function to print the log message
    void PrintLog();
    static void ResetColor();
    // Static member functions to get logger instances
    static Logger* Info();
    static Logger* Log();
    static Logger* Warn();
    static Logger* Error();
    static Logger* Severe();

private:
    std::ostream& logStream;

    static Logger* ReturnLogger(COLORTYPE color, const std::string& logName, std::ostream& outputStream);
};
}

#ifdef PS_PLATFORM_WINDOWS
#define RESET_COLOR     ; Engine::Logger::ResetColor()
#endif
#ifdef PS_PLATFORM_LINUX
#define RESET_COLOR     << RESET
#endif

#ifdef PS_DEBUG
#ifdef PS_PLATFORM_WINDOWS
    #define PS_LOGGER_INFO(x)  (*Engine::Logger::Info() << x << "\n") RESET_COLOR
    #define PS_LOGGER_LOG(x)   (*Engine::Logger::Log() << x << "\n") RESET_COLOR
    #define PS_LOGGER_WARN(x)  (*Engine::Logger::Warn() << x << "\n") RESET_COLOR
    #define PS_LOGGER_ERROR(x) (*Engine::Logger::Error() << x << "\n") RESET_COLOR
    #define PS_LOGGER_FATAL(x) (*Engine::Logger::Severe() << x << "\n") RESET_COLOR
#endif
#ifdef PS_PLATFORM_LINUX
    #define PS_LOGGER_INFO(x)  (*Engine::Logger::Info() << x << "\n") RESET_COLOR
    #define PS_LOGGER_LOG(x)   (*Engine::Logger::Log() << x << "\n") RESET_COLOR
    #define PS_LOGGER_WARN(x)  (*Engine::Logger::Warn() << x << "\n") RESET_COLOR
    #define PS_LOGGER_ERROR(x) (*Engine::Logger::Error() << x << "\n") RESET_COLOR
    #define PS_LOGGER_FATAL(x) (*Engine::Logger::Severe() << x << "\n") RESET_COLOR
#endif
#elif defined(PS_RELEASE)
#define PS_LOGGER_INFO(x)
#define PS_LOGGER_LOG(x)
#define PS_LOGGER_WARN(x)
#define PS_LOGGER_ERROR(x)
#define PS_LOGGER_FATAL(x)
#endif
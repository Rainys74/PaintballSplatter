#include "logger.h"

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

namespace Engine
{
    void Logger::PrintLog()
    {
        logStream << std::endl;  // Add a newline to the log message
        logStream.flush();       // Flush the log stream
    }

    void Logger::ResetColor()
    {
        #ifdef PS_PLATFORM_LINUX
        //logStream << "RESET";
        #endif
        #ifdef PS_PLATFORM_WINDOWS
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, RESET);
        #endif
    }

    // Static member functions to get logger instances
    Logger* Logger::Info()
    {
        return ReturnLogger(INFO_COLOR,     "Info: ", std::cout);     //return info;
    }
    Logger* Logger::Log()
    {
        return ReturnLogger(LOG_COLOR,      "Log: ", std::cout);      //return log;
    }
    Logger* Logger::Warn()
    {
        return ReturnLogger(WARN_COLOR,     "Warning: ", std::cerr);     //return warning;
    }
    Logger* Logger::Error()
    {
        return ReturnLogger(ERROR_COLOR,    "Error: ", std::cerr);    //return error;
    }
    Logger* Logger::Severe()
    {
        return ReturnLogger(CRITICAL_COLOR, "Fatal: ", std::cerr); //return critical;
    }

    Logger* Logger::ReturnLogger(COLORTYPE color, const std::string& logName, std::ostream& outputStream)
    {
        //std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        //std::cout << std::ctime(&currentTime) << "\n";
        //std::cout << std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
    
        // Get the current time
        std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm* timeInfo = std::localtime(&currentTime);

        //const std::string& formattedTime = "[" + std::to_string(timeInfo->tm_hour) + ":" + std::to_string(timeInfo->tm_min) + ":" + std::to_string(timeInfo->tm_sec) + "]: ";
        
        // Format the time with leading zeros
        std::stringstream formattedTimeSStream;
        // max 13 chars
        formattedTimeSStream << "[" << std::setfill('0') << std::setw(2) << timeInfo->tm_hour << ":" << std::setfill('0') << std::setw(2) << timeInfo->tm_min << ":" << std::setfill('0') << std::setw(2) << timeInfo->tm_sec << "]: ";

        const std::string& formattedTime = formattedTimeSStream.str();

        #ifdef PS_PLATFORM_LINUX
        return new Logger(outputStream << color << formattedTime << logName);
        #endif

        #ifdef PS_PLATFORM_WINDOWS
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);

        //static Logger logger(outputStream << formattedTime);
        return new Logger(outputStream << formattedTime << logName); //return logger;
        #endif
    }
}
#include "Debug.h"
#include <fstream>
#include <cstdio>
#include <cstdarg>

Debug* Debug::instance = nullptr;

Debug::Debug() = default;


void Debug::Log(const char* message, ...)
{
    GetInstance();

    instance->logStream.open("log.txt", std::fstream::out | std::fstream::app);
    instance->logStream << Debug::prefix( __FILE__ , __LINE__ );

    va_list args;
    va_start(args, message);

    while (*message != '\0')
    {
        if (*message == '%') {
            ++message;

            if (*message == 'd')
            {
                int i = va_arg(args, int);
                instance->logStream << i;
            }
            else if (*message  == 'c')
            {
                int c = va_arg(args, int);
                instance->logStream << static_cast<char>(c);
            }
            else if (*message == 'f')
            {
                double d = va_arg(args, double);
                instance->logStream << d;
            }
            else if (*message == 's')
            {
                char* s = va_arg(args, char*);
                instance->logStream << s;
            }
            else
            {
                instance->logStream << '%' << message;
            }
        }
        else
        {
            instance->logStream << *message;
        }
        ++message;
    }

    va_end(args);

    instance->logStream << std::endl;
    instance->logStream.close();

}

Debug *Debug::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new Debug();
        remove("log.txt");
    }

    return instance;
}

void Debug::OpenFile()
{
    if (!instance->logStream.is_open())
        instance->logStream.open("log.txt", std::fstream::out | std::fstream::app);
}

void Debug::CloseFile()
{
    if (instance->logStream.is_open())
        instance->logStream.close();
}

std::string Debug::prefix(const std::string &file, const int line)
{
    std::string result;

    result += "{" + std::to_string(instance->logCount) + "} " + file + "(" + std::to_string(line) + ") :  ";

    instance->logCount++;

    return result;
}
#pragma once

#include <fstream>
#include <string>
#include <iostream>

#define log (*Debug::GetInstance() << Debug::GetInstance()->prefix( __FILE__ , __LINE__ ))
#define lendl (*Debug::GetInstance() << Debug::)

class Debug
{
public:
    static void Log(const char* message, ...);

protected:
    Debug();

public:
    Debug(Debug &other) = delete;

    void operator=(const Debug &) = delete;

    static Debug *GetInstance();

    static std::string prefix( const std::string& file , int line );

public:
    template< typename T > Debug& operator<<( T t );
    Debug& operator<<( std::ostream& (*fun)( std::ostream& ) );

private:
    static void OpenFile();
    static void CloseFile();

private:
    static Debug* instance;
    std::fstream logStream;
    int logCount = 0;

};

template< typename T > inline Debug& Debug::operator<<( T t )
{
    if ( instance == nullptr )
        GetInstance();

    Debug::OpenFile();

    instance->logStream << t;
    std::cout << t;

    Debug::CloseFile();

    return *this;
}

inline Debug& Debug::operator<<( std::ostream& (*fun)( std::ostream& ) )
{
    if ( instance == nullptr )
        GetInstance();

    Debug::OpenFile();

    instance->logStream << std::endl;
    std::cout << std::endl;

    Debug::CloseFile();

    return *this;
}
#pragma once

#include <exception>
#include <string>

class FatException : public std::exception
{
private:
    int linenum;
    std::string filename;


protected:
    mutable std::string whatBuffer;


public:
    FatException( int line_num, const char* file_name ) noexcept;

    std::string GetOriginString() const noexcept;

    const char* what() const noexcept override;
    virtual const char* GetType() const noexcept;

    int GetLine() const noexcept;
};
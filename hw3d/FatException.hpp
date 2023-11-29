#pragma once

#include <exception>
#include <string>

class FatException : public std::exception
{
public:
    FatException( int line_num, const char* file_name ) noexcept;


public:
    std::string GetOriginString() const noexcept;
    std::string GetFileName() const noexcept;

    virtual const char* GetType() const noexcept;
    const char* what() const noexcept override;

    int GetLineNum() const noexcept;


protected:
    mutable std::string whatBuffer;


private:
    std::string filename;

    int linenum;
};
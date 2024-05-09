#pragma once

#include <exception>
#include <string>

class FatException : public std::exception
{
public:
    FatException(int line_num, const char* file_name) noexcept;


public:
    virtual const char* what() const noexcept override;
    virtual const char* GetType() const noexcept;


public:
    std::string GetOriginString() const noexcept;
    std::string GetFileName() const noexcept;

    int GetLineNum() const noexcept;


protected:
    mutable std::string what_buffer_;


private:
    std::string file_name_;

    int line_num_;
};
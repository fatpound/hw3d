#include "FatException.hpp"
#include <sstream>

FatException::FatException(int line_num, const char* file_name) noexcept
    :
    linenum(line_num),
    filename(file_name)
{}

const char* FatException::what() const noexcept
{
    std::ostringstream oss;

    oss << GetType() << '\n'
        << GetOriginString();

    whatBuffer = oss.str();

    return whatBuffer.c_str();
}
#include "FatException.hpp"
#include <sstream>

FatException::FatException(int line_num, const char* file_name) noexcept
    :
    linenum(line_num),
    filename(file_name)
{

}

std::string FatException::GetOriginString() const noexcept
{
    std::ostringstream oss;

    oss << "[File] : " << filename << '\n'
        << "[Line] : " << linenum;

    return oss.str();
}
std::string FatException::GetFileName() const noexcept
{
    return filename;
}

const char* FatException::GetType() const noexcept
{
    return "Fat Exception";
}
const char* FatException::what() const noexcept
{
    std::ostringstream oss;

    oss << GetType() << '\n'
        << GetOriginString();

    whatBuffer = oss.str();

    return whatBuffer.c_str();
}

int FatException::GetLineNum() const noexcept
{
    return linenum;
}
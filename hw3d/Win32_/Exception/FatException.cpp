#include "FatException.hpp"

#include <sstream>

FatException::FatException(int line_num, const char* file_name) noexcept
    :
    line_num_(line_num),
    file_name_(file_name)
{

}

std::string FatException::GetOriginString() const noexcept
{
    std::ostringstream oss;

    oss << "[File] : " << file_name_ << '\n'
        << "[Line] : " << line_num_;

    return oss.str();
}
std::string FatException::GetFileName() const noexcept
{
    return file_name_;
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

    what_buffer_ = oss.str();

    return what_buffer_.c_str();
}

int FatException::GetLineNum() const noexcept
{
    return line_num_;
}
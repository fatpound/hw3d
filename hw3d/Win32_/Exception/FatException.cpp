#include "FatException.hpp"

#include <sstream>

FatException::FatException(int line_num, const char* file_name) noexcept
    :
    line_num_(line_num),
    file_name_(file_name)
{

}

const char* FatException::what() const noexcept
{
    std::ostringstream oss;

    oss << GetType() << '\n'
        << GetOriginString();

    what_buffer_ = oss.str();

    return what_buffer_.c_str();
}
const char* FatException::GetType() const noexcept
{
    return "Fat Exception";
}

auto FatException::GetOriginString() const noexcept -> std::string
{
    std::ostringstream oss;

    oss << "[File] : " << file_name_ << '\n'
        << "[Line] : " << line_num_;

    return oss.str();
}
auto FatException::GetFileName() const noexcept -> std::string
{
    return file_name_;
}

int FatException::GetLineNum() const noexcept
{
    return line_num_;
}
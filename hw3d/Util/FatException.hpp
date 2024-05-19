#pragma once

#include <exception>
#include <string>

namespace fatpound::util
{
    class FatException : public std::exception
    {
    public:
        FatException(int line_num, const char* file_name) noexcept;


    public:
        virtual const char* what() const noexcept override;
        virtual const char* GetType() const noexcept;


    public:
        auto GetOriginString() const noexcept -> std::string;
        auto GetFileName()     const noexcept -> std::string;

        int GetLineNum() const noexcept;


    protected:
        mutable std::string what_buffer_;


    private:
        std::string file_name_;

        int line_num_;
    };
}
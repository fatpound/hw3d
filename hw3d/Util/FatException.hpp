#pragma once

#include <exception>
#include <string>

namespace fatpound::util
{
    class FatException : public std::exception
    {
    public:
        FatException(int line_num, const char* file_name) noexcept;

        FatException() = delete;
        FatException(const FatException& src) = default;
        FatException& operator = (const FatException& src) = default;

        FatException(FatException&& src) = default;
        FatException& operator = (FatException&& src) = default;
        virtual ~FatException() = default;


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
#pragma once

#include "FatWin.hpp"

#include <string>
#include <sstream>
#include <iomanip>
#include <unordered_map>

class WindowsMessageMap
{
public:
    WindowsMessageMap() noexcept;

    std::string operator () ( DWORD msg, WPARAM wp, LPARAM lp ) const noexcept;


protected:


private:
    std::unordered_map<DWORD, std::string> map;
};
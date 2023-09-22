#pragma once

#include "FatWin.hpp"

#include <string>
#include <sstream>
#include <iomanip>
#include <unordered_map>

class WindowsMessageMap
{
private:
    std::unordered_map<DWORD, std::string> map;


public:
    WindowsMessageMap() noexcept;

    std::wstring operator () ( DWORD msg, WPARAM wp, LPARAM lp ) const noexcept;
};
#pragma once

#include "FatWin32_.hpp"

#include <string>
#include <unordered_map>

class WindowsMessageMap final
{
public:
    WindowsMessageMap() noexcept;

    
public:
    std::string operator () (DWORD msg, WPARAM wp, LPARAM lp) const noexcept;


protected:


private:
    std::unordered_map<DWORD, std::string> map_;
};
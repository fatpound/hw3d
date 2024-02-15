#pragma once

#define FULL_WINTARD

#include "FatWin.hpp"

class GDIPlusManager
{
public:
    GDIPlusManager();
    GDIPlusManager(const GDIPlusManager& src) = delete;
    GDIPlusManager(GDIPlusManager&& src) = delete;
    GDIPlusManager& operator = (const GDIPlusManager& src) = delete;
    GDIPlusManager& operator = (GDIPlusManager&& src) = delete;
    ~GDIPlusManager();


protected:


private:
    static ULONG_PTR token_;
    static int refCount_;
};
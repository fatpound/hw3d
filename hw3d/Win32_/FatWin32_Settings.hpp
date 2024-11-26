#pragma once

// target Windows 7 or later
#define _WIN32_WINNT 0x0601

#include <sdkddkver.h>

#define NOMINMAX
#define STRICT

#ifndef FATPOUND_FULL_WIN_TARGETED
#define WIN32_LEAN_AND_MEAN // for 16-bit Windows libraries, rarely used
#define NOGDICAPMASKS
#define NOMENUS
#define NOICONS
#define NORASTEROPS
#define OEMRESOURCE
#define NOCLIPBOARD
#define NOCTLMGR
#define NODRAWTEXT
#define NOKERNEL
#define NOMEMMGR
#define NOMETAFILE
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE
// #define NOSYSMETRICS
// #define NOSYSCOMMANDS
// #define NOATOM
// #define NOCOLOR
// #define NONLS
#endif


// #define _CRTDBG_MAP_ALLOC
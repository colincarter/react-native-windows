// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define FOLLY_NO_CONFIG

#include "targetver.h"

#define _SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING
#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#pragma push_macro("GetCurrentTime")
#pragma push_macro("TRY")
#undef GetCurrentTime
#undef TRY

#include <unknwn.h>
#include <winrt/base.h>
#include <winrt/Windows.UI.Xaml.Controls.h>
#include <winrt/Windows.UI.Xaml.Hosting.h>

#pragma pop_macro("TRY")
#pragma pop_macro("GetCurrentTime")

#include <Windows.UI.Xaml.Hosting.DesktopWindowXamlSource.h>

#include <memory>

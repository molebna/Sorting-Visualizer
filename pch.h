//***************************************************************************************

#pragma once

#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers
#define _AFX_ALL_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "targetver.h"

#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <string>
#include <ctime>
#include <chrono>
#include <thread>
#include <atomic>
#include <iostream>
#include <fstream>

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>       // MFC support for Internet Explorer 4 Common Controls
#include <afxcmn.h>         // MFC support for Windows Common Controls

//#ifdef _UNICODE
//#if defined _M_IX86
//#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
//#elif defined _M_X64
//#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
//#else
//#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
//#endif
//#endif

#define WM_REPAINT            0x8101
#define WM_SORTING_FINISHED   0x8102

//***************************************************************************************


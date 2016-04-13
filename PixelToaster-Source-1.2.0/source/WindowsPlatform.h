// Windows Platform
// Copyright © Glenn Fiedler
// http://www.pixeltoaster.com

#define VC_EXTRALEAN 
#define WIN32_LEAN_AND_MEAN 

#include <windows.h>
#include <windowsx.h>

#include <d3d9.h>

#ifndef __MINGW32__ 
#include <d3dx9.h> 
#endif

namespace PixelToaster
{
	#include "WindowsAdapter.h"
	#include "WindowsWindow.h"
	#include "WindowsDevice.h"
	#include "WindowsDisplay.h"
	#include "WindowsTimer.h"
}

#define TimerClass WindowsTimer
#define DisplayClass WindowsDisplay

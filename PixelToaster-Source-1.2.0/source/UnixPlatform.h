// Unix Platform
// Copyright © Glenn Fiedler
// http://www.pixeltoaster.com

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cassert>
#include <vector>

namespace PixelToaster
{
	#include "UnixDisplay.h"
	#include "UnixTimer.h"

	//#define TimerClass UnixTimer				// note: using portable timer for the moment
	#define TimerClass PortableTimer
	#define DisplayClass UnixDisplay
}

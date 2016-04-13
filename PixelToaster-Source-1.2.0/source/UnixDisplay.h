// Unix Display
// Copyright © Glenn Fiedler
// http://www.pixeltoaster.com

// X11 version implemented by Bram de Greve <bramz@users.sourceforge.net>
// inspired by X11 version of TinyPTC by Alessandro Gatti <a.gatti@tiscali.it>

Format findFormat(int bitsPerPixel, unsigned long redMask, unsigned long greenMask, unsigned long blueMask)
{
	switch (bitsPerPixel)
	{
	case 16:
		if (redMask == 0x7c00 && greenMask == 0x03e0 && blueMask == 0x001f)
			return Format::XRGB1555;
		if (redMask == 0x001f && greenMask == 0x03e0 && blueMask == 0x7c00)
			return Format::XBGR1555;
		if (redMask == 0xf800 && greenMask == 0x07e0 && blueMask == 0x001f)
			return Format::RGB565;
		if (redMask == 0x001f && greenMask == 0x07e0 && blueMask == 0xf800)
			return Format::BGR565;
		break;
	case 24:
		if (redMask == 0xff0000 && greenMask == 0x00ff00 && blueMask == 0x0000ff)
			return Format::RGB888;
		if (redMask == 0x0000ff && greenMask == 0x00ff00 && blueMask == 0xff0000)
			return Format::BGR888;
		break;
	case 32:
		if (redMask == 0xff0000 && greenMask == 0x00ff00 && blueMask == 0x0000ff)
			return Format::XRGB8888;
		if (redMask == 0x0000ff && greenMask == 0x00ff00 && blueMask == 0xff0000)
			return Format::XBGR8888;
		break;
	}
	return Format::Unknown;
}

class UnixDisplay : public DisplayAdapter
{
public:
	
	UnixDisplay()
	{
		defaults();

		// ...
	}
	
	~UnixDisplay()
	{
		// ...
	}

	bool open( const char title[], int width, int height, Output output, Mode mode )
	{
		DisplayAdapter::open( title, width, height, output, mode );

		// let's open a display
		
		display_ = ::XOpenDisplay(0);
		if (!display_)
		{
			close();
			return false;
		}

		const int screen = DefaultScreen(display_);
		::Visual* visual = DefaultVisual(display_, screen);
		if (!visual)
		{
			close();
			return false;
		}

		// It gets messy when talking about color depths.
		//
		// For the image buffer, we either need 8, 16 or 32 bitsPerPixel.  8 bits we'll 
		// never have (hopefully), 16 bits will be used for displayDepth 15 & 16, and 
		// 32 bits must be used for depths 24 and 32.
		//
		// The converters will get this right when talking about displayDepth 15 & 16, but 
		// it will wrongly assume that displayDepth 24 takes _exactly_ 24 bitsPerPixel.  We 
		// solve that by tricking the converter requester by presenting it a 32 bit
		// bufferDepth instead.
		//
		const int displayDepth = DefaultDepth(display_, screen);
		const int bufferDepth = displayDepth == 24 ? 32 : displayDepth;
		const int bytesPerPixel = (bufferDepth + 7) / 8;
		const int bitsPerPixel = 8 * bytesPerPixel;
		if (bitsPerPixel != 16 && bitsPerPixel != 32)
		{
			close();
			return false;
		}

		Format destFormat = findFormat(bufferDepth,
			visual->red_mask, visual->green_mask, visual->blue_mask);
		floatingPointConverter_ = requestConverter(Format::XBGRFFFF, destFormat);
		trueColorConverter_ = requestConverter(Format::XRGB8888, destFormat);
		if (!floatingPointConverter_ || !trueColorConverter_)
		{
			close();
			return false;
		}


		// let's create a window
		
		const Window root = DefaultRootWindow(display_);
		
		const int screenWidth = DisplayWidth(display_, screen);
		const int screenHeight = DisplayHeight(display_, screen);
		const int left = (screenWidth - width) / 2;
		const int top = (screenHeight - height) / 2;

		::XSetWindowAttributes attributes;
		attributes.border_pixel = attributes.background_pixel = BlackPixel(display_, screen);
		attributes.backing_store = NotUseful;

		window_ = ::XCreateWindow(display_, root, left, top, width, height, 0,
			displayDepth, InputOutput, visual, 
			CWBackPixel | CWBorderPixel | CWBackingStore, &attributes);

		::XStoreName(display_, window_, title);

		::XSizeHints sizeHints;
		sizeHints.flags = PPosition | PMinSize | PMaxSize;
		sizeHints.x = sizeHints.y = 0;
		sizeHints.min_width = sizeHints.max_width = width;
		sizeHints.min_height = sizeHints.max_height = height;
		::XSetNormalHints(display_, window_, &sizeHints);

		::XClearWindow(display_, window_);
		::XMapRaised(display_, window_);
		::XFlush(display_);


		// create (image) buffer

		try
		{
			buffer_.resize(width * height * bytesPerPixel);
		}
		catch(...)
		{
			close();
			return false;
		}

		gc_ = DefaultGC(display_, screen);
		image_ = ::XCreateImage(display_, CopyFromParent, displayDepth, ZPixmap, 0, 0,
			width, height, bitsPerPixel, width * bytesPerPixel);
#ifdef __LITTLE_ENDIAN__
		image_->byte_order = LSBFirst;
#else
		image_->byte_order = MSBFirst;
#endif
		if (!image_)
		{
			close();
			return false;
		}
		image_->data = &buffer_[0];
		

		// we have a winner!

		return true;
	}
	
	void close()
	{
		trueColorConverter_ = 0;
		floatingPointConverter_ = 0;

		buffer_.clear();
		if (image_)
		{
			image_->data = 0;
			XDestroyImage(image_);
		}
		image_ = 0;

		if (display_ && window_)
			XDestroyWindow(display_, window_);
		window_ = 0;
			
		if (display_)
			XCloseDisplay(display_);
		display_ = 0;

		DisplayAdapter::close();
	}

	bool update( const TrueColorPixel * trueColorPixels, const FloatingPointPixel * floatingPointPixels )
	{
		if (!display_ || !window_ || !image_)
			return false;

		const int w = width();
		const int h = height();
		const int size = w * h;

		if (trueColorPixels)
			trueColorConverter_->convert(trueColorPixels, image_->data, size);
		else if (floatingPointPixels)
			floatingPointConverter_->convert(floatingPointPixels, image_->data, size);
		else
			return false;

		::XPutImage(display_, window_, gc_, image_, 0, 0, 0, 0, w, h);
		::XFlush(display_);

		return true;
	}

protected:

	void defaults()
	{
		display_ = 0;
		window_ = 0;
		gc_ = 0;
		image_ = 0;
		buffer_.clear();
		trueColorConverter_ = 0;
		floatingPointConverter_ = 0;
	}

private:

	typedef std::vector<char> TBuffer;

	::Display* display_;
	::Window window_;
	::GC gc_;
	::XImage* image_;
	TBuffer buffer_;
	Converter* trueColorConverter_;
	Converter* floatingPointConverter_;
};

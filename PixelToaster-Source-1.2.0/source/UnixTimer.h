// Unix Timer (unimplemented)
// Copyright © Glenn Fiedler
// http://www.pixeltoaster.com

class UnixTimer : public TimerInterface
{
public:
	
	UnixTimer()
	{
		reset();
	}
	
	void reset()
	{
		// ...
	}
	
	double time()
	{
		return 0.0;
	}
	
	double delta()
	{
		return 0.0;
	}
	
	double resolution()
	{
		return 1.0 / 1000000.0;
	}
	
	void wait(double seconds)
	{
		// ...
	}
	
private:

	// ...
};

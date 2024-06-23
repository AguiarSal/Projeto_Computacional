#include "FPS.h"

FPS::FPS()
{
	QueryPerformanceFrequency(&freq);

	ZeroMemory(&start, sizeof(start));
	ZeroMemory(&end, sizeof(end));

	stop = false;
}

void FPS::Start()
{
	if (stop)
	{
		long long elapsed = end.QuadPart - start.QuadPart;

		QueryPerformanceCounter(&start);
		start.QuadPart -= elapsed;

		stop = false;
	}
	else
	{
		QueryPerformanceCounter(&start);
	}
}

void FPS::Stop()
{
	if (!stop)
	{
		QueryPerformanceCounter(&end);
		stop = true;
	}
}

float FPS::Reset()
{
	long long elapsed;

	if (stop)
	{
		elapsed = end.QuadPart - start.QuadPart;

		QueryPerformanceCounter(&start);

		stop = false;
	}
	else
	{
		QueryPerformanceCounter(&end);

		elapsed = end.QuadPart - start.QuadPart;

		start = end;
	}

	return float(elapsed / double(freq.QuadPart));
}

float FPS::Elapsed()
{
	long long elapsed;

	if (stop)
	{
		elapsed = end.QuadPart - start.QuadPart;
	}
	else
	{
		QueryPerformanceCounter(&end);

		elapsed = end.QuadPart - start.QuadPart;
	}

	return float(elapsed / double(freq.QuadPart));
}
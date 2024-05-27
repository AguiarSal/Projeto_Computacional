#ifndef FPS_H
#define FPS_H

#include <windows.h>

// apesar do nome, essa classe � respons�vel por nos ajudar
// a usar um contador de alta precis�o do windows.

class FPS
{
private:
	LARGE_INTEGER start, end;
	LARGE_INTEGER freq;
	bool stop;

public:
	FPS();

	void Start();
	void Stop();
	float Reset();
	float Elapsed();
	bool Elapsed(float Secs);
};

inline bool FPS::Elapsed(float Secs) {
	return (Elapsed() >= Secs ? true : false);
}

#endif
#pragma once
#include "GWin.h"
#include "AutoReleasePool.h"
#define W_HEIGHT 600
#define W_WIDTH  800
class GEngine {
public:
	static GEngine *getInstance();
private:
	static GEngine *_inst;
private:
	GEngine();
	~GEngine();
public:
	void run();
	void destroy();
	AutoReleasePool * getAutoReleasePool() { return _rPool; }
private:
	GWin *_gmWindows;
	AutoReleasePool * _rPool;
	GLuint64 _lastTickTime;
};
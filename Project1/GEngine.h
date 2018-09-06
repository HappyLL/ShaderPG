#pragma once
#include "GWin.h"
#include "AutoReleasePool.h"
#include "GScheduler.h"
#include "GameLogic.h"
#include "GRender.h"
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
	GScheduler * _scheduler;
	GameLogic * _gmLogic;
	GRender * _grender;

	GLuint64 _lastTickTime;
};
#include "GEngine.h"
GEngine* GEngine::_inst = nullptr;
#include <sys/timeb.h>

#define ONE_TICK_TIME 20

GEngine *GEngine::getInstance() {
	if (_inst)
		return _inst;
	_inst = new GEngine();
	return _inst;
}

GEngine::GEngine()
{
	_gmWindows = new GWin(W_WIDTH, W_HEIGHT);
	_rPool = new AutoReleasePool();
	_scheduler = new GScheduler();
	_gmLogic = new GameLogic();
	_grender = new GRender();
}

GEngine::~GEngine()
{
	delete _gmWindows;
	_gmWindows = nullptr;
	delete _rPool;
	_rPool = nullptr;
	_scheduler->release();
	_scheduler = nullptr;
	_gmLogic->release();
	_gmLogic = nullptr;
	_grender->release();
	_grender = nullptr;
}

void GEngine::run()
{
	struct timeb nowTime;
	ftime(&nowTime);
	_lastTickTime = __int64(nowTime.time * 1000) + nowTime.millitm;
	GLuint64 nowTickTime = 0;
	double delta_time;
	while (!_gmWindows->gWinShouldClose()) {
		ftime(&nowTime);
		nowTickTime = __int64(nowTime.time * 1000) + nowTime.millitm;
		delta_time = (nowTickTime - _lastTickTime) * 1.0 / 1000;
		if (nowTickTime - _lastTickTime >= ONE_TICK_TIME) {
			_scheduler->update(delta_time);
			_grender->render();
			_rPool->clearGRefs();
			_gmWindows->gWinPollEvents();
			_lastTickTime = nowTickTime - nowTickTime % ONE_TICK_TIME; //��֤_lastTickTime��oneTicktime�ı���
			continue;
		}
		Sleep(1);
	}
}

void GEngine::destroy()
{
	delete _inst;
	_inst = nullptr;
}

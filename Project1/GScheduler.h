#pragma once
#include "GRef.h"
#include <functional>
#include <vector>
typedef std::function<void(double dt)> SECHDULE_FUNC;
#define hashpointer(pointer) (size_t)(pointer)

class GTimer : public GRef {
public:
	GTimer(SECHDULE_FUNC func);
	~GTimer();
public:
	void trigger(double dt);
	bool isEqual(SECHDULE_FUNC func);
private:
	SECHDULE_FUNC _func;
};

class GTimerCallBack : public GRef {
public:
	GTimerCallBack(void *target);
	~GTimerCallBack();
public:
	void update(double dt);
	void *getTarget() { return _target; }
	bool isEqual(void *target) { return _target == target; }
	int getNext() { return _next; }
	void setNext(int next) { _next = next; }
	void pushFunc(SECHDULE_FUNC func);
	void removeFunc(SECHDULE_FUNC func);
	bool emptyFuncs() { return _timers.empty(); }
private:
	std::vector<GTimer*> _timers;
	void *_target;
	int _next;
};

class GTimerHTable : public GRef {
public:
	GTimerHTable();
	~GTimerHTable();
public:
	GTimerCallBack *getElement(void *target);
	int _getFreeKey();
	void updateAllElements(double dt);
	void addElement(void *target, GTimerCallBack * element);
	void removeElement(void *target);
private:
	void _rehash();
	int _getHashKey(void *target);
private:
	size_t _tsize;
	GTimerCallBack ** _htable;
	size_t _lastFreeIndex;
};

class GScheduler : public GRef {
public:
	GScheduler();
	~GScheduler();
public:
	void gscheduler(SECHDULE_FUNC func, void *target);
	void ungscheduler(void *target, SECHDULE_FUNC func);
	void ungscheduler(void *target);
	void update(double dt);
private:
	GTimerHTable * _gtable;
};
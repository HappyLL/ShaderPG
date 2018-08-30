#pragma once
#include "GRef.h"
#include <functional>
#include <vector>
typedef std::function<void(double dt)> SECHDULE_FUNC;
#define hashpointer(pointer) (size_t)(pointer)

class GTimerCallBack;
class GDelayNode;

class GProrityNode {
public:
	GProrityNode();
	~GProrityNode();
public:
	GProrityNode *next;
	GProrityNode *pre;
	GTimerCallBack * timer;
	int priority;
};

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
	GProrityNode *getPriorityNode() { return _prority_node; }
private:
	std::vector<GTimer*> _timers;
	void *_target;
	int _next;
	GProrityNode *_prority_node;
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

class GDelayNode {
public:
	void *target;
	SECHDULE_FUNC func;
	int priority;
	int op;
};

class GScheduler : public GRef {
public:
	GScheduler();
	~GScheduler();
public:
	void gscheduler(SECHDULE_FUNC func, void *target, int priority);
	void ungscheduler(void *target, SECHDULE_FUNC func);
	void ungscheduler(void *target);
	void update(double dt);
private:
	void _priorityIn(GTimerCallBack *timer, int priority, GProrityNode * pList);
	void _priorityOut(GTimerCallBack *time);
	void _delList(GProrityNode *pList);
	void _pushDelayNode(void *target=nullptr, SECHDULE_FUNC func=nullptr, int priority=1, int op=1);
	void _addDelayElements();
	GProrityNode* _gPList(int priority);
private:
	GTimerHTable * _gtable;
	bool _updating;
	GProrityNode * _negPriorityList;
	GProrityNode * _zeroPriorityList;
	GProrityNode * _posPriorityList;
	std::vector<GDelayNode> _opDelayNodes;
};
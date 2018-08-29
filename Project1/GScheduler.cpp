#include "GScheduler.h"
#include <iostream>

GScheduler::GScheduler()
{
	_gtable = new GTimerHTable();
}

GScheduler::~GScheduler()
{
	_gtable->release();
	_gtable = nullptr;
}

void GScheduler::gscheduler(SECHDULE_FUNC func, void * target)
{
	GTimerCallBack * timer_cb = _gtable->getElement(target);
	if (timer_cb) {
		timer_cb->pushFunc(func);
		return;
	}
	timer_cb = new GTimerCallBack(target);
	timer_cb->pushFunc(func);
	_gtable->addElement(target, timer_cb);
}

void GScheduler::ungscheduler(void * target, SECHDULE_FUNC func)
{
	GTimerCallBack * timer_cb = _gtable->getElement(target);
	if (!timer_cb) {
		return;
	}
	timer_cb->removeFunc(func);
	if (timer_cb->emptyFuncs())
		_gtable->removeElement(target);
}

void GScheduler::ungscheduler(void * target)
{
	GTimerCallBack * timer_cb = _gtable->getElement(target);
	if (!timer_cb)
		return;
	_gtable->removeElement(target);
}

void GScheduler::update(double dt)
{
	_gtable->updateAllElements(dt);
}

GTimerCallBack::GTimerCallBack(void *target)
{
	_target = target;
	_next = -1;
}

GTimerCallBack::~GTimerCallBack()
{
	for (auto itr = _timers.begin(); itr != _timers.end(); ++itr)
		(*itr)->release();
	_timers.clear();
	_target = nullptr;
	_next = -1;
}

void GTimerCallBack::update(double dt)
{
	for (auto itr = _timers.begin(); itr != _timers.end(); ++itr) {
		(*itr)->trigger(dt);
	}
}

void GTimerCallBack::pushFunc(SECHDULE_FUNC func)
{
	GTimer* timer = new GTimer(func);
	_timers.push_back(timer);
}

void GTimerCallBack::removeFunc(SECHDULE_FUNC func)
{
	for (auto itr = _timers.begin(); itr != _timers.end(); ++itr) {
		if ((*itr)->isEqual(func)) {
			_timers.erase(itr);
			break;
		}
	}
}

GTimerHTable::GTimerHTable()
{
	_tsize = 2;
	_htable = new GTimerCallBack*[_tsize];
	for (int i = 0; i < _tsize; ++i)
		_htable[i] = nullptr;
	_lastFreeIndex = 0;
}

GTimerHTable::~GTimerHTable()
{
	delete []_htable;
}

void GTimerHTable::addElement(void* target, GTimerCallBack * element)
{
	_rehash();
	size_t hashkey = hashpointer(target) % _tsize;
	if (_htable[hashkey] != nullptr) {
		// 说明有冲突
		size_t _thashkey = hashpointer(_htable[hashkey]->getTarget()) % _tsize;
		if (_thashkey != hashkey) {
			// 说明目标的hashkey也是冲突节点
			GTimerCallBack *_tElement = _htable[_thashkey];
			int free_key = _getFreeKey();
			_htable[free_key] = _htable[hashkey];
			_htable[hashkey] = element;
			while (_tElement->getNext() != hashkey) {
				_tElement = _htable[_tElement->getNext()];
			}
			_tElement->setNext(free_key);
		}
		else {
			// hash键一致是解决冲突方法
			int free_key = _getFreeKey();
			free_key = _getFreeKey();
			_htable[free_key] = element;
			size_t nx_key = _htable[hashkey]->getNext();
			_htable[hashkey]->setNext(free_key);
			element->setNext(nx_key);
		}
		return;
	}
	_htable[hashkey] = element;
}

void GTimerHTable::removeElement(void *target)
{
	int hashkey = hashpointer(target) % _tsize;
	int pre = -1;
	int cur = hashkey;
	while (cur != -1) {
		if (_htable[cur]->isEqual(target)) {
			if (pre != -1) {
				_htable[pre]->setNext(_htable[cur]->getNext());
				_htable[cur]->release();
				_htable[cur] = nullptr;
			}
			else {
				int next = _htable[cur]->getNext();
				if (next != -1) {
					_htable[cur]->release();
					_htable[cur] = _htable[next];
					_htable[next] = nullptr;
				}
				else {
					_htable[cur]->release();
					_htable[cur] = nullptr;
				}
			}
			break;
		}
		pre = cur;
		cur = _htable[cur]->getNext();
	}
	_rehash();
}

GTimerCallBack * GTimerHTable::getElement(void * target)
{
	int hashkey = _getHashKey(target);
	if (hashkey != -1)
		return _htable[hashkey];
	return nullptr;
}

int GTimerHTable::_getFreeKey()
{
	for (; _lastFreeIndex < _tsize; ++_lastFreeIndex)
		if (!_htable[_lastFreeIndex])
			return _lastFreeIndex;
	return -1;
}

void GTimerHTable::updateAllElements(double dt)
{
	for (int index = 0; index < _tsize; ++index) {
		if (!_htable[index])
			continue;
		_htable[index]->update(dt);
	}
}

void GTimerHTable::_rehash()
{
	int ne_num = 0;
	for (int index = 0; index < _tsize; ++index) {
		if (_htable[index])
			ne_num++;
	}
	if ((ne_num << 1) <= _tsize && (ne_num >= (_tsize >> 1)))
		return;
	int new_size = 1;
	if ((ne_num << 1) > _tsize)
		new_size = _tsize >> 1;
	else
		new_size = _tsize << 1;
	std::cout << "GTimerHandle _rehash size is " << new_size<<std::endl;
	GTimerCallBack ** newHtable = new GTimerCallBack*[new_size];
	for (int index = 0; index < new_size; ++index)
		newHtable[index] = nullptr;
	GTimerCallBack ** oldHTable = _htable;
	_htable = newHtable;
	_lastFreeIndex = 0;
	_tsize = new_size;
	for (int index = 0; index < new_size; ++index) {
		if (!oldHTable[index])
			continue;
		addElement(oldHTable[index]->getTarget(), oldHTable[index]);
	}
	delete[]oldHTable;
}

int GTimerHTable::_getHashKey(void * target)
{
	size_t hashkey = hashpointer(target) % _tsize;
	while (hashkey != -1) {
		if (_htable[hashkey]->isEqual(target))
			return hashkey;
		hashkey = _htable[hashkey]->getNext();
	}
	return -1;
}

GTimer::GTimer(SECHDULE_FUNC func)
{
	_func = func;
}

GTimer::~GTimer()
{
	_func = nullptr;
}

void GTimer::trigger(double dt)
{
	_func(dt);
}

bool GTimer::isEqual(SECHDULE_FUNC func)
{
	return (&_func) == (&func);
}

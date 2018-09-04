#include "GScheduler.h"
#include <iostream>
#include <cmath>

GScheduler::GScheduler()
{
	_gtable = new GTimerHTable();
	_updating = false;
	_negPriorityList = new GProrityNode;
	_posPriorityList = new GProrityNode;
	_zeroPriorityList = new GProrityNode;
}

GScheduler::~GScheduler()
{
	_gtable->release();
	_gtable = nullptr;
	_updating = false;
	_delList(_negPriorityList);
	_delList(_posPriorityList);
	_delList(_zeroPriorityList);
	_negPriorityList = nullptr;
	_posPriorityList = nullptr;
	_zeroPriorityList = nullptr;
}

void GScheduler::gscheduler(SECHDULE_FUNC func, void * target, int priority)
{
	if (!_updating) {
		GTimerCallBack * timer_cb = _gtable->getElement(target);
		if (timer_cb) {
			timer_cb->pushFunc(func);
			_priorityIn(timer_cb, priority, _gPList(priority));
			return;
		}
		timer_cb = new GTimerCallBack(target);
		timer_cb->pushFunc(func);
		_gtable->addElement(target, timer_cb);
		_priorityIn(timer_cb, priority, _gPList(priority));
		return;
	}
	_pushDelayNode(target, func, priority);
}

void GScheduler::ungscheduler(void * target, SECHDULE_FUNC func)
{
	if (!_updating) {
		GTimerCallBack * timer_cb = _gtable->getElement(target);
		if (!timer_cb) {
			return;
		}
		timer_cb->removeFunc(func);
		if (timer_cb->emptyFuncs()) {
			_priorityOut(timer_cb);
			_gtable->removeElement(target);
		}
		return;
	}
	_pushDelayNode(target, func, 1, 2);
}

void GScheduler::ungscheduler(void * target)
{
	if (!_updating) {
		GTimerCallBack * timer_cb = _gtable->getElement(target);
		if (!timer_cb)
			return;
		_priorityOut(timer_cb);
		_gtable->removeElement(target);
		return;
	}
	_pushDelayNode(target, nullptr, 1, 2);
}

void GScheduler::update(double dt)
{
	_updating = true;
	_gtable->updateAllElements(dt);
	_updating = false;
	_addDelayElements();
}

void GScheduler::_priorityIn(GTimerCallBack * timer, int priority, GProrityNode *pList)
{
	GProrityNode* _pnode = timer->getPriorityNode();
	if (!_pnode) {
		_pnode = new GProrityNode();
		_pnode->timer = timer;
		_pnode->next = pList->next;
		if(pList->next)
			pList->next->pre = _pnode;
		pList->next = _pnode;
		_pnode->pre = pList;
	}
	_pnode->priority = priority;
	while (true) {
		auto pre_node = _pnode->pre;
		if (!pre_node->pre)
			break;
		if (pre_node->priority > _pnode->priority) {
			pre_node->pre->next = _pnode;
			_pnode->pre = pre_node->pre;
			pre_node->next = _pnode->next;
			if (_pnode->next)
				_pnode->next->pre = pre_node;
			pre_node->pre = _pnode;
			_pnode->next = pre_node;
		}
		else
			break;
	}
	while (true) {
		auto next_node = _pnode->next;
		if (!next_node)
			break;
		if (_pnode->priority > next_node->priority) {
			_pnode->pre->next = next_node;
			next_node->pre = _pnode->pre;
			_pnode->next = next_node->next;
			if (next_node->next)
				next_node->next->pre = _pnode;
			_pnode->pre = next_node;
		}
		else
			break;
	}

}

void GScheduler::_priorityOut(GTimerCallBack * timer)
{
	auto _priorityNode = timer->getPriorityNode();
	_priorityNode->pre->next = _priorityNode->next;
	if (_priorityNode->next)
		_priorityNode->next->pre = _priorityNode->pre;
	delete _priorityNode;
}

void GScheduler::_delList(GProrityNode * pList)
{
	GProrityNode *cur = pList;
	GProrityNode * next = nullptr;
	while (cur) {
		next = cur->next;
		delete cur;
		cur = next;
	}
}

void GScheduler::_pushDelayNode(void * target, SECHDULE_FUNC func, int priority, int op)
{
	GDelayNode delayNode;
	delayNode.target = target;
	delayNode.func = func;
	delayNode.priority = priority;
	delayNode.op = op;
	_opDelayNodes.push_back(delayNode);
}

void GScheduler::_addDelayElements()
{
	for (auto delayNode : _opDelayNodes) {
		if (delayNode.op == 1) {
			gscheduler(delayNode.func, delayNode.target, delayNode.priority);
		}
		else if(delayNode.func){
			ungscheduler(delayNode.target, delayNode.func);
		}
		else
			ungscheduler(delayNode.target);
	}
	_opDelayNodes.clear();
}

GProrityNode* GScheduler::_gPList(int priority)
{
	if (priority < 0)
		return _negPriorityList;
	if (priority == 0)
		return _zeroPriorityList;
	return _posPriorityList;
}

GTimerCallBack::GTimerCallBack(void *target)
{
	_target = target;
	_next = -1;
	_prority_node = nullptr;
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
	_tsize = 1;
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
	_addElement(target, element);
}

void GTimerHTable::_addElement(void* target, GTimerCallBack *element){
	size_t hashkey = _hposition(target);
	if (_htable[hashkey] != nullptr) {
		// ˵���г�ͻ
		size_t _thashkey = _hposition(_htable[hashkey]->getTarget());
		//std::cout << "new hash key is " << _thashkey << std::endl;
		if (_thashkey != hashkey) {
			// ˵��Ŀ���hashkeyҲ�ǳ�ͻ�ڵ�
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
			// hash��һ���ǽ����ͻ����
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
	int hashkey = _hposition(target);
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
	int _sIndex = _lastFreeIndex;
	while (true) {
		if (!_htable[_sIndex]) {
			_lastFreeIndex = _sIndex;
			return _sIndex;
		}
		_sIndex = (_sIndex + 1) % _tsize;
		if (_sIndex == _lastFreeIndex)
			return -1;
	}
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
int cnt = 0;
int GTimerHTable::_hposition(void * target)
{
	int hpos = hashpointer(target) % (_tsize|1);
	if (hpos == _tsize)
		hpos >>= 1;
	return hpos;
}
void GTimerHTable::_rehash()
{
	int ne_num = 0;
	int max_ne_num = 0;
	for (int index = 0; index < _tsize; ++index) {
		if (_htable[index]) {
			ne_num++;
		}
		else {
			max_ne_num = max_ne_num < ne_num ? ne_num : max_ne_num;
			ne_num = 0;
		}
	}
	max_ne_num = max_ne_num < ne_num ? ne_num : max_ne_num;
	if ((max_ne_num << 1)<= _tsize && (_tsize - max_ne_num >= (_tsize >> 1)))
		return;
	int new_size = 1;
	if ((max_ne_num << 1) > _tsize)
		new_size = (_tsize << 1);
	else
		new_size = _tsize >> 1;
	cnt++;
	std::cout << "GTimerHandle _rehash size is " << new_size <<" "<< cnt <<std::endl;
	GTimerCallBack ** newHtable = new GTimerCallBack*[new_size];
	for (int index = 0; index < new_size; ++index)
		newHtable[index] = nullptr;
	GTimerCallBack ** oldHTable = _htable;
	_htable = newHtable;
	_lastFreeIndex = 0;
	int o_size = _tsize;
	_tsize = new_size;
	for (int index = 0; index < o_size; ++index) {
		if (!oldHTable[index])
			continue;
		oldHTable[index]->setNext(-1);
		_addElement(oldHTable[index]->getTarget(), oldHTable[index]);
	}
	delete[]oldHTable;
}

int GTimerHTable::_getHashKey(void * target)
{
	int hashkey = _hposition(target);
	while (hashkey != -1) {
		if (!_htable[hashkey])
			return -1;
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

GProrityNode::GProrityNode()
{
	next = nullptr;
	timer = nullptr;
	pre = nullptr;
}

GProrityNode::~GProrityNode()
{
	next = nullptr;
	timer = nullptr;
}

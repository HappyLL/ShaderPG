#pragma once
class GRef {
public:
	GRef();
	virtual ~GRef();
public:
	unsigned int getReferenceCount();
	void release();
	void autoRelease();
	void retain();
private:
	unsigned int _refcount;
};
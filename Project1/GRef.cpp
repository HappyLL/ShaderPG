#include "GRef.h"
#include "GEngine.h"

GRef::GRef()
{
	_refcount = 1;
}

GRef::~GRef()
{
}

unsigned int GRef::getReferenceCount()
{
	return _refcount;
}

void GRef::release()
{
	--_refcount;
	if (_refcount > 0)
		return;
	delete this;
}

void GRef::autoRelease()
{
	GEngine::getInstance()->getAutoReleasePool()->addGRef(this);
}

void GRef::retain()
{
	++_refcount;
}

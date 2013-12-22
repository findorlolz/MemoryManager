#include "memcontainer.h"

bool MemContainer::isRoom(size_t size) const
{
	if(size <= spaceLeft_)
		return true;
	return false;
}

bool MemContainer::isEmpty() const
{
	return (cursor_ == begin_);
}
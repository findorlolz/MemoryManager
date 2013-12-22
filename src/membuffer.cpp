#include "membuffer.h"

void MemBuffer::startUp()
{
	unsigned char* ptr = reinterpret_cast<unsigned char*>(malloc(totalSpace_ + sizeof(char*)));
	spaceLeft_ = totalSpace_;
	begin_ = ptr;
	cursor_ = ptr;
	type_ = BUFFER;
	state_ = READY;
}

void MemBuffer::shutDown()
{
	free(begin_);
	begin_ = nullptr;
	cursor_ = nullptr;
	state_ = BROKEN;
}

void MemBuffer::clear()
{
	cursor_ = begin_;
	spaceLeft_ = totalSpace_;
}

unsigned char* MemBuffer::pushBuffer(size_t size)
{
	if(!isRoom(size))
		return nullptr;
	spaceLeft_ -= size;
	unsigned char* address = cursor_;
	cursor_ += size;
	return address;
}
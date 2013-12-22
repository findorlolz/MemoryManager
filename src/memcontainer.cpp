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

void MemStack::startUp()
{
	unsigned char* ptr = reinterpret_cast<unsigned char*>(malloc(totalSpace_ + sizeof(char*)));
	spaceLeft_ = totalSpace_;
	begin_ = ptr;
	cursor_ = ptr;
	type_ = STACK;
	state_ = READY;
}


void MemStack::shutDown()
{
	free(begin_);
	begin_= nullptr;
	cursor_ = nullptr;
	state_ = BROKEN;
}

unsigned char* MemStack::pushStack(size_t size)
{
	if(!isRoom(size))
		return nullptr;
	spaceLeft_ -= size; 
	unsigned char* address = cursor_;
	cursor_ += size;
	unsigned char** tmp = reinterpret_cast<unsigned char**>(cursor_);
	tmp[0] = address;
	return address;
}

void MemStack::popStack()
{
	if(isEmpty())
		return;
	unsigned char** tmp = reinterpret_cast<unsigned char**>(cursor_);
	unsigned char* address = tmp[0];
	size_t size = reinterpret_cast<size_t>(cursor_) - reinterpret_cast<size_t>(address);
	spaceLeft_ += size;
	cursor_ = address;
}

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
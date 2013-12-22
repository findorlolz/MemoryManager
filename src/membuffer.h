#pragma once

#include "memcontainer.h"

class MemBuffer : public MemContainer
{
public:
	explicit MemBuffer(size_t size) : MemContainer(size) {}
	~MemBuffer() {}

	//Virtuals
	void startUp();
	void shutDown();
	unsigned char* alloc(size_t size) { return pushBuffer(size); }

	//Specific
	void clear();

private:
	unsigned char* pushBuffer(size_t);

	//RO3
	MemBuffer& operator=(MemBuffer& other);
	MemBuffer(MemBuffer& other);
};
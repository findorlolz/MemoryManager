#pragma once

#include "memcontainer.h"

class MemStack : public MemContainer
{
public:
	explicit MemStack(size_t size) : MemContainer(size) {}
	~MemStack() {}

	void startUp();
	void shutDown();
	unsigned char* alloc(size_t size) { return pushStack(size); }

	void popStack();
private:
	unsigned char* pushStack(size_t);

	//RO3
	MemStack& operator=(MemStack& other);
	MemStack(MemStack& other);
};
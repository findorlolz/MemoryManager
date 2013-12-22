#pragma once

#include "memcontainer.h"

#include <vector>
#include <iostream>

/*Memory mangager, singleton. Acts as overlay for different memorytypes
Niklas Smal Oct. 2013*/

class MemoryManager
{
public:

	MemoryManager(){}
	~MemoryManager(){}

	static MemoryManager& get()
	{
		static MemoryManager* gpSingleton = nullptr;
		if (gpSingleton == nullptr)
		{
			gpSingleton = new MemoryManager();
		}
		assert(gpSingleton != nullptr && "Failed to create MemoryManager");
		return *gpSingleton;
	}

	void startUp()
	{
		maxContainers_ = 0;
		largerIndexList(100);
	}

	void shutDown()
	{
		for(auto iter : containers_)
		{
			iter->shutDown();
			delete iter;
		}
		delete &(this->get());
	}
	
	size_t initAllocation(memContainerType type, size_t size)
	{
		size_t index = indexStack_.back();
		indexStack_.pop_back();
		if(type == BUFFER)
		{
			MemBuffer* buffer = new MemBuffer(size);
			buffer->startUp();
			containers_.push_back(buffer);
		}
		if(type == STACK)
		{
			MemBuffer* stack = new MemBuffer(size);
			stack->startUp();
			containers_.push_back(stack);
		}
		return index;
	}

	template<typename T>
	T* alloc(size_t index)
	{
		return ptr<T>(containers_[index]->alloc(sizeof(T)));
	}

	MemContainer& operator[](size_t index)
	{
		return *containers_[index];
	}

private:
	size_t maxContainers_;
	std::vector<MemContainer*> containers_;
	std::vector<size_t> indexStack_;

	void largerIndexList(size_t t)
	{
		size_t tmp = maxContainers_;
		maxContainers_ += t;
		indexStack_.reserve(t);
		for(size_t i = tmp; i <= maxContainers_; i++)
		{
			indexStack_.push_back(maxContainers_ - i);
		}
	}
};
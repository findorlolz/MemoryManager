#include "memorymanager.h"

void MemoryManager::startUpCall(MemContainerVersion v)
{
		version_ = v;
		maxContainers_ = 0;
		largerIndexList(1000);
}

void MemoryManager::shutDownCall()
{
	for(auto i = 0u; i < indexStack_.back(); ++i)
	{
		containers_[i]->shutDown();
		delete containers_[i];
	}
	indexStack_.clear();
	delete[] containers_;
	delete &(this->get());
}

MemContainer& MemoryManager::operator[](size_t index)
{
	return *containers_[index];
}

size_t MemoryManager::initAllocationCall(MemContainerType type, const size_t size, const size_t numOfBlocks)
{
	size_t index = indexStack_.back();
	indexStack_.pop_back();
		
	if(type == MemContainerType_BUFFER)
	{
		MemBuffer* buffer = new MemBuffer(size);
		buffer->startUp();
		containers_[index] = buffer;
	}
	else if(type == MemContainerType_STACK)
	{
		MemStack* stack = new MemStack(size);
		stack->startUp();
		containers_[index] = stack;
	}
	else if(type == MemContainerType_POOL)
	{
		MemPool* pool = new MemPool(size, numOfBlocks);
		pool->startUp(size, numOfBlocks);
		containers_[index] = pool;
	}

	if(indexStack_.empty())
		largerIndexList(100);

	return index;
}

void MemoryManager::largerIndexList(size_t t)
{
	size_t tmp = maxContainers_;
	maxContainers_ += t;
	indexStack_.reserve(t);
	MemContainer** newConteinersArray = new MemContainer*[maxContainers_];
	MemContainer** oldConteinersArray = containers_;
	for(auto i = 0u; i < tmp; ++i)
	{
		newConteinersArray[i] = containers_[i];
	}
	containers_ = newConteinersArray;
	delete[] oldConteinersArray;

	for(size_t i = 0; i < t; i++)
	{
		indexStack_.push_back(maxContainers_ - (i+1));
	}
}
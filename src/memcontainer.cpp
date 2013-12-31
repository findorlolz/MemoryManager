#include "memorymanager.h"

inline bool MemContainer::isRoom(size_t size) const
{
	if(size <= spaceLeft_)
		return true;
	return false;
}

inline bool MemContainer::isEmpty() const
{
	return (cursor_ == begin_);
}

void MemContainer::startUp()
{
	if(MemoryManager::getVersion() == MemContainerVersion_DEBUG)
		assert("ERROR - StartUp for abstract memory container");
}

void MemContainer::shutDown()
{
	if(MemoryManager::getVersion() == MemContainerVersion_DEBUG)
		assert("ERROR - StartUp for abstract memory container");
}

unsigned char* MemContainer::alloc(size_t)
{
	if(MemoryManager::getVersion() == MemContainerVersion_DEBUG)
		assert("ERROR - Allocation called for abstract memory container");
	return nullptr;
}

void MemStack::startUp()
{
	unsigned char* ptr = reinterpret_cast<unsigned char*>(malloc(totalSpace_ + sizeof(char*)));
	spaceLeft_ = totalSpace_;
	begin_ = ptr;
	cursor_ = ptr;
	state_ = MemContainerState_READY;
}


void MemStack::shutDown()
{
	free(begin_);
	begin_= nullptr;
	cursor_ = nullptr;
	state_ = MemContainerState_BROKEN;
}

unsigned char* MemStack::pushStack(size_t size)
{
	if(MemoryManager::getVersion() == MemContainerVersion_DEBUG)
		assert(state_ == MemContainerState_READY && "ERROR - pushStack from invalid stack ");
	
	if(!isRoom(size))
	{
		if(MemoryManager::getVersion() == MemContainerVersion_DEBUG)
		{
			assert(state_ == MemContainerState_READY && "ERROR - Allocating from invalid memory stack");
			assert(cursor_ != nullptr && "ERROR - Allocating from full memory stack");	
		}
		return nullptr;
	}
	spaceLeft_ -= size; 
	unsigned char* address = cursor_;
	cursor_ += size;
	unsigned char** tmp = reinterpret_cast<unsigned char**>(cursor_);
	tmp[0] = address;
	return address;
}

void MemStack::popStack()
{
	if(MemoryManager::getVersion() == MemContainerVersion_DEBUG)
		assert(state_ == MemContainerState_READY && "ERROR - popStack from invalid stack");
	
	if(isEmpty())
	{
		if(MemoryManager::getVersion() == MemContainerVersion_DEBUG)
			assert(isEmpty() && "ERROR - popStack from empty stack");	
		else
			return;
	}
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
	state_ = MemContainerState_READY;
}

void MemBuffer::shutDown()
{
	free(begin_);
	begin_ = nullptr;
	cursor_ = nullptr;
	state_ = MemContainerState_BROKEN;
}

void MemBuffer::clear()
{
	cursor_ = begin_;
	spaceLeft_ = totalSpace_;
}

unsigned char* MemBuffer::pushBuffer(size_t size)
{
	if(MemoryManager::getVersion() == MemContainerVersion_DEBUG)
		assert(state_ == MemContainerState_READY && "ERROR - pushBuffer from invalid buffer");
	
	if(!isRoom(size))
	{
		if(MemoryManager::getVersion() == MemContainerVersion_DEBUG)
		{
			assert(state_ == MemContainerState_READY && "ERROR - Allocating from invalid memory buffer");
			assert(cursor_ != nullptr && "ERROR - Allocating from full memory buffer");	
		}
		return nullptr;
	}
	spaceLeft_ -= size;
	unsigned char* address = cursor_;
	cursor_ += size;
	return address;
}

void MemPool::startUp(const size_t sizeOfBlock, const size_t numberOfBlocks)
{
	unsigned char* ptr = reinterpret_cast<unsigned char*>(malloc(totalSpace_));
	begin_ = ptr;
	spaceLeft_ = totalSpace_;
	cursor_ = ptr;
	state_ = MemContainerState_READY;

	size_t nextAddress;
	size_t currentAddress = reinterpret_cast<size_t>(ptr);
	for(auto i = 0u; i < numberOfBlocks - 1; ++i)
	{
		nextAddress = currentAddress + sizeOfBlock;
		unsigned char** tmp = reinterpret_cast<unsigned char**>(currentAddress);
		tmp[0] = reinterpret_cast<unsigned char*>(nextAddress);
		currentAddress = nextAddress;
	}

	lastMemberOfPool_ = reinterpret_cast<unsigned char*>(currentAddress);
	unsigned char** tmp = reinterpret_cast<unsigned char**>(currentAddress);
	tmp[0] = nullptr;

}

void MemPool::shutDown()
{	
	free(begin_);
	begin_= nullptr;
	cursor_ = nullptr;
	state_ = MemContainerState_BROKEN;
}

unsigned char* MemPool::alloc(size_t size)
{
	if(isRoom(blockSize_))
	{
		if(MemoryManager::getVersion() == MemContainerVersion_DEBUG)
		{
			assert(state_ == MemContainerState_READY && "ERROR - Allocating from invalid memory pool");
			assert(cursor_ != nullptr && "ERROR - Allocating from full memory pool");	
		}
		return nullptr;
	}

	unsigned char* r = cursor_;
	unsigned char** tmp = reinterpret_cast<unsigned char**>(cursor_);
	cursor_ = tmp[0];
	spaceLeft_ -= blockSize_;
	return r;
}

//TODO Erase member from pool
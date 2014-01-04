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

void MemContainer::release(unsigned char*)
{
	if(MemoryManager::getVersion() == MemContainerVersion_DEBUG)
		assert("ERROR - Releasing memory address from unvalid container");
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

	size_t totalBlock = (size+charSize);
	unsigned char* address = cursor_;
	unsigned char** tmp = reinterpret_cast<unsigned char**>(cursor_ + size);
	tmp[0] = address;
	cursor_ += totalBlock;
	spaceLeft_ -= totalBlock; 
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
	unsigned char** tmp = reinterpret_cast<unsigned char**>(cursor_ - charSize);
	unsigned char* address = tmp[0];
	size_t size = reinterpret_cast<size_t>(cursor_) - reinterpret_cast<size_t>(address);
	spaceLeft_ += size;
	cursor_ = address;
}

void MemBuffer::startUp()
{
	unsigned char* ptr = reinterpret_cast<unsigned char*>(malloc(totalSpace_ + charSize));
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
	if(!isRoom(blockSize_))
	{
		if(MemoryManager::getVersion() == MemContainerVersion_DEBUG)
		{
			assert(state_ == MemContainerState_READY && "ERROR - Allocating from invalid memory pool");
			assert(spaceLeft_ > size && "ERROR - Allocating from full memory pool");	
		}
		return nullptr;
	}

	if(MemoryManager::getVersion() == MemContainerVersion_DEBUG)
	{
		assert(cursor_ != nullptr && "ERROR - MemoryPool has been corrupted");
		assert(state_ == MemContainerState_READY && "ERROR - Allocating from invalid memory pool");
	}

	unsigned char* r = cursor_;
	unsigned char** tmp = reinterpret_cast<unsigned char**>(cursor_);
	cursor_ = tmp[0];
	spaceLeft_ -= blockSize_;
	return r;
}

void MemPool::releaseAddress(unsigned char* ptr)
{
	if(MemoryManager::getVersion() == MemContainerVersion_DEBUG)
	{
		size_t p = reinterpret_cast<size_t>(ptr);
		assert(state_ == MemContainerState_READY && "ERROR - Releasing from invalid memory pool");
		assert( p > reinterpret_cast<size_t>(begin_) && "ERRO - Releasing in memeroy pool from address that doesn't match malloced block");
		assert( p < (reinterpret_cast<size_t>(begin_)+totalSpace_) && "ERRO - Releasing in memeroy pool from address that doesn't match malloced block");	
	}

	if(cursor_ == nullptr)
		cursor_ = ptr;

	unsigned char** tmp = reinterpret_cast<unsigned char**>(lastMemberOfPool_);
	tmp[0] = ptr;
	tmp = reinterpret_cast<unsigned char**>(ptr);
	tmp[0] = nullptr;
	lastMemberOfPool_ = ptr;
	spaceLeft_ += blockSize_;
}

void YoloMemPool::startUp(const size_t sizeOfBlock, const size_t numberOfBlocks)
{
	unsigned char* ptr = reinterpret_cast<unsigned char*>(malloc(sizeOfBlock*numberOfBlocks));
	begin_ = ptr;
	cursor_ = ptr;

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

void YoloMemPool::shutDown()
{	
	free(begin_);
	begin_= nullptr;
	cursor_ = nullptr;
}

unsigned char* YoloMemPool::alloc()
{
	unsigned char* r = cursor_;
	unsigned char** tmp = reinterpret_cast<unsigned char**>(cursor_);
	cursor_ = tmp[0];
	return r;
}

void YoloMemPool::release(unsigned char* ptr)
{
	if(cursor_ == nullptr)
		cursor_ = ptr;

	unsigned char** tmp = reinterpret_cast<unsigned char**>(lastMemberOfPool_);
	tmp[0] = ptr;
	tmp = reinterpret_cast<unsigned char**>(ptr);
	tmp[0] = nullptr;
	lastMemberOfPool_ = ptr;
}

void YoloMemBuffer::startUp(const size_t s)
{
	unsigned char* ptr = reinterpret_cast<unsigned char*>(malloc(s));
	begin_ = ptr;
	cursor_ = ptr;
}

void YoloMemBuffer::shutDown()
{
	free(begin_);
	begin_ = nullptr;
	cursor_ = nullptr;
}

void YoloMemBuffer::clear()
{
	cursor_ = begin_;
}

unsigned char* YoloMemBuffer::alloc(size_t size)
{
	unsigned char* address = cursor_;
	cursor_ += size;
	return address;
}

void YoloMemStack::startUp(const size_t sizeOfBlock, const size_t numOfBlocks)
{
	unsigned char* ptr = reinterpret_cast<unsigned char*>(malloc(sizeOfBlock*numOfBlocks));
	blockSize_ = sizeOfBlock;
	begin_ = ptr;
	cursor_ = ptr;
}


void YoloMemStack::shutDown()
{
	free(begin_);
	begin_= nullptr;
	cursor_ = nullptr;
}

unsigned char* YoloMemStack::push()
{
	unsigned char* address = cursor_;
	cursor_ += blockSize_;
	return address;
}

void YoloMemStack::pop()
{
	cursor_ -= blockSize_;
}
#pragma once

/*This is abstract class for different memory stuctures
Niklas Smal Oct. 2013*/

#include <vector>
#include <iostream>
#include <cstdlib>
#include <cassert>

enum MemContainerType
{
	MemContainerType_STACK, 
	MemContainerType_POOL, 
	MemContainerType_BUFFER
};

enum MemContainerState
{
	MemContainerState_BROKEN, 
	MemContainerState_READY, 
	MemContainerState_FULL
};

enum MemContainerVersion
{
	MemContainerVersion_DEBUG,
	MemContainerVersion_RELEASE
};

class MemContainer
{
public:
	MemContainer() 
	{}
	MemContainer(const size_t size) : 
		totalSpace_(size),
		state_(MemContainerState_BROKEN)
	{}


	virtual ~MemContainer(){}

	virtual void startUp();
	virtual void shutDown();
	virtual unsigned char* alloc(size_t);
	virtual void release(unsigned char* = nullptr);

	bool isValid() const { return state_ == MemContainerState_READY; }

	//Accessories
	size_t getSpaceLeft() const { return spaceLeft_; }
	const size_t getTotalSpace() const { return totalSpace_; }

protected:
	MemContainerState state_;
	unsigned char* cursor_;
	size_t totalSpace_;
	size_t spaceLeft_;
	unsigned char* begin_;

	inline bool isRoom(size_t) const;
	inline bool isEmpty() const;

};

class MemStack : public MemContainer
{
public:
	explicit MemStack(const size_t size) : MemContainer(size) {}
	~MemStack() {}

	virtual void startUp();
	virtual void shutDown();
	virtual unsigned char* alloc(size_t size) { return pushStack(size); }
	virtual void release(unsigned char* = nullptr) { popStack(); }

private:
	void popStack();
	unsigned char* pushStack(size_t);

	//RO3
	MemStack& operator=(MemStack& other);
	MemStack(MemStack& other);
};

class MemBuffer : public MemContainer
{
public:
	explicit MemBuffer(const size_t size) : MemContainer(size) {}
	~MemBuffer() {}

	//Virtuals
	virtual void startUp();
	virtual void shutDown();
	virtual unsigned char* alloc(size_t size) { return pushBuffer(size); }
	virtual void release(unsigned char* = nullptr) { clear(); }

private:
	void clear();
	unsigned char* pushBuffer(size_t);

	//RO3
	MemBuffer& operator=(MemBuffer& other);
	MemBuffer(MemBuffer& other);
};

class MemPool : public MemContainer
{
public:
	explicit MemPool(const size_t sizeOfBlock, const size_t numberOfBlocks) : 
	MemContainer(sizeOfBlock*numberOfBlocks),
		blockSize_(sizeOfBlock)
	{}
	~MemPool() {}

	virtual void startUp(const size_t sizeOfBlock, const size_t numberOfBlocks);
	virtual void shutDown();
	virtual unsigned char* alloc(size_t size);
	virtual void release(unsigned char* ptr = nullptr) { releaseAddress(ptr); }


private:
	size_t blockSize_;
	unsigned char* lastMemberOfPool_;
	void releaseAddress(unsigned char*);

	//RO3
	MemPool& operator=(MemPool& other);
	MemPool(MemBuffer& other);
};

class YoloMemPool
{
public:
	YoloMemPool()
	{}
	~YoloMemPool() {}

	void startUp(const size_t, const size_t);
	void shutDown();
	unsigned char* alloc();
	void release(unsigned char*);


private:
	size_t blockSize_;
	unsigned char* lastMemberOfPool_;
	unsigned char* begin_;
	unsigned char* cursor_;

	//RO3
	YoloMemPool& operator=(YoloMemPool& other);
	YoloMemPool(YoloMemPool& other);
};

class YoloMemBuffer
{
public:
	YoloMemBuffer()
	{}
	~YoloMemBuffer() {}

	void startUp(const size_t);
	void shutDown();
	unsigned char* alloc(size_t size);
	void clear();

private:
	unsigned char* begin_;
	unsigned char* cursor_;

	//RO3
	YoloMemBuffer& operator=(YoloMemBuffer& other);
	YoloMemBuffer(YoloMemBuffer& other);
};
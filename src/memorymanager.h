#pragma once

#include "memcontainer.h"

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

	static void startUp(MemContainerVersion v = MemContainerVersion_DEBUG) { get().startUpCall(v); }
	static void shutDown() { get().shutDownCall(); }
	static size_t initAllocation(MemContainerType type, const size_t size, const size_t numOfBlocks = 1){ return get().initAllocationCall(type, size, numOfBlocks); }
	static MemContainerVersion getVersion() { return get().getVersionCall(); }
	static void setVersion(MemContainerVersion v) { get().setVersionCall(v);}

	template<typename T>
	static T* alloc(size_t index) { return get().allocCall<T>(index); }

	/*
	This is a release interface for used memory types. It's function depends on the type of container in question:
	-For stack, it pops the last element of the stack
	-For buffer, entire buffer is cleared
	-For 
	*/
	template<typename T>
	static void release(size_t index, T* ptr = nullptr) { get().releaseCall(index, reinterpret_cast<unsigned char*>(ptr)); }


private:
	size_t maxContainers_;
	MemContainerVersion version_;
	MemContainer** containers_;
	std::vector<size_t> indexStack_;

	MemContainerVersion getVersionCall() const { return version_; }
	void setVersionCall(MemContainerVersion v) { version_ = v; }
	
	void startUpCall(MemContainerVersion v);
	void shutDownCall();
	MemContainer& operator[](size_t index);
	size_t initAllocationCall(MemContainerType type, const size_t size, const size_t numOfBlocks);
	void largerIndexList(size_t t);

	template<typename T>
	T* allocCall(size_t index) 	{ return reinterpret_cast<T*>(containers_[index]->alloc(sizeof(T))); }

	void releaseCall(size_t index, unsigned char* ptr = nullptr) { containers_[index]->release(ptr); }
};
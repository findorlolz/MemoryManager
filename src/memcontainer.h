#pragma once

/*This is abstract class for different memory stuctures
Niklas Smal Oct. 2013*/

#include <cstdlib>
#include <cassert>

enum memContainerType{
	STACK, POOL, BUFFER
};

enum memContainerState{
	BROKEN, READY, FULL
};


class MemContainer
{
public:
	MemContainer(size_t size) : totalSpace_(size) 
	{
		state_ = BROKEN;
	}

	virtual ~MemContainer(){}

	virtual void startUp() = 0;
	virtual void shutDown() = 0;
	virtual unsigned char* alloc(size_t) = 0;

	bool isGood() const { return state_ == READY; }

	//Accessories
	memContainerType getType() const { return type_; }
	size_t getSpaceLeft() const { return spaceLeft_; }
	const size_t getTotalSpace() const { return totalSpace_; }

protected:
	memContainerType type_;
	memContainerState state_;
	unsigned char* cursor_;
	unsigned char* begin_;
	const size_t totalSpace_;
	size_t spaceLeft_;

	bool isRoom(size_t) const;
	bool isEmpty() const;

private:
};
	
template<typename T>
static T* ptr(unsigned char*p) { return reinterpret_cast<T*>(p);} 
#pragma once

#include "memcontainer.h"

template<typename T>
class cVector
{
public:
	cVector() : size_(0), capacity_(0), data_(0) {}
	~cVector() { deleteRange(data_, data_ + size_); free(data_); }

	void push_back(const T& v)
	{
		if(size_ != capacity_)
		{
			new((void*) (data_ + size_)) T(v);
			size_++;
			return;
		}
        size_t newCapacity = capacity_ ? capacity_ * 2 : 1;
        T* newData = allocate(newCapacity);
        copyRange(data_, data_ + size_, newData);
        new((void*)(newData + size_)) T(v);
        deleteRange(data_, data_ + size_);
        free(data_);
        data_ = newData;
        capacity_ = newCapacity;
        ++size_;
	}
	
	T& operator[](size_t index) { return data_[index]; }
    const T& operator[](size_t index) const { return data[index]_; }
    T* begin() const { return data_; }
    T* end() const { return data_ + size_; }

private:
	size_t size_;
	size_t capacity_;
	T* data_;

	static T* allocate(size_t s) { return reinterpret_cast<T*>(malloc(sizeof(T) * s)); }

	static void copyRange(T* begin, T* end, T* dest)
	{
		while(begin != end)
		{
			new((void*)dest) T(*begin);
			++begin;
			++dest;
		}
	}

	static void deleteRange(T* begin, T* end)
	{
		while(begin != end)
		{
			begin->~T();
			++begin;
		}
	}
};

template<typename T>
class nsVector
{
public:
	nsVector() : size_(0), capacity_(0), data_(0) {}
	nsVector(const size_t i) : size_(0), capacity_(i)
	{
		data_ = reinterpret_cast<T*>(malloc(capacity_*s));
	}

	~nsVector() { release(); }

	static const size_t s  = sizeof(T*);

	void push_back(const T& v)
	{
		if(size_ == capacity_)
		{
			capacity_ = capacity_ ? capacity_ * 2 : 16;
			allocateAndCopy();
		}
		new((void*) (data_+size_)) T;
        ++size_;
	}

	void pop_back() { --size_; }
	
	T& operator[](size_t index) { return data_[index]; }
    const T& operator[](size_t index) const { return data_[index]; }
    T* begin() const { return data_; }
    T* end() const { return data_ + size_; }
	size_t getCapacity() const { return capacity_;}
	void clear() { size_ = 0; }

private:
	inline void allocateAndCopy()
	{
		T* newBegin = reinterpret_cast<T*>(malloc(capacity_*s));
		copyAndDeleteRange(data_ , data_+size_, newBegin);
		free(data_);
		data_ = newBegin;
	}

	static inline void copyAndDeleteRange(T* begin, T* end, T* newBegin)
	{
		while(begin != end)
		{
			new((void*) newBegin) T(*begin);
			begin->~T();
			++begin;
			++newBegin;
		}
	}

	static inline void deleteRange(T* begin, T* end)
	{
		while(begin != end)
		{
			begin->~T();
			++begin;
		}
	}

	inline void release()
	{
		if(data_ != nullptr)
		{
			free(data_);
			deleteRange(data_ , data_ + size_);
			data_ = nullptr;
		}
	}

	T* data_;
	size_t size_;
	size_t capacity_;
};
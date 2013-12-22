#include "memorymanager.h"
#include "membuffer.h"
/*include only for debuging state*/
//#include <vld.h>

#include <iostream>

/*
This is main function for testing different memory containers
Niklas Smal Oct. 2013
*/

struct test{
	float t;
};

//Test memStack
/*
int main()
{
	MemStack* stack = new MemStack(10*sizeof(test));
	std::cout << "isGood pre start: " << stack->isGood() << std::endl;
	stack->startUp();
	std::cout << "isGood post start: " << stack->isGood() << std::endl;
	stack->popStack();
	std::cout << "Type: " << stack->getType() << std::endl;
	std::cout << "Space :" << stack->getSpaceLeft() << "/" << stack->getTotalSpace() << std::endl;
	test* t1 = (test*) stack->pushStack(sizeof(test));
	t1->t = 1.521f;
	std::cout << "1.521f = " << t1->t << std::endl;
	std::cout << "Space :" << stack->getSpaceLeft() << "/" << stack->getTotalSpace() << std::endl;
	stack->popStack();
	std::cout << "After pop - Space :" << stack->getSpaceLeft() << "/" << stack->getTotalSpace() << std::endl;
	test* t2 = ptr<test>(stack->pushStack(sizeof(test)));
	t2->t = 2.521f;
	std::cout << "2.52123f = " << t2->t << std::endl;
	std::cout << "Space :" << stack->getSpaceLeft() << "/" << stack->getTotalSpace() << std::endl;
	for(size_t i = 0; i < 9; ++i)
	{
		t2 = ptr<test>(stack->pushStack(sizeof(test)));
		t2->t = i;
	}
	std::cout << "8 = " << t2->t << std::endl;
	std::cout << "Space :" << stack->getSpaceLeft() << "/" << stack->getTotalSpace() << std::endl;
	stack->shutDown();
	delete stack;
	system("pause");
	return 0;
}*/

//Test membuffer
/*
int main()
{
	MemBuffer* buffer = new MemBuffer(sizeof(test) * 10);
	std::cout << "pre ready: 0/" << buffer->isGood() << std::endl;
	buffer->startUp();
	std::cout << "post ready: 1/" << buffer->isGood() << std::endl;
	test* t1 = ptr<test>(buffer->pushBuffer(sizeof(test)));
	t1->t = 2.0f;
	std::cout << "2.0 = " << t1->t << std::endl;
	buffer->clear();
	for(size_t i = 0; i < 10; i++)
		t1->t = i;
	std::cout << "9 = " << t1->t << std::endl;
	system("pause");
	return 0;
}*/

int main()
{
	MemoryManager manager = MemoryManager::get();
	manager.startUp();


	size_t stackIndex = manager.initAllocation(STACK, sizeof(float) * 20);
	test t = *(manager.alloc<test>(stackIndex));
	t.t = 3;
	std::cout << "3 = " << t.t << std::endl;
	manager.shutDown();

	system("pause");
	return 0;
}

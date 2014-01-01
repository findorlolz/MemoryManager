#include "memorymanager.h"
#include <omp.h>

/*include only for debuging state*/
#include <vld.h>

#include <iostream>

/*
This is main function for testing different memory containers.
THESE MIGHT BE OUT OF DATE!
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
	test* t1 = (test*) stack->alloc(sizeof(test));
	t1->t = 1.521f;
	std::cout << "1.521f = " << t1->t << std::endl;
	std::cout << "Space :" << stack->getSpaceLeft() << "/" << stack->getTotalSpace() << std::endl;
	stack->popStack();
	std::cout << "After pop - Space :" << stack->getSpaceLeft() << "/" << stack->getTotalSpace() << std::endl;
	test* t2 = ptr<test>(stack->alloc(sizeof(test)));
	t2->t = 2.521f;
	std::cout << "2.52123f = " << t2->t << std::endl;
	std::cout << "Space :" << stack->getSpaceLeft() << "/" << stack->getTotalSpace() << std::endl;
	for(size_t i = 0; i < 9; ++i)
	{
		t2 = ptr<test>(stack->alloc(sizeof(test)));
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
	test* t1 = ptr<test>(buffer->alloc(sizeof(test)));
	t1->t = 2.0f;
	std::cout << "2.0 = " << t1->t << std::endl;
	buffer->clear();
	for(size_t i = 0; i < 10; i++)
		t1->t = i;
	std::cout << "9 = " << t1->t << std::endl;
	system("pause");
	return 0;
}*/

//Test for manager funtionalities and memstack
/*
int main()
{
	MemoryManager manager = MemoryManager::get();
	manager.startUp();


	size_t stackIndex1 = manager.initAllocation(MemContainerType_POOL, sizeof(float), 3);
	std::cout << "Alloc 1" << std::endl;
	test* t1 = manager.alloc<test>(stackIndex1);
	t1->t = 1;
	std::cout << "1 = " << t1->t << std::endl;
	std::cout << "Alloc 2" << std::endl;
	test* t2 = manager.alloc<test>(stackIndex1);
	t2->t = 2;
	std::cout << "2 = " << t2->t << std::endl;
	std::cout << "Alloc 3" << std::endl;
	test* t3 = manager.alloc<test>(stackIndex1);
	t3->t = 3;
	std::cout << "3 = " << t3->t << std::endl;
	std::cout << "2 = " << t2->t << std::endl;
	
	std::cout << "Release 3" << std::endl;
	manager.release<test>(stackIndex1, t3);
	std::cout << "Alloc 4" << std::endl;
	test* t4 = manager.alloc<test>(stackIndex1);
	t4->t = 4;
	std::cout << "4 = " << t4->t << std::endl;
	std::cout << "1 = " << t1->t << std::endl;

	manager.shutDown();

	system("pause");
	return 0;
}
*/

int main()
{
	MemoryManager manager = MemoryManager::get();
	manager.startUp();

	std::cout << "Normal array   ";
	double start = omp_get_wtime();
	size_t* testArray = new size_t[100000];
	for(auto i = 0u; i < 100000; i++)
	{
		testArray[i] = i;
	}
	double end = omp_get_wtime();
	std::cout << "Timer: " << end - start << std::endl;
	delete[] testArray;

	std::cout << "Std::vector with push in each  ";
	start = omp_get_wtime();
	std::vector<size_t> testVector1;
	for(auto i = 0u; i < 100000; i++)
	{
		testVector1.push_back(i);
	}
	end = omp_get_wtime();
	std::cout << "Timer: " << end - start << std::endl;
	
	manager.shutDown();
	system("pause");
	return 0;
}


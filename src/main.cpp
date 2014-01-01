#include "memorymanager.h"
#include <omp.h>

/*include only for debuging state*/
//#include <vld.h>

#include <iostream>

/*
This is main function for testing different memory containers.
THESE MIGHT BE OUT OF DATE!
Niklas Smal Oct. 2013
*/

struct test{
	size_t t;
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

//Test for manager funtionalities and mempool
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

//Memory containers benchmarking

int main()
{
	MemoryManager manager = MemoryManager::get();
	const size_t run1 = 1000000;
	const size_t run2 = 100;
	double total, start, end;
	const size_t s = sizeof(test);
	manager.startUp(MemContainerVersion_RELEASE);
	std::cout << "Normal array				";
	total = 0;
	for(auto j = 0u; j < run2; j++)
	{
		start = omp_get_wtime();
		size_t* testArray = new size_t[run1];
		for(auto i = 0u; i < run1; i++)
		{
			testArray[i] = i;
		}
		end = omp_get_wtime();
		total += end - start;
		delete[] testArray;
	}
	std::cout << total/(double) run2 << std::endl;

	std::cout << "Std::vector with push in each		";
	total = 0;
	for(auto j = 0u; j < run2; j++)
	{
		start = omp_get_wtime();
		std::vector<test> testVector1;
		for(auto i = 0u; i < run1; i++)
		{
			test tmp;
			tmp.t = i;
			testVector1.push_back(tmp);
		}
		end = omp_get_wtime();
		total += end - start;

	}
	std::cout << total/(double) run2 << std::endl;
	
	std::cout << "Std::vector with reverse		";
	total = 0;
	for(auto j = 0u; j < run2; j++)
	{
		start = omp_get_wtime();
		std::vector<test> testVector1;
		testVector1.reserve(run1);
		for(auto i = 0u; i < run1; i++)
		{
			test tmp;
			tmp.t = i;
			testVector1.push_back(tmp);
		}
		end = omp_get_wtime();
		total += end - start;
	}
	std::cout << total/(double) run2 << std::endl;

	std::cout << "Memory buffer (release):		";
	total = 0;
	for(auto j = 0u; j < run2; j++)
	{
		start = omp_get_wtime();
		size_t index = manager.initAllocation(MemContainerType_BUFFER, run1*s);
		for(auto i = 0u; i < run1; i++)
		{
			test* tmp = manager.alloc<test>(index);
			tmp->t = i;
		}
		end = omp_get_wtime();
		total += end - start;
	}
	std::cout << total/(double) run2 << std::endl;

	std::cout << "Memory pool(release):			";
	total = 0;
	for(auto j = 0u; j < run2; j++)
	{
		start = omp_get_wtime();
		size_t index = manager.initAllocation(MemContainerType_POOL, s, run1);
		for(auto i = 0u; i < run1; i++)
		{
			test* tmp = manager.alloc<test>(index);
			tmp->t = i;
		}
		end = omp_get_wtime();
		total += end - start;
	}
	std::cout << total/(double) run2 << std::endl;

	manager.setVersion(MemContainerVersion_DEBUG);

	std::cout << "Memory buffer (debug):			";
	total = 0;
	for(auto j = 0u; j < run2; j++)
	{
		start = omp_get_wtime();
		size_t index = manager.initAllocation(MemContainerType_BUFFER, run1*s);
		for(auto i = 0u; i < run1; i++)
		{
			test* tmp = manager.alloc<test>(index);
			tmp->t = i;
		}
		end = omp_get_wtime();
		total += end - start;
	}
	std::cout << total/(double) run2 << std::endl;

	std::cout << "Memory pool(debug):			";
	total = 0;
	for(auto j = 0u; j < run2; j++)
	{
		start = omp_get_wtime();
		size_t index = manager.initAllocation(MemContainerType_POOL, s, run1);
		for(auto i = 0u; i < run1; i++)
		{
			test* tmp = manager.alloc<test>(index);
			tmp->t = i;
		}
		end = omp_get_wtime();
		total += end - start;
	}
	std::cout << total/(double) run2 << std::endl;

	manager.setVersion(MemContainerVersion_RELEASE);

	std::cout << "Memory stack clear(release):		";
	total = 0;
	start = omp_get_wtime();
	size_t index = manager.initAllocation(MemContainerType_BUFFER, s*run1);
	end = omp_get_wtime();
	total += end - start;	

	for(auto j = 0u; j < run2; j++)
	{
		start = omp_get_wtime();
		manager.release<test>(index);
		for(auto i = 0u; i < run1; i++)
		{
			test* tmp = manager.alloc<test>(index);
			tmp->t = i;
		}
		end = omp_get_wtime();
		total += end - start;
	}
	std::cout << total/(double) run2 << std::endl;

	manager.setVersion(MemContainerVersion_DEBUG);

	std::cout << "Memory stack clear(debug):		";
	total = 0;
	start = omp_get_wtime();
	index = manager.initAllocation(MemContainerType_BUFFER, run1*s);
	end = omp_get_wtime();
	total += end - start;	

	for(auto j = 0u; j < run2; j++)
	{
		start = omp_get_wtime();
		manager.release<test>(index);
		for(auto i = 0u; i < run1; i++)
		{
			test* tmp = manager.alloc<test>(index);
			tmp->t = i;
		}
		end = omp_get_wtime();
		total += end - start;
	}
	std::cout << total/(double) run2 << std::endl;

	manager.setVersion(MemContainerVersion_RELEASE);

	std::cout << "Memory stack direct access:		";
	total = 0;
	for(auto j = 0u; j < run2; j++)
	{
		start = omp_get_wtime();
		MemStack stack(run1*s);
		stack.startUp();
		for(auto i = 0u; i < run1; i++)
		{
			test* tmp = reinterpret_cast<test*>(stack.alloc(s));
			tmp->t = i;
		}
		end = omp_get_wtime();
		stack.shutDown();
		total += end - start;
	}
	std::cout << total/(double) run2 << std::endl;

	std::cout << "Memory pool direct access:		";
	total = 0;
	for(auto j = 0u; j < run2; j++)
	{
		start = omp_get_wtime();
		MemPool pool(s, run1);
		pool.startUp(s, run1);
		for(auto i = 0u; i < run1; i++)
		{
			test* tmp = reinterpret_cast<test*>(pool.alloc(s));
			tmp->t = i;
		}
		end = omp_get_wtime();
		pool.shutDown();
		total += end - start;
	}
	std::cout << total/(double) run2 << std::endl;

	std::cout << "YOLO Memory buffer direct access:	";
	total = 0;
	for(auto j = 0u; j < run2; j++)
	{
		start = omp_get_wtime();
		YoloMemBuffer buffer;
		buffer.startUp(s*run1);
		for(auto i = 0u; i < run1; i++)
		{
			test* tmp = reinterpret_cast<test*>(buffer.alloc(s));
			tmp->t = i;
		}
		end = omp_get_wtime();
		buffer.shutDown();
		total += end - start;
	}
	std::cout << total/(double) run2 << std::endl;

	std::cout << "YOLO Memory pool direct access:		";
	total = 0;
	for(auto j = 0u; j < run2; j++)
	{
		start = omp_get_wtime();
		YoloMemPool pool;
		pool.startUp(s, run1);
		for(auto i = 0u; i < run1; i++)
		{
			test* tmp = reinterpret_cast<test*>(pool.alloc());
			tmp->t = i;
		}
		end = omp_get_wtime();
		pool.shutDown();
		total += end - start;
	}
	std::cout << total/(double) run2 << std::endl;
	manager.shutDown();
	system("pause");
	return 0;
}


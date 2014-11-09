#include <stdio.h>
#include <vector>

#include <AnimaStackAllocator.h>

#define LINEAR_ALLOC 1
#define STACK_ALLOC 1
#define FREELIST_ALLOC 1
#define POOL_ALLOC 1

#define MAX_NUM_ALLOCS 100000

#define NUM_16B_ALLOCS 10000
#define NUM_256B_ALLOCS 1000
#define NUM_2MB_ALLOCS 50

#define MB2INBYTES  2097152 //2mb

#define MEM_SIZE 1048576000 //1GB

int main(int argc, char** argv)
{
	void* pMem = malloc(MEM_SIZE);
	
	void* allocs[MAX_NUM_ALLOCS];
	
	Anima::AnimaStackAllocator* pStackAlloc   = new Anima::AnimaStackAllocator(MEM_SIZE, pMem);
	
	for(int i = 0; i < NUM_16B_ALLOCS; i++)
		allocs[i] = pStackAlloc->Allocate(16, 8);
	
	for(int i = NUM_16B_ALLOCS-1; i >= 0; i--)
		pStackAlloc->Deallocate(allocs[i]);
	
	for(int i = 0; i < NUM_256B_ALLOCS; i++)
		allocs[i] = pStackAlloc->Allocate(256, 8);
	
	for(int i = NUM_256B_ALLOCS-1; i >= 0; i--)
		pStackAlloc->Deallocate(allocs[i]);
	
	for(int i = 0; i < NUM_2MB_ALLOCS; i++)
		allocs[i] = pStackAlloc->Allocate(MB2INBYTES, 8);;
	
	for(int i = NUM_2MB_ALLOCS-1; i >= 0; i--)
		pStackAlloc->Deallocate(allocs[i]);
	
	delete pStackAlloc;
}
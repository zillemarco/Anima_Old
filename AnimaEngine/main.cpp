#include <stdio.h>
#include <xmmintrin.h>

#include <AnimaAssert.h>

int main()
{
    ANIMA_ASSERT(false);
	
	for(long long i = 0; i < 10000000; i++)
	{
		printf("%d\n", i);
	}
}
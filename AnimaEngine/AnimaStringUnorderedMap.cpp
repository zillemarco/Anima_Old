#include "AnimaStringUnorderedMap.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

namespace AnimaStringUnorderedMapNamespace
{
	size_t Hash::operator()(const char* ptr)
	{
		size_t hash = 0;
		for (; *ptr; ++ptr)
			hash = (hash << 2) + *ptr;
		return hash;
	}

	bool Comparaison::operator()(const char* str1, const char* str2)
	{
		for (; *str1 && *str1 == *str2; ++str1, ++str2) {}
		return (*str1 == *str2);
	}
}

END_ANIMA_ENGINE_NAMESPACE
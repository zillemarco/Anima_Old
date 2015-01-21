#pragma once

#include "AnimaEngineCore.h"
#include <boost\unordered_map.hpp>

BEGIN_ANIMA_ENGINE_NAMESPACE

namespace AnimaStringUnorderedMapNamespace
{
	struct Hash 
	{
		size_t operator()(const char* ptr);
	};

	struct Comparaison 
	{
		bool operator()(const char* str1, const char* str2);
	};
}

template<typename T>
struct AnimaStringUnorderedMap : public boost::unordered_map < const char*, T, AnimaStringUnorderedMapNamespace::Hash, AnimaStringUnorderedMapNamespace::Comparaison > {};

END_ANIMA_ENGINE_NAMESPACE
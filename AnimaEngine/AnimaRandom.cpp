//
//  AnimaRandom.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaRandom.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

namespace AnimaRandom
{
	static bool _initialized = false;

	std::default_random_engine& GetRandomEngine()
	{
		static std::default_random_engine u{};
		return u;
	}

	void Randomize()
	{
		static std::random_device rd{};
		GetRandomEngine().seed(rd());

		_initialized = true;
	}

	AInt GetRandomInt(const AInt& from, const AInt& to)
	{
		if (!_initialized)
			Randomize();

		static std::uniform_int_distribution<> d{};
		using param_t = decltype(d)::param_type;
		return d(GetRandomEngine(), param_t{ from, to });
	}

	AFloat GetRandomFloat(const AFloat& from, const AFloat& to)
	{
		if (!_initialized)
			Randomize();

		static std::uniform_real_distribution<> d{};
		using param_t = decltype(d)::param_type;
		return d(GetRandomEngine(), param_t{ from, to });
	}
}

END_ANIMA_ENGINE_NAMESPACE

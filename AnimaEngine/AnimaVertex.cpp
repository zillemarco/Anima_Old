#include "AnimaVertex.h"
#include "AnimaMatrix.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

#define _mm_shufd(xmm, mask) _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(xmm), mask))

template class AnimaVertex<float, 2>;
template class AnimaVertex<float, 3>;
template class AnimaVertex<float, 4>;

template class AnimaVertex<char, 3>;
template class AnimaVertex<char, 4>;

#undef _mm_shufd

END_ANIMA_ENGINE_NAMESPACE

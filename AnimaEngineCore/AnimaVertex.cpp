#include "AnimaVertex.h"

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

template class AnimaVertex<double, 2>;
template class AnimaVertex<double, 3>;
template class AnimaVertex<double, 4>;

template class AnimaVertex<float, 2>;
template class AnimaVertex<float, 3>;
template class AnimaVertex<float, 4>;

template class AnimaVertex<char, 3>;
template class AnimaVertex<char, 4>;

END_ANIMA_ENGINE_CORE_NAMESPACE
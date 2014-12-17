#include "AnimaMatrix.h"
#include "AnimaAssert.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaMatrix::AnimaMatrix(AnimaEngine* engine)
{
	ANIMA_ASSERT(engine != nullptr);
	_engine = engine;

	_matrixData = (AFloat*)(_engine->GetGenericAllocator())->Allocate(sizeof(AFloat) * ANIMA_MATRIX_SIZE, ANIMA_ENGINE_ALIGN_OF(AFloat));
	memset(_matrixData, 0, sizeof(AFloat) * ANIMA_MATRIX_SIZE);
}

AnimaMatrix::AnimaMatrix(AnimaEngine* engine, AFloat data[ANIMA_MATRIX_SIZE])
{
	ANIMA_ASSERT(engine != nullptr);
	_engine = engine;

	_matrixData = (AFloat*)(_engine->GetGenericAllocator())->Allocate(sizeof(AFloat) * ANIMA_MATRIX_SIZE, ANIMA_ENGINE_ALIGN_OF(AFloat));
	memset(_matrixData, 0, sizeof(AFloat) * ANIMA_MATRIX_SIZE);
}

AnimaMatrix::AnimaMatrix(const AnimaMatrix& src)
{
	_engine = src._engine;
	_matrixData = (AFloat*)(_engine->GetGenericAllocator())->Allocate(sizeof(AFloat) * ANIMA_MATRIX_SIZE, ANIMA_ENGINE_ALIGN_OF(AFloat));

	SetData(src._matrixData);
}

AnimaMatrix::AnimaMatrix(AnimaMatrix&& src)
: _engine(src._engine)
, _matrixData(src._matrixData)
{
	src._matrixData = nullptr;
}

AnimaMatrix::~AnimaMatrix()
{
	ANIMA_ASSERT(_engine != nullptr);

	(_engine->GetGenericAllocator())->Deallocate(_matrixData);
	_matrixData = nullptr;
}

AnimaMatrix& AnimaMatrix::operator=(const AnimaMatrix& src)
{
	if (this != &src)
	{
		if (_matrixData == nullptr)
			_matrixData = (AFloat*)(_engine->GetGenericAllocator())->Allocate(sizeof(AFloat) * ANIMA_MATRIX_SIZE, ANIMA_ENGINE_ALIGN_OF(AFloat));

		memcpy(_matrixData, src._matrixData, sizeof(AFloat) * ANIMA_MATRIX_SIZE);
	}

	return *this;
}

AnimaMatrix& AnimaMatrix::operator=(AnimaMatrix&& src)
{
}

void AnimaMatrix::SetData(AFloat data[ANIMA_MATRIX_SIZE])
{
	memcpy(_matrixData, data, sizeof(AFloat) * ANIMA_MATRIX_SIZE);
}

void AnimaMatrix::SetData(AFloat data, int index)
{
	ANIMA_ASSERT(index >= 0 && index < ANIMA_MATRIX_SIZE);
	_matrixData[index] = data;
}

END_ANIMA_ENGINE_NAMESPACE
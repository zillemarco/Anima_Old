#pragma once

#include "AnimaEngineCore.h"
#include "AnimaVertex.h"
#include "AnimaEngine.h"
#include "AnimaTypes.h"

#define ANIMA_MATRIX_SIZE	16

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaMatrix
{
public:
	AnimaMatrix(AnimaEngine* engine);
	AnimaMatrix(AnimaEngine* engine, AFloat data[ANIMA_MATRIX_SIZE]);
	AnimaMatrix(const AnimaMatrix& src);
	AnimaMatrix(AnimaMatrix&& src);
	~AnimaMatrix();

	AnimaMatrix& operator=(const AnimaMatrix& src);
	AnimaMatrix& operator=(AnimaMatrix&& src);

public:
	void SetData(AFloat data[ANIMA_MATRIX_SIZE]);
	void SetData(AFloat data, int index);

private:
	// _matrixData memory structure
	//	[0]		[1]		[2]		[3]
	//	[4]		[5]		[6]		[7]
	//	[8]		[9]		[10]	[11]
	//	[12]	[13]	[14]	[15]
	AFloat* _matrixData;

	AnimaEngine* _engine;
};

END_ANIMA_ENGINE_NAMESPACE
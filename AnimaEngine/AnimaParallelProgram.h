#pragma once

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaNamedObject.h"

#ifdef __APPLE__
#	include "OpenCL/opencl.h"
#else
#	include <CL/cl.h>
#endif

BEGIN_ANIMA_ENGINE_NAMESPACE

enum AnimaParallelelProgramType {
	APP_TYPE_CPU,
	APP_TYPE_GPU,
	APP_TYPE_DEFAULT,
	APP_TYPE_ACCELLERATOR,
	APP_TYPE_ALL
};

class AnimaParallelProgramsManager;

class ANIMA_ENGINE_EXPORT AnimaParallelProgram : public AnimaNamedObject
{
	DECLARE_ANIMA_CLASS(AnimaParallelProgram);

public:
	AnimaParallelProgram(const AnimaString& name, AnimaAllocator* allocator, AnimaParallelProgramsManager* parallelProgramsManager);
	AnimaParallelProgram(const AnimaParallelProgram& src);
	AnimaParallelProgram(AnimaParallelProgram&& src);
	~AnimaParallelProgram();
	
	AnimaParallelProgram& operator=(const AnimaParallelProgram& src);
	AnimaParallelProgram& operator=(AnimaParallelProgram&& src);

	bool operator==(const AnimaParallelProgram& left);
	bool operator!=(const AnimaParallelProgram& left);

public:	
	void SetCode(const AnimaString& code);
	AnimaString GetCode() const;

public:
	static AUint TranslateParallelProgramDeviceType(const AnimaParallelelProgramType& deviceType);
	
private:
	AnimaString	_code;

	AnimaParallelProgramsManager* _parallelProgramsManager;
};

END_ANIMA_ENGINE_NAMESPACE
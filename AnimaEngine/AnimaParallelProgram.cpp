#include "AnimaParallelProgram.h"
#include "AnimaParallelProgramsManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaParallelProgram::AnimaParallelProgram(const AnimaString& name, AnimaAllocator* allocator, AnimaParallelProgramsManager* parallelProgramsManager)
	: AnimaNamedObject(name, allocator)
{
	IMPLEMENT_ANIMA_CLASS(AnimaParallelProgram);

	ANIMA_ASSERT(_parallelProgramsManager != nullptr);
	_parallelProgramsManager = parallelProgramsManager;
}

AnimaParallelProgram::AnimaParallelProgram(const AnimaParallelProgram& src)
	: AnimaNamedObject(src)
	, _code(src._code)
	, _parallelProgramsManager(src._parallelProgramsManager)
{
}

AnimaParallelProgram::AnimaParallelProgram(AnimaParallelProgram&& src)
	: AnimaNamedObject(src)
	, _code(src._code)
	, _parallelProgramsManager(src._parallelProgramsManager)
{
}

AnimaParallelProgram::~AnimaParallelProgram()
{
}

AnimaParallelProgram& AnimaParallelProgram::operator=(const AnimaParallelProgram& src)
{
	if (this != &src)
	{
		AnimaNamedObject::operator=(src);
		_code = src._code;
		_parallelProgramsManager = src._parallelProgramsManager;
	}

	return *this;
}

AnimaParallelProgram& AnimaParallelProgram::operator=(AnimaParallelProgram&& src)
{
	if (this != &src)
	{
		AnimaNamedObject::operator=(src);
		_code = src._code;
		_parallelProgramsManager = src._parallelProgramsManager;
	}

	return *this;
}

bool AnimaParallelProgram::operator==(const AnimaParallelProgram& left)
{
	if (_code != left._code) return false;
	if (_parallelProgramsManager != left._parallelProgramsManager) return false;

	return true;
}

bool AnimaParallelProgram::operator!=(const AnimaParallelProgram& left)
{
	if (_code != left._code) return true;
	if (_parallelProgramsManager != left._parallelProgramsManager) return true;

	return false;
}

void AnimaParallelProgram::SetCode(const AnimaString& code)
{
	_code = code;
}

AnimaString AnimaParallelProgram::GetCode() const
{
	return _code;
}

AUint AnimaParallelProgram::TranslateParallelProgramDeviceType(const AnimaParallelelProgramType& deviceType)
{
	switch (deviceType)
	{
	case APP_TYPE_CPU:			return CL_DEVICE_TYPE_CPU; break;
	case APP_TYPE_GPU:			return CL_DEVICE_TYPE_GPU; break;
	case APP_TYPE_DEFAULT:		return CL_DEVICE_TYPE_DEFAULT; break;
	case APP_TYPE_ACCELLERATOR:	return CL_DEVICE_TYPE_ACCELERATOR; break;
	case APP_TYPE_ALL:			return CL_DEVICE_TYPE_ALL; break;
	}

	return CL_DEVICE_TYPE_ALL;
}

END_ANIMA_ENGINE_NAMESPACE
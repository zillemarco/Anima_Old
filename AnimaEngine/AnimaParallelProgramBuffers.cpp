#include "AnimaParallelProgramBuffers.h"
#include "AnimaParallelProgram.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaParallelProgramBufferBase::AnimaParallelProgramBufferBase(const AnimaString& name, AnimaAllocator* allocator, AnimaParallelProgram* program, AInt programKernelArgumentIndex, AnimaParallelProgramBufferDeviceReadWritePolicy deviceReadWritePolicy, AnimaParallelProgramBufferDeviceComunicationPolicy deviceComunicationPolicy)
	: AnimaNamedObject(name, allocator)
{
	IMPLEMENT_ANIMA_CLASS(AnimaParallelProgramBufferBase);

	ANIMA_ASSERT(program != nullptr)

	_deviceReadWritePolicy = deviceReadWritePolicy;
	_deviceComunicationPolicy = deviceComunicationPolicy;
	_program = program;
	_programKernelArgumentIndex = programKernelArgumentIndex;
}

AnimaParallelProgramBufferBase::AnimaParallelProgramBufferBase(const AnimaParallelProgramBufferBase& src)
	: AnimaNamedObject(src)
	, _deviceReadWritePolicy(src._deviceReadWritePolicy)
	, _deviceComunicationPolicy(src._deviceComunicationPolicy)
	, _program(src._program)
	, _programKernelArgumentIndex(src._programKernelArgumentIndex)
{
}

AnimaParallelProgramBufferBase::AnimaParallelProgramBufferBase(AnimaParallelProgramBufferBase&& src)
	: AnimaNamedObject(src)
	, _deviceReadWritePolicy(src._deviceReadWritePolicy)
	, _deviceComunicationPolicy(src._deviceComunicationPolicy)
	, _program(src._program)
	, _programKernelArgumentIndex(src._programKernelArgumentIndex)
{
}

AnimaParallelProgramBufferBase::~AnimaParallelProgramBufferBase()
{
}

AnimaParallelProgramBufferBase& AnimaParallelProgramBufferBase::operator=(const AnimaParallelProgramBufferBase& src)
{
	if (this != &src)
	{
		AnimaNamedObject::operator=(src);
		_deviceReadWritePolicy = src._deviceReadWritePolicy;
		_deviceComunicationPolicy = src._deviceComunicationPolicy;
		_program = src._program;
		_programKernelArgumentIndex = src._programKernelArgumentIndex;
	}

	return *this;
}

AnimaParallelProgramBufferBase& AnimaParallelProgramBufferBase::operator=(AnimaParallelProgramBufferBase&& src)
{
	if (this != &src)
	{
		AnimaNamedObject::operator=(src);
		_deviceReadWritePolicy = src._deviceReadWritePolicy;
		_deviceComunicationPolicy = src._deviceComunicationPolicy;
		_program = src._program;
		_programKernelArgumentIndex = src._programKernelArgumentIndex;
	}

	return *this;
}

AUint AnimaParallelProgramBufferBase::TranslateParallelProgramBufferDeviceReadWritePolicy(AnimaParallelProgramBufferDeviceReadWritePolicy deviceReadWritePolicy)
{
	switch (deviceReadWritePolicy)
	{
	case APPBDRWP_READ_ONLY:	return CL_MEM_READ_ONLY;
	case APPBDRWP_WRITE_ONLY:	return CL_MEM_WRITE_ONLY;
	case APPBDRWP_READ_WRITE:	return CL_MEM_READ_WRITE;
	}

	return 0;
}

END_ANIMA_ENGINE_NAMESPACE
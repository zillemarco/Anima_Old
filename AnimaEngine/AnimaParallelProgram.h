#pragma once

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaNamedObject.h"
#include "AnimaMappedArray.h"
#include "AnimaParallelProgramBuffers.h"

#ifdef __APPLE__
#include <OpenCL/OpenCL.h>
#else
#include <CL/opencl.h>
#endif

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaParallelProgramsManager;

enum AnimaParallelelProgramType {
	APP_TYPE_CPU,
	APP_TYPE_GPU,
	APP_TYPE_DEFAULT,
	APP_TYPE_ACCELLERATOR,
	APP_TYPE_ALL
};

typedef struct _AnimaParallelProgramContextInfo {
	AnimaParallelelProgramType _type;
	bool _graphicsInterop;
	cl_platform_id _platformId;
	cl_context _context;
	AnimaArray<cl_device_id> _devices;
} AnimaParallelProgramContextInfo;

class ANIMA_ENGINE_EXPORT AnimaParallelProgram : public AnimaNamedObject
{
	DECLARE_ANIMA_CLASS(AnimaParallelProgram);

public:
	AnimaParallelProgram(const AnimaString& name, AnimaAllocator* allocator, AnimaParallelProgramsManager* parallelProgramsManager, const AnimaParallelProgramContextInfo* contextInfo);
	AnimaParallelProgram(const AnimaParallelProgram& src);
	AnimaParallelProgram(AnimaParallelProgram&& src);
	~AnimaParallelProgram();
	
	AnimaParallelProgram& operator=(const AnimaParallelProgram& src);
	AnimaParallelProgram& operator=(AnimaParallelProgram&& src);

	bool operator==(const AnimaParallelProgram& left);
	bool operator!=(const AnimaParallelProgram& left);

public:	
	bool Create();
	bool Destroy();

	bool Execute(AUint globalSize, AUint localSize);

	void SetCode(const AnimaString& code) { _code = code; }
	AnimaString GetCode() const { return _code; }

	void SetKernelName(const AnimaString& kernelName) { _kernelName = kernelName; }
	AnimaString GetKernelName() const { return _kernelName; }

	const AnimaParallelProgramContextInfo* GetContextInfo() const { return _contextInfo; }

	template<typename data_type>
	bool AddSingleValueKernelArgument(const AnimaString& argumentName, AInt argumentIndex, data_type* data, AnimaParallelProgramBufferDeviceReadWritePolicy deviceReadWritePolicy = APPBDRWP_READ_WRITE, AnimaParallelProgramBufferDeviceComunicationPolicy deviceComunicationPolicy = APPBDCP_READ_WRITE)
	{
		// Controllo che non esista già un argomento con quel nome o a quel indice
		if (KernelHasArgument(argumentName) || KernelHasArgument(argumentIndex))
			return false;

		AnimaParallelProgramSingleValueBuffer<data_type>* newBuffer = AnimaAllocatorNamespace::AllocateNew<AnimaParallelProgramSingleValueBuffer<data_type> >(*_allocator, argumentName, _allocator, data, this, argumentIndex, deviceReadWritePolicy, deviceComunicationPolicy);
		_kernelArguments.Add(argumentName, newBuffer);
		return true;
	}

	template<typename vector_type, typename vector_data_type>
	bool AddMultipleValueKernelArgument(const AnimaString& argumentName, AInt argumentIndex, vector_type* data, AnimaParallelProgramBufferDeviceReadWritePolicy deviceReadWritePolicy = APPBDRWP_READ_WRITE, AnimaParallelProgramBufferDeviceComunicationPolicy deviceComunicationPolicy = APPBDCP_READ_WRITE)
	{
		// Controllo che non esista già un argomento con quel nome o a quel indice
		if (KernelHasArgument(argumentName) || KernelHasArgument(argumentIndex))
			return false;

		AnimaParallelProgramMultipleValueBuffer<vector_type, vector_data_type>* newBuffer = AnimaAllocatorNamespace::AllocateNew<AnimaParallelProgramMultipleValueBuffer<vector_type, vector_data_type> >(*_allocator, argumentName, _allocator, data, this, argumentIndex, deviceReadWritePolicy, deviceComunicationPolicy);
		_kernelArguments.Add(argumentName, newBuffer);
		return true;
	}

	bool KernelHasArgument(const AnimaString& argumentName)
	{
		return _kernelArguments.Contains(argumentName) >= 0;
	}

	bool KernelHasArgument(AInt argumentIndex)
	{
		AInt count = _kernelArguments.GetSize();
		for (AInt i = 0; i < count; i++)
		{
			if (_kernelArguments[i]->GetProgramKernelArgumentIndex() == argumentIndex)
				return true;
		}
		return false;
	}

	AnimaParallelProgramBufferBase* GetKernelArgument(AInt argumentIndex) { return _kernelArguments[argumentIndex]; }
	AnimaParallelProgramBufferBase* GetKernelArgumentFromName(const AnimaString& argumentName) { return _kernelArguments[argumentName]; }

	cl_command_queue GetQueue() const { return _queue; }
	cl_program GetProgram() const { return _program; }
	cl_kernel GetKernel() const { return _kernel; }
	
protected:
	bool CreateQueue();
	bool CreateProgram();
	bool CreateKernel();
	
public:
	static AUint TranslateParallelProgramDeviceType(const AnimaParallelelProgramType& deviceType);
	
private:
	AnimaString	_code;
	AnimaString _kernelName;

	cl_command_queue _queue;
	cl_program _program;
	cl_kernel _kernel;

	AnimaMappedArray<AnimaParallelProgramBufferBase*> _kernelArguments;

	const AnimaParallelProgramContextInfo* _contextInfo;
	AnimaParallelProgramsManager* _parallelProgramsManager;
};

END_ANIMA_ENGINE_NAMESPACE
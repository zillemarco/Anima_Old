#include "AnimaParallelProgram.h"
#include "AnimaParallelProgramsManager.h"
#include "AnimaParallelProgramBuffers.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaParallelProgram::AnimaParallelProgram(const AnimaString& name, AnimaAllocator* allocator, AnimaParallelProgramsManager* parallelProgramsManager, const AnimaParallelProgramContextInfo* contextInfo)
	: AnimaNamedObject(name, allocator)
{
	IMPLEMENT_ANIMA_CLASS(AnimaParallelProgram);

	ANIMA_ASSERT(_parallelProgramsManager != nullptr);
	ANIMA_ASSERT(_contextInfo != nullptr);
	_parallelProgramsManager = parallelProgramsManager;
	_contextInfo = contextInfo;

	_queue = nullptr;
	_program = nullptr;
	_kernel = nullptr;
}

AnimaParallelProgram::AnimaParallelProgram(const AnimaParallelProgram& src)
	: AnimaNamedObject(src)
	, _code(src._code)
	, _parallelProgramsManager(src._parallelProgramsManager)
	, _contextInfo(src._contextInfo)
	, _queue(src._queue)
	, _program(src._program)
	, _kernel(src._kernel)
{
}

AnimaParallelProgram::AnimaParallelProgram(AnimaParallelProgram&& src)
	: AnimaNamedObject(src)
	, _code(src._code)
	, _parallelProgramsManager(src._parallelProgramsManager)
	, _contextInfo(src._contextInfo)
	, _queue(src._queue)
	, _program(src._program)
	, _kernel(src._kernel)
{
}

AnimaParallelProgram::~AnimaParallelProgram()
{
	Destroy();
}

AnimaParallelProgram& AnimaParallelProgram::operator=(const AnimaParallelProgram& src)
{
	if (this != &src)
	{
		AnimaNamedObject::operator=(src);
		_code = src._code;
		_parallelProgramsManager = src._parallelProgramsManager;
		_contextInfo = src._contextInfo;
		_queue = src._queue;
		_program = src._program;
		_kernel = src._kernel;
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
		_contextInfo = src._contextInfo;
		_queue = src._queue;
		_program = src._program;
		_kernel = src._kernel;
	}

	return *this;
}

bool AnimaParallelProgram::operator==(const AnimaParallelProgram& left)
{
	if (_code != left._code) return false;
	if (_parallelProgramsManager != left._parallelProgramsManager) return false;
	if (_contextInfo != left._contextInfo) return false;

	return true;
}

bool AnimaParallelProgram::operator!=(const AnimaParallelProgram& left)
{
	if (_code != left._code) return true;
	if (_parallelProgramsManager != left._parallelProgramsManager) return true;
	if (_contextInfo != left._contextInfo) return true;

	return false;
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

bool AnimaParallelProgram::Create()
{
	if (_queue != nullptr && _program != nullptr && _kernel != nullptr)
		return true;

	if (!CreateQueue())
	{
		Destroy();
		return false;
	}

	if (!CreateProgram())
	{
		Destroy();
		return false;
	}

	if (!CreateKernel())
	{
		Destroy();
		return false;
	}

	return true;
}

bool AnimaParallelProgram::Destroy()
{
	bool result = true;

	if (_kernel != nullptr)
		result &= clReleaseKernel(_kernel) == CL_SUCCESS;

	if (_program != nullptr)
		result &= clReleaseProgram(_program) == CL_SUCCESS;

	if (_queue != nullptr)
		result &= clReleaseCommandQueue(_queue) == CL_SUCCESS;

	return result;
}

bool AnimaParallelProgram::CreateQueue()
{
	if (_queue != nullptr)
		return true;

	// Necessito delle informazioni del contesto per avere il contesto e la lista dei device
	if (_contextInfo == nullptr)
		return false;

	AInt status = CL_SUCCESS;
	_queue = clCreateCommandQueue(_contextInfo->_context, _contextInfo->_devices[0], 0, &status);

	if (status != CL_SUCCESS)
	{
		_queue = nullptr;
		return false;
	}

	return true;
}

bool AnimaParallelProgram::CreateProgram()
{
	if (_program != nullptr)
		return true;

	// Necessito delle informazioni del contesto per avere il contesto e la lista dei device
	if (_contextInfo == nullptr)
		return false;

	AInt status = CL_SUCCESS;

	// Prima di tutto creo il programma
	const char* source = _code.c_str();
	_program = clCreateProgramWithSource(_contextInfo->_context, 1, &source, nullptr, &status);
	if (status != CL_SUCCESS)
	{
		_program = nullptr;
		return false;
	}

	// Se il programma è stato creato con successo lo vado a compilare
	status = clBuildProgram(_program, 1, &_contextInfo->_devices[0], nullptr, nullptr, nullptr);
	if (status != CL_SUCCESS)
	{
		// Se il programma è stato creato ma non compilato lo vado a distruggere immediatamente
		clReleaseProgram(_program);
		_program = nullptr;
		return false;
	}

	return true;
}

bool AnimaParallelProgram::CreateKernel()
{
	if (_kernel != nullptr)
		return true;

	// Ho bisogno che il programma sia compilato per poter cercare il kernel
	if (_program == nullptr)
		return false;

	const char* source = _code.c_str();

	AInt status = CL_SUCCESS;
	_kernel = clCreateKernel(_program, _kernelName.c_str(), &status);

	if (status != CL_SUCCESS)
	{
		_kernel = nullptr;
		return false;
	}

	return true;
}

bool AnimaParallelProgram::Execute(AUint globalSize, AUint localSize)
{
	if (_queue == nullptr || _program == nullptr || _kernel == nullptr)
		return false;

	size_t gSize = globalSize;
	size_t lSize = localSize;
	AInt kernelArgumentsCount = _kernelArguments.GetSize();
	AInt status = CL_SUCCESS;

	// Copio i valori degli argomenti che devo passare al kernel nella memoria del dispositivo
	for (AInt i = 0; i < kernelArgumentsCount; i++)
	{
		// Se non riesco a copiare il valore in un argomento non posso eseguire il programma quindi termino segnalando un errore
		if (_kernelArguments[i]->Write() == false)
			return false;
	}
	
	// Imposto gli argomenti del kernel
	for (AInt i = 0; i < kernelArgumentsCount; i++)
	{
		AnimaParallelProgramBufferBase* kernelArgument = _kernelArguments[i];
		status = clSetKernelArg(_kernel, kernelArgument->GetProgramKernelArgumentIndex(), kernelArgument->GetDataSizeForProgram(), kernelArgument->GetDataForProgram());

		// Se non riesco ad impostare un argomento del kernel non posso eseguire il programma quindi termino segnalando un errore
		if (status != CL_SUCCESS)
			return false;
	}

	// Lancio l'esecuzione del programma
	status = clEnqueueNDRangeKernel(_queue, _kernel, 1, nullptr, &gSize, &lSize, 0, nullptr, nullptr);
	if (status != CL_SUCCESS)
		return false;

	// Una volta lanciata l'esecuzione del programma leggo i valori di ritorno
	for (AInt i = 0; i < kernelArgumentsCount; i++)
	{
		AnimaParallelProgramBufferBase* kernelArgument = _kernelArguments[i];
		AnimaParallelProgramBufferDeviceComunicationPolicy deviceComunicationPolicy = kernelArgument->GetDeviceComunicationPolicy();
		if (deviceComunicationPolicy == APPBDCP_READ_WRITE || deviceComunicationPolicy == APPBDCP_READ_ONLY)
			kernelArgument->Read();
	}

	// Forzo la terminazione della queue
	clFlush(_queue);
	clFinish(_queue);

	return true;
}

END_ANIMA_ENGINE_NAMESPACE
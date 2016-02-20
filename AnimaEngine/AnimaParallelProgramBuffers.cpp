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

template<typename data_type>
bool AnimaParallelProgramMultipleValueBuffer<data_type>::Create()
{
	if (_program == nullptr)
		return false;
	
	if (_bufferMemory != nullptr)
		return true;
	
	AInt status = CL_SUCCESS;
	
	if (_data != nullptr && _needsUpdate == true)
	{
		_bufferMemory = clCreateBuffer(_program->GetContextInfo()->_context, TranslateParallelProgramBufferDeviceReadWritePolicy(_deviceReadWritePolicy) | CL_MEM_USE_HOST_PTR, _dataLength * sizeof(data_type), _data, &status);
		_needsUpdate = false;
	}
	else
		_bufferMemory = clCreateBuffer(_program->GetContextInfo()->_context, TranslateParallelProgramBufferDeviceReadWritePolicy(_deviceReadWritePolicy), _dataLength * sizeof(data_type), nullptr, &status);
	
	// Se la creazione non Ë andata a buon fine reimposto l'attributo _bufferMemory a nullptr e termino tornando false
	if (status != CL_SUCCESS)
	{
		_bufferMemory = nullptr;
		return false;
	}
	
	return true;
}

template<typename data_type>
bool AnimaParallelProgramMultipleValueBuffer<data_type>::Read()
{
	// Posso leggere il valore dalla memoria solamente se la policy me lo consente e il buffer Ë stato creato
	if (_bufferMemory != nullptr && (_deviceComunicationPolicy == APPBDCP_READ_ONLY || _deviceComunicationPolicy == APPBDCP_READ_WRITE))
	{
		AInt status = clEnqueueReadBuffer(_program->GetQueue(), _bufferMemory, CL_TRUE, 0, _dataLength * sizeof(data_type), _data, 0, nullptr, nullptr);
		// Se la lettura non Ë andata a buon fine termino tornando false
		if (status != CL_SUCCESS)
			return false;
		return true;
	}
	return false;
}

template<typename data_type>
bool AnimaParallelProgramMultipleValueBuffer<data_type>::Write()
{
	// Deve per forza essere prima creato e poi scritto
	if (_bufferMemory == nullptr)
		return false;
	
	// Se non ho bisogno di aggiornare il buffer allora termino immediatamente
	if (_needsUpdate == false)
		return true;
	
	AInt status = clEnqueueWriteBuffer(_program->GetQueue(), _bufferMemory, CL_TRUE, 0, _dataLength * sizeof(data_type), _data, 0, nullptr, nullptr);
	
	// Se la scrittura non Ë andata a buon fine termino tornando false
	if (status != CL_SUCCESS)
		return false;
	
	_needsUpdate = false;
	return true;
}

template class AnimaParallelProgramMultipleValueBuffer<AFloat>;

END_ANIMA_ENGINE_NAMESPACE
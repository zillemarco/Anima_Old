#pragma once

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaNamedObject.h"
#include "AnimaMappedArray.h"

#include <type_traits>

#ifdef __APPLE__
#include <OpenCL/OpenCL.h>
#else
#include <CL/opencl.h>
#endif

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaParallelProgram;

enum AnimaParallelProgramBufferDeviceReadWritePolicy {
	APPBDRWP_READ_ONLY,
	APPBDRWP_WRITE_ONLY,
	APPBDRWP_READ_WRITE
};

enum AnimaParallelProgramBufferDeviceComunicationPolicy {
	APPBDCP_READ_ONLY,
	APPBDCP_WRITE_ONLY,
	APPBDCP_READ_WRITE
};

class ANIMA_ENGINE_EXPORT AnimaParallelProgramBufferBase : public AnimaNamedObject
{
	DECLARE_ANIMA_CLASS(AnimaParallelProgramBufferBase);

protected:
	AnimaParallelProgramBufferBase(const AnimaString& name, AnimaAllocator* allocator, AnimaParallelProgram* program, AInt programKernelArgumentIndex, AnimaParallelProgramBufferDeviceReadWritePolicy deviceReadWritePolicy = APPBDRWP_READ_WRITE, AnimaParallelProgramBufferDeviceComunicationPolicy deviceComunicationPolicy = APPBDCP_READ_WRITE);
	AnimaParallelProgramBufferBase(const AnimaParallelProgramBufferBase& src);
	AnimaParallelProgramBufferBase(AnimaParallelProgramBufferBase&& src);

public:
	~AnimaParallelProgramBufferBase();
	
	AnimaParallelProgramBufferBase& operator=(const AnimaParallelProgramBufferBase& src);
	AnimaParallelProgramBufferBase& operator=(AnimaParallelProgramBufferBase&& src);
		
public:
	virtual bool Create() = 0;
	virtual bool Read() = 0;
	virtual bool Write() = 0;
	virtual void* GetDataForProgram() = 0;
	virtual AUint GetDataSizeForProgram() = 0;

public:
	void SetDeviceReadWritePolicy(AnimaParallelProgramBufferDeviceReadWritePolicy deviceReadWritePolicy) { _deviceReadWritePolicy = deviceReadWritePolicy; }
	AnimaParallelProgramBufferDeviceReadWritePolicy GetDeviceReadWritePolicy() const { return _deviceReadWritePolicy; }

	void SetDeviceComunicationPolicy(AnimaParallelProgramBufferDeviceComunicationPolicy deviceComunicationPolicy) { _deviceComunicationPolicy = deviceComunicationPolicy; }
	AnimaParallelProgramBufferDeviceComunicationPolicy GetDeviceComunicationPolicy() const { return _deviceComunicationPolicy; }

	AnimaParallelProgram* GetProgram() { return _program; }

	void SetProgramKernelArgumentIndex(AInt programKernelArgumentIndex) { _programKernelArgumentIndex = programKernelArgumentIndex; }
	AInt GetProgramKernelArgumentIndex() const { return _programKernelArgumentIndex; }

public:
	static AUint TranslateParallelProgramBufferDeviceReadWritePolicy(AnimaParallelProgramBufferDeviceReadWritePolicy deviceReadWritePolicy);

protected:
	AnimaParallelProgramBufferDeviceReadWritePolicy _deviceReadWritePolicy;
	AnimaParallelProgramBufferDeviceComunicationPolicy _deviceComunicationPolicy;
	AnimaParallelProgram* _program;

	AInt _programKernelArgumentIndex;
};

template<typename data_type>
class ANIMA_ENGINE_EXPORT AnimaParallelProgramSingleValueBuffer : public AnimaParallelProgramBufferBase
{
	DECLARE_ANIMA_CLASS(AnimaParallelProgramSingleValueBuffer);

	static_assert(std::is_same<data_type, AFloat>::value, 
		"AnimaParallelProgramSingleValueBuffer can be used only with float values");

public:
	AnimaParallelProgramSingleValueBuffer(const AnimaString& name, AnimaAllocator* allocator, data_type* data, AnimaParallelProgram* program, AInt programKernelArgumentIndex, AnimaParallelProgramBufferDeviceReadWritePolicy deviceReadWritePolicy = APPBDRWP_READ_WRITE, AnimaParallelProgramBufferDeviceComunicationPolicy deviceComunicationPolicy = APPBDCP_READ_WRITE)
		: AnimaParallelProgramBufferBase(name, allocator, program, programKernelArgumentIndex, deviceReadWritePolicy, deviceComunicationPolicy)
	{
		_data = data;
	}

	AnimaParallelProgramSingleValueBuffer(const AnimaParallelProgramSingleValueBuffer& src)
		: AnimaParallelProgramBufferBase(src)
		, _data(src._data)
	{
	}

	AnimaParallelProgramSingleValueBuffer(AnimaParallelProgramSingleValueBuffer&& src)
		: AnimaParallelProgramBufferBase(src)
		, _data(src._data)
	{
	}

	~AnimaParallelProgramSingleValueBuffer() 
	{
	}

	AnimaParallelProgramSingleValueBuffer& operator=(const AnimaParallelProgramSingleValueBuffer& src)
	{
		if (this != &src)
		{
			AnimaParallelProgramBufferBase::operator=(src);
			_data = src._data;
		}

		return *this;
	}

	AnimaParallelProgramSingleValueBuffer& operator=(AnimaParallelProgramSingleValueBuffer&& src)
	{
		if (this != &src)
		{
			AnimaParallelProgramBufferBase::operator=(src);
			_data = src._data;
		}

		return *this;
	}

public:
	// Il metodo create per i buffer a valore singolo non deve fare nulla perchè in effetti non viene creato un buffer
	bool Create() override { return true; }

	bool Read() override
	{
		// Posso leggere il valore dalla memoria solamente se la policy me lo consente
		if (_deviceReadWritePolicy == APPBDRWP_READ_ONLY || _deviceReadWritePolicy == APPBDRWP_READ_WRITE)
		{
			return true;
		}

		return false;
	}

	bool Write() override { return true; }

	void* GetDataForProgram() override { return (void*)_data; }
	AUint GetDataSizeForProgram() override { return sizeof(data_type); }

	data_type* GetData() { return _data; }

protected:
	data_type* _data;
};

template<typename data_type>
class ANIMA_ENGINE_EXPORT AnimaParallelProgramMultipleValueBuffer : public AnimaParallelProgramBufferBase
{
	DECLARE_ANIMA_CLASS(AnimaParallelProgramMultipleValueBuffer);

	static_assert(std::is_same<data_type, AFloat>::value,
		"AnimaParallelProgramSingleValueBuffer can be used only with float values");

public:
	AnimaParallelProgramMultipleValueBuffer(const AnimaString& name, AnimaAllocator* allocator, AnimaArray<data_type>* data, AnimaParallelProgram* program, AInt programKernelArgumentIndex, AnimaParallelProgramBufferDeviceReadWritePolicy deviceReadWritePolicy = APPBDRWP_READ_WRITE, AnimaParallelProgramBufferDeviceComunicationPolicy deviceComunicationPolicy = APPBDCP_READ_WRITE)
		: AnimaParallelProgramBufferBase(name, allocator, program, programKernelArgumentIndex, deviceReadWritePolicy, deviceComunicationPolicy)
	{
		ANIMA_ASSERT(data != nullptr);

		_data = &(*data)[0];
		_dataLength = data->size();
		_needsUpdate = true;
		_bufferMemory = nullptr;
	}

	AnimaParallelProgramMultipleValueBuffer(const AnimaString& name, AnimaAllocator* allocator, data_type* data, AUint dataLength, AnimaParallelProgram* program, AInt programKernelArgumentIndex, AnimaParallelProgramBufferDeviceReadWritePolicy deviceReadWritePolicy = APPBDRWP_READ_WRITE, AnimaParallelProgramBufferDeviceComunicationPolicy deviceComunicationPolicy = APPBDCP_READ_WRITE)
		: AnimaParallelProgramBufferBase(name, allocator, program, programKernelArgumentIndex, deviceReadWritePolicy, deviceComunicationPolicy)
	{
		ANIMA_ASSERT(data != nullptr);

		_data = data;
		_dataLength = dataLength;
		_needsUpdate = true;
		_bufferMemory = nullptr;
	}

	AnimaParallelProgramMultipleValueBuffer(const AnimaParallelProgramMultipleValueBuffer& src)
		: AnimaParallelProgramBufferBase(src)
		, _data(src._data)
		, _bufferMemory(src._bufferMemory)
	{
	}

	AnimaParallelProgramMultipleValueBuffer(AnimaParallelProgramMultipleValueBuffer&& src)
		: AnimaParallelProgramBufferBase(src)
		, _data(src._data)
		, _bufferMemory(src._bufferMemory)
	{
	}

	~AnimaParallelProgramMultipleValueBuffer()
	{
	}

	AnimaParallelProgramMultipleValueBuffer& operator=(const AnimaParallelProgramMultipleValueBuffer& src)
	{
		if (this != &src)
		{
			AnimaParallelProgramBufferBase::operator=(src);
			_data = src._data;
			_bufferMemory = src._bufferMemory;
		}

		return *this;
	}

	AnimaParallelProgramMultipleValueBuffer& operator=(AnimaParallelProgramMultipleValueBuffer&& src)
	{
		if (this != &src)
		{
			AnimaParallelProgramBufferBase::operator=(src);
			_data = src._data;
			_bufferMemory = src._bufferMemory;
		}

		return *this;
	}
	
public:
	bool Create() override;
	bool Read() override;
	bool Write() override;

	void* GetDataForProgram() override { return (void*)&_bufferMemory; }
	AUint GetDataSizeForProgram() override { return sizeof(cl_mem); }

	void SetData(AnimaArray<data_type>* data)
	{
		ANIMA_ASSERT(data != nullptr);
		_data = &(*data)[0];
		_dataLength = data->size();
		_needsUpdate = true;
	}

	void SetData(data_type* data, AUint dataLength)
	{
		ANIMA_ASSERT(data != nullptr);
		_data = data;
		_dataLength = dataLength;
		_needsUpdate = true;
	}

	data_type* GetData() { return _data; }

	void SetNeedsUpdate() { _needsUpdate = true; }
	
protected:
	data_type* _data;
	AUint _dataLength;
	cl_mem _bufferMemory;

	bool _needsUpdate;
};

//// Definizione dei tipi supportati dal buffer a valore singolo per l'esportazione al di fuori della libreria
//template AnimaParallelProgramSingleValueBuffer < AFloat > ;
//
//// Definizione dei tipi supportati dal buffer a valore multiplo per l'esportazione al di fuori della libreria
//template AnimaParallelProgramMultipleValueBuffer < AFloat >;
//
//template AnimaMappedArray < AnimaParallelProgramBufferBase* > ;

END_ANIMA_ENGINE_NAMESPACE
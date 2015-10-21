//
//  AnimaParallelProgramsManager.cpp
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#include "AnimaParallelProgramsManager.h"
#include "AnimaLogger.h"
#include "AnimaEngine.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaParallelProgramsManager::AnimaParallelProgramsManager(AnimaEngine* engine)
{
	_engine = engine;

	_cpuContext = 0;
	_gpuContext = 0;
	_defaultContext = 0;
	_acceleratorContext = 0;
	_allContext = 0;
}

AnimaParallelProgramsManager::~AnimaParallelProgramsManager()
{
	ClearPrograms();
}

AnimaParallelProgram* AnimaParallelProgramsManager::CreateProgram(const AnimaString& name)
{
	AInt index = _programs.Contains(name);
	if (index >= 0)
	{
		AnimaLogger::LogMessageFormatColor(ANIMA_LOGGER_TEXT_COLOR_RED, "[WARNING] Parallel programs manager: program named %s already exists", name.c_str());
		return nullptr;
	}
	
	AnimaParallelProgram* program = AnimaAllocatorNamespace::AllocateNew<AnimaParallelProgram>(*(_engine->GetParallelProgramsAllocator()), name, _engine->GetParallelProgramsAllocator(), this);
	_programs.Add(name, program);
	return program;
}

void AnimaParallelProgramsManager::ClearPrograms()
{
	AInt count = _programs.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaParallelProgram* program = _programs[i];
		AnimaAllocatorNamespace::DeallocateObject(*(_engine->GetParallelProgramsAllocator()), program);
		program = nullptr;
	}

	_programs.RemoveAll();
}

AnimaParallelProgram* AnimaParallelProgramsManager::GetProgram(AUint index)
{
	return _programs[index];
}

AnimaParallelProgram* AnimaParallelProgramsManager::GetProgramFromName(const AnimaString& name)
{
	return _programs[name];
}

AUint AnimaParallelProgramsManager::GetPlatformsCount() const
{
	AUint count = 0;
	if (clGetPlatformIDs(0, nullptr, &count) != CL_SUCCESS)
		AnimaLogger::LogMessage("[ERROR] Parallel programs manager: unable to get platorms count", ANIMA_LOGGER_TEXT_COLOR_RED);

	return count;
}

AnimaArray<cl_platform_id> AnimaParallelProgramsManager::GetPlatformIDs() const
{
	AUint count = GetPlatformsCount();
	return GetPlatformIDs(count);
}

AnimaArray<cl_platform_id> AnimaParallelProgramsManager::GetPlatformIDs(const AUint& count) const
{
	AnimaArray<cl_platform_id> platforms(count);

	if (count <= 0)
		AnimaLogger::LogMessage("[WARNING] Parallel programs manager: platforms count must be grater than 0 to get platforms IDs");
	else
	{
		if (clGetPlatformIDs(count, &platforms[0], nullptr) != CL_SUCCESS)
			AnimaLogger::LogMessage("[ERROR] Parallel programs manager: unable to get platforms count", ANIMA_LOGGER_TEXT_COLOR_RED);
	}
	return platforms;
}

AnimaString AnimaParallelProgramsManager::GetPlatformName(const cl_platform_id& platform) const
{
	ASizeT size = 0;
	if (clGetPlatformInfo(platform, CL_PLATFORM_NAME, 0, nullptr, &size) != CL_SUCCESS)
	{
		AnimaLogger::LogMessage("[ERROR] Parallel programs manager: unable to get platform name size", ANIMA_LOGGER_TEXT_COLOR_RED);
		return "";
	}

	AnimaString name;
	name.resize(size);
	if (clGetPlatformInfo(platform, CL_PLATFORM_NAME, size, const_cast<char*>(name.data()), nullptr) != CL_SUCCESS)
	{
		AnimaLogger::LogMessage("[ERROR] Parallel programs manager: unable to get platform name", ANIMA_LOGGER_TEXT_COLOR_RED);
		return "";
	}

	return name;
}

AnimaString AnimaParallelProgramsManager::GetPlatformVersion(const cl_platform_id& platform) const
{
	ASizeT size = 0;
	if (clGetPlatformInfo(platform, CL_PLATFORM_VERSION, 0, nullptr, &size) != CL_SUCCESS)
	{
		AnimaLogger::LogMessage("[ERROR] Parallel programs manager: unable to get platform version size", ANIMA_LOGGER_TEXT_COLOR_RED);
		return "";
	}

	AnimaString name;
	name.resize(size);
	if (clGetPlatformInfo(platform, CL_PLATFORM_VERSION, size, const_cast<char*>(name.data()), nullptr) != CL_SUCCESS)
	{
		AnimaLogger::LogMessage("[ERROR] Parallel programs manager: unable to get platform version", ANIMA_LOGGER_TEXT_COLOR_RED);
		return "";
	}

	return name;
}

AUint AnimaParallelProgramsManager::GetDevicesCount(const cl_platform_id& platform, const AnimaParallelelProgramType& type) const
{
	AUint count = 0;
	if (clGetDeviceIDs(platform, AnimaParallelProgram::TranslateParallelProgramDeviceType(type), 0, nullptr, &count) != CL_SUCCESS)
		AnimaLogger::LogMessage("[ERROR] Parallel programs manager: unable to get devices count", ANIMA_LOGGER_TEXT_COLOR_RED);

	return count;
}

AnimaArray<cl_device_id> AnimaParallelProgramsManager::GetDeviceIDs(const cl_platform_id& platform, const AnimaParallelelProgramType& type) const
{
	AUint count = GetDevicesCount(platform, type);
	return GetDeviceIDs(platform, count, type);
}

AnimaArray<cl_device_id> AnimaParallelProgramsManager::GetDeviceIDs(const cl_platform_id& platform, const AUint& count, const AnimaParallelelProgramType& type) const
{
	AnimaArray<cl_device_id> devices(count);

	if (count <= 0)
		AnimaLogger::LogMessage("[WARNING] Parallel programs manager: devices count must be grater than 0 to get devices IDs");
	else
	{
		if (clGetDeviceIDs(platform, AnimaParallelProgram::TranslateParallelProgramDeviceType(type), count, &devices[0], nullptr) != CL_SUCCESS)
			AnimaLogger::LogMessage("[ERROR] Parallel programs manager: unable to get devices", ANIMA_LOGGER_TEXT_COLOR_RED);
	}

	return devices;
}

AnimaString AnimaParallelProgramsManager::GetDeviceName(const cl_device_id& device) const
{
	ASizeT size = 0;
	if (clGetDeviceInfo(device, CL_DEVICE_NAME, 0, nullptr, &size) != CL_SUCCESS)
	{
		AnimaLogger::LogMessage("[ERROR] Parallel programs manager: unable to get device name size", ANIMA_LOGGER_TEXT_COLOR_RED);
		return "";
	}

	AnimaString name;
	name.resize(size);
	if (clGetDeviceInfo(device, CL_DEVICE_NAME, size, const_cast<char*>(name.data()), nullptr) != CL_SUCCESS)
	{
		AnimaLogger::LogMessage("[ERROR] Parallel programs manager: unable to get device name", ANIMA_LOGGER_TEXT_COLOR_RED);
		return "";
	}

	return name;
}

AnimaString AnimaParallelProgramsManager::GetDeviceVersion(const cl_device_id& device) const
{
	ASizeT size = 0;
	if (clGetDeviceInfo(device, CL_DEVICE_VERSION, 0, nullptr, &size) != CL_SUCCESS)
	{
		AnimaLogger::LogMessage("[ERROR] Parallel programs manager: unable to get device version size", ANIMA_LOGGER_TEXT_COLOR_RED);
		return "";
	}

	AnimaString name;
	name.resize(size);
	if (clGetDeviceInfo(device, CL_DEVICE_VERSION, size, const_cast<char*>(name.data()), nullptr) != CL_SUCCESS)
	{
		AnimaLogger::LogMessage("[ERROR] Parallel programs manager: unable to get device version", ANIMA_LOGGER_TEXT_COLOR_RED);
		return "";
	}

	return name;
}

bool AnimaParallelProgramsManager::GetContext(cl_context& context, const AnimaParallelelProgramType& type, cl_platform_id platformId, bool tryCreateIfInvalid)
{
	context = nullptr;

	switch (type)
	{
	case APP_TYPE_CPU:
		if (_cpuContext == 0 && tryCreateIfInvalid)
		{
			if (!CreateContext(APP_TYPE_CPU, platformId))
				return false;
		}

		context = _cpuContext;
		break;
	case APP_TYPE_GPU:
		if (_gpuContext == 0 && tryCreateIfInvalid)
		{
			if (!CreateContext(APP_TYPE_GPU, platformId))
				return false;
		}

		context = _gpuContext;
		break;
	case APP_TYPE_DEFAULT:
		if (_defaultContext == 0 && tryCreateIfInvalid)
		{
			if (!CreateContext(APP_TYPE_DEFAULT, platformId))
				return false;
		}

		context = _defaultContext;
		break;
	case APP_TYPE_ACCELLERATOR:
		if (_acceleratorContext == 0 && tryCreateIfInvalid)
		{
			if (!CreateContext(APP_TYPE_ACCELLERATOR, platformId))
				return false;
		}

		context = _acceleratorContext;
		break;
	case APP_TYPE_ALL:
		if (_allContext == 0 && tryCreateIfInvalid)
		{
			if (!CreateContext(APP_TYPE_ALL, platformId))
				return false;
		}

		context = _allContext;
		break;
	}

	return true;
}

bool AnimaParallelProgramsManager::CreateContext(const AnimaParallelelProgramType& type, cl_platform_id platformId)
{
	cl_platform_id platform = platformId;
	if (platform == nullptr)
	{
		AnimaArray<cl_platform_id> platforms = GetPlatformIDs();
		if (platforms.size() == 0)
		{
			AnimaLogger::LogMessage("[ERROR] Parallel programs manager: unable to find valid platforms", ANIMA_LOGGER_TEXT_COLOR_RED);
			return false;
		}

		platform = platforms[0];
	}

	switch (type)
	{
	case APP_TYPE_CPU:
	{
		if (_cpuContext != 0)
			return true;

		const cl_context_properties contextProperties[] = {
			CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>(platform),
			0, 0
		};

		AnimaArray<cl_device_id> deviceIds = GetDeviceIDs(platform, APP_TYPE_CPU);
		if (deviceIds.size() == 0)
		{
			AnimaLogger::LogMessage("[ERROR] Parallel programs manager: unable to find devices for CPU platform", ANIMA_LOGGER_TEXT_COLOR_RED);
			return false;
		}

		AInt error = CL_SUCCESS;
		_cpuContext = clCreateContext(contextProperties, deviceIds.size(), &deviceIds[0], nullptr, nullptr, &error);
		if (error == CL_SUCCESS)
		{
			AnimaLogger::LogMessage("[ERROR] Parallel programs manager: unable to create CPU context", ANIMA_LOGGER_TEXT_COLOR_RED);
			return false;
		}

		break;
	}
	case APP_TYPE_GPU:
	{
		if (_gpuContext != 0)
			return true;

		const cl_context_properties contextProperties[] = {
			CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>(platform),
			0, 0
		};

		AnimaArray<cl_device_id> deviceIds = GetDeviceIDs(platform, APP_TYPE_GPU);
		if (deviceIds.size() == 0)
		{
			AnimaLogger::LogMessage("[ERROR] Parallel programs manager: unable to find devices for GPU platform", ANIMA_LOGGER_TEXT_COLOR_RED);
			return false;
		}

		AInt error = CL_SUCCESS;
		_gpuContext = clCreateContext(contextProperties, deviceIds.size(), &deviceIds[0], nullptr, nullptr, &error);
		if (error == CL_SUCCESS)
		{
			AnimaLogger::LogMessage("[ERROR] Parallel programs manager: unable to create GPU context", ANIMA_LOGGER_TEXT_COLOR_RED);
			return false;
		}

		break;
	}
	case APP_TYPE_DEFAULT:
	{
		if (_defaultContext != 0)
			return true;

		const cl_context_properties contextProperties[] = {
			CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>(platform),
			0, 0
		};

		AnimaArray<cl_device_id> deviceIds = GetDeviceIDs(platform, APP_TYPE_DEFAULT);
		if (deviceIds.size() == 0)
		{
			AnimaLogger::LogMessage("[ERROR] Parallel programs manager: unable to find devices for DEFAULT platform", ANIMA_LOGGER_TEXT_COLOR_RED);
			return false;
		}

		AInt error = CL_SUCCESS;
		_defaultContext = clCreateContext(contextProperties, deviceIds.size(), &deviceIds[0], nullptr, nullptr, &error);
		if (error == CL_SUCCESS)
		{
			AnimaLogger::LogMessage("[ERROR] Parallel programs manager: unable to create DEFAULT context", ANIMA_LOGGER_TEXT_COLOR_RED);
			return false;
		}

		break;
	}
	case APP_TYPE_ACCELLERATOR:
	{
		if (_acceleratorContext != 0)
			return true;

		const cl_context_properties contextProperties[] = {
			CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>(platform),
			0, 0
		};

		AnimaArray<cl_device_id> deviceIds = GetDeviceIDs(platform, APP_TYPE_ACCELLERATOR);
		if (deviceIds.size() == 0)
		{
			AnimaLogger::LogMessage("[ERROR] Parallel programs manager: unable to find devices for ACCELERATOR platform", ANIMA_LOGGER_TEXT_COLOR_RED);
			return false;
		}

		AInt error = CL_SUCCESS;
		_acceleratorContext = clCreateContext(contextProperties, deviceIds.size(), &deviceIds[0], nullptr, nullptr, &error);
		if (error == CL_SUCCESS)
		{
			AnimaLogger::LogMessage("[ERROR] Parallel programs manager: unable to create ACCELERATOR context", ANIMA_LOGGER_TEXT_COLOR_RED);
			return false;
		}

		break;
	}
	case APP_TYPE_ALL:
	{
		if (_allContext != 0)
			return true;

		const cl_context_properties contextProperties[] = {
			CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>(platform),
			0, 0
		};

		AnimaArray<cl_device_id> deviceIds = GetDeviceIDs(platform, APP_TYPE_ALL);
		if (deviceIds.size() == 0)
		{
			AnimaLogger::LogMessage("[ERROR] Parallel programs manager: unable to find devices for ALL platform", ANIMA_LOGGER_TEXT_COLOR_RED);
			return false;
		}

		AInt error = CL_SUCCESS;
		_allContext = clCreateContext(contextProperties, deviceIds.size(), &deviceIds[0], nullptr, nullptr, &error);
		if (error == CL_SUCCESS)
		{
			AnimaLogger::LogMessage("[ERROR] Parallel programs manager: unable to create ALL context", ANIMA_LOGGER_TEXT_COLOR_RED);
			return false;
		}

		break;
	}
	}

	return true;
}

END_ANIMA_ENGINE_NAMESPACE
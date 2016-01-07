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

AnimaString AnimaParallelProgramsManager::GetDeviceExtensions(const cl_device_id& device) const
{
	ASizeT size = 0;
	if (clGetDeviceInfo(device, CL_DEVICE_EXTENSIONS, 0, nullptr, &size) != CL_SUCCESS)
	{
		AnimaLogger::LogMessage("[ERROR] Parallel programs manager: unable to get device extensions size", ANIMA_LOGGER_TEXT_COLOR_RED);
		return "";
	}

	AnimaString extensions;
	extensions.resize(size);
	if (clGetDeviceInfo(device, CL_DEVICE_EXTENSIONS, size, const_cast<char*>(extensions.data()), nullptr) != CL_SUCCESS)
	{
		AnimaLogger::LogMessage("[ERROR] Parallel programs manager: unable to get device extensions", ANIMA_LOGGER_TEXT_COLOR_RED);
		return "";
	}

	return extensions;
}

bool AnimaParallelProgramsManager::DeviceHasExtension(const cl_device_id& device, const AnimaString& extension)
{
	AnimaString extensions = GetDeviceExtensions(device);
	if (extension.find(extension) != AnimaString::npos)
		return true;
	return false;
}

bool AnimaParallelProgramsManager::FindContext(cl_context& context, const AnimaParallelelProgramType& type, cl_platform_id platformId, bool graphicsInterop)
{
	context = nullptr;

	AInt count = _contexts.size();
	for (AInt i = 0; i < count; i++)
	{
		AnimaParallelProgramContextInfo* info = &_contexts[i];
		// Controllo che le specifiche del contesto siano uguali
		// Nel caso l'ID della piattaforma passata sia nullptr allora prendo in considerazioe i contesti di tutte le piattaforme installate
		if (info->_graphicsInterop == graphicsInterop && info->_type == type && (info->_platformId == platformId || platformId == nullptr))
		{
			context = info->_context;
			return true;
		}
	}

	return false;
}

bool AnimaParallelProgramsManager::GetContext(cl_context& context, const AnimaParallelelProgramType& type, cl_platform_id platformId, bool tryCreateToIfNotFound, bool graphicsInterop)
{
	// Se il contesto è già presente con le informazioni necessarie lo torno subito
	if (FindContext(context, type, platformId, graphicsInterop))
		return context;

	// Se non ho trovato il contesto lo vado a tentare di creare
	if (CreateContext(type, platformId, graphicsInterop))
		return true;

	return false;
}

bool AnimaParallelProgramsManager::CreateContext(const AnimaParallelelProgramType& type, cl_platform_id platformId, bool graphicsInterop)
{
	// Prima di tutto controllo che il contesto non sia già stato creato, nel qual caso termino immediatamente la funzione
	cl_context context = nullptr;
	if (FindContext(context, type, platformId, graphicsInterop))
		return true;

	// Se non mi viene passata l'ID di una piattaforma specifica vado a cercare la piattaforme installate
	cl_platform_id platform = platformId;
	if (platform == nullptr)
	{
		AnimaArray<cl_platform_id> platforms = GetPlatformIDs();

		// Se non trovo nessuna piattaforma allora non posso utilizzare OpenCL
		if (platforms.size() == 0)
		{
			AnimaLogger::LogMessage("[ERROR] Parallel programs manager: unable to find valid platforms", ANIMA_LOGGER_TEXT_COLOR_RED);
			return false;
		}

		// Trovo la prima piattaforma che ha almeno un device del tipo richiesto
		AInt count = platforms.size();
		for (AInt i = 0; i < count && platform == nullptr; i++)
		{
			if (GetDeviceIDs(platforms[i], type).size() > 0)
				platform = platforms[i];
		}
	}

	// Se la piattaforma è ancora nullptr allora non ho trovato nessuna piattaforma che abbia almeno un device del tipo cercato
	// quindi termino la funzione segnalando un errore
	if (platform == nullptr)
	{
		AnimaLogger::LogMessage("[ERROR] Parallel programs manager: unable to find a platform with the requested device type", ANIMA_LOGGER_TEXT_COLOR_RED);
		return false;
	}
	
	// Cerco di device della piattaforma
	AnimaArray<cl_device_id> deviceIds = GetDeviceIDs(platform, type);

	// Se la piattaforma non ha nessun device del tipo cercato allora non termino la funzione segnalando un errore
	if (deviceIds.size() == 0)
	{
		AnimaLogger::LogMessage("[ERROR] Parallel programs manager: unable to find devices for the selected platform", ANIMA_LOGGER_TEXT_COLOR_RED);
		return false;
	}

	// Nel caso sia stato trovato almeno un device vado a tentare di creare il contesto
	AInt error = CL_SUCCESS;

	if (graphicsInterop)
	{
		#if defined APPLE
			AnimaString extensionString = "cl_apple_gl_sharing";
		#else
			AnimaString extensionString = "cl_khr_gl_sharing";
		#endif

		// Controllo tra i device se ne trovo uno che supporti interoperabilità con la grafica
		cl_device_id device = nullptr;
		AInt count = deviceIds.size();
		for (AInt i = 0; i < count && device == nullptr; i++)
		{
			if (DeviceHasExtension(deviceIds[i], extensionString))
				device = deviceIds[i];
		}

		// Se non è stato trovato un device che supporti interoperabilità con la grafica allora termino la funzione segnalando un errore
		if (device == nullptr)
		{
			AnimaLogger::LogMessage("[ERROR] Parallel programs manager: unable to find a device capable of graphics interoperability for the selected platform", ANIMA_LOGGER_TEXT_COLOR_RED);
			return false;
		}

		#if defined APPLE
		#else
			const cl_context_properties contextProperties[] = {
				CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>(platform),
				CL_WGL_HDC_KHR, reinterpret_cast<cl_context_properties>(wglGetCurrentDC()),
				CL_GL_CONTEXT_KHR, reinterpret_cast<cl_context_properties>(wglGetCurrentContext()),
				0, 0
			};
		#endif

		context = clCreateContext(contextProperties, 1, &deviceIds[0], nullptr, nullptr, &error);
	}
	else
	{
		const cl_context_properties contextProperties[] = {
			CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>(platform),
			0, 0
		};

		context = clCreateContext(contextProperties, 1, &deviceIds[0], nullptr, nullptr, &error);
	}

	if (error != CL_SUCCESS)
	{
		AnimaLogger::LogMessage("[ERROR] Parallel programs manager: unable to create CPU context", ANIMA_LOGGER_TEXT_COLOR_RED);
		return false;
	}

	// Aggiungo alla lista di contesti creati quello nuovo appena creato
	AnimaParallelProgramContextInfo info;
	info._type = type;
	info._platformId = platform;
	info._graphicsInterop = graphicsInterop;
	info._context = context;

	_contexts.push_back(info);

	return true;
}

END_ANIMA_ENGINE_NAMESPACE
//
//  AnimaParallelProgramsManager.h
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#ifndef __Anima__AnimaParallelProgramsManager__
#define __Anima__AnimaParallelProgramsManager__

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaString.h"
#include "AnimaMappedArray.h"
#include "AnimaParallelProgram.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaEngine;

class ANIMA_ENGINE_EXPORT AnimaParallelProgramsManager
{
public:
	AnimaParallelProgramsManager(AnimaEngine* engine);
	~AnimaParallelProgramsManager();
	
	AnimaParallelProgram* CreateProgram(const AnimaString& name, const AnimaParallelProgramContextInfo* contextInfo);
	AnimaParallelProgram* CreateProgram(const AnimaString& name, const AnimaParallelelProgramType& type, cl_platform_id platformId = nullptr, bool graphicsInterop = false);

	AnimaParallelProgram* GetProgram(AUint index);
	AnimaParallelProgram* GetProgramFromName(const AnimaString& name);

	AUint GetPlatformsCount() const;
	AnimaArray<cl_platform_id> GetPlatformIDs() const;
	AnimaArray<cl_platform_id> GetPlatformIDs(const AUint& count) const;
	AnimaString GetPlatformName(const cl_platform_id& platform) const;
	AnimaString GetPlatformVersion(const cl_platform_id& platform) const;

	AUint GetDevicesCount(const cl_platform_id& platform, const AnimaParallelelProgramType& type) const;
	AnimaArray<cl_device_id> GetDeviceIDs(const cl_platform_id& platform, const AnimaParallelelProgramType& type) const;
	AnimaArray<cl_device_id> GetDeviceIDs(const cl_platform_id& platform, const AUint& count, const AnimaParallelelProgramType& type) const;
	AnimaString GetDeviceName(const cl_device_id& device) const;
	AnimaString GetDeviceVersion(const cl_device_id& device) const;
	AnimaString GetDeviceExtensions(const cl_device_id& device) const;
	bool DeviceHasExtension(const cl_device_id& device, const AnimaString& extension);

	AnimaParallelProgramContextInfo* FindContext(const AnimaParallelelProgramType& type, cl_platform_id platformId = nullptr, bool graphicsInterop = false);

	AnimaParallelProgramContextInfo* GetContext(const AnimaParallelelProgramType& type, cl_platform_id platformId = nullptr, bool graphicsInterop = false, bool tryCreateToIfNotFound = true);
	AnimaParallelProgramContextInfo* CreateContext(const AnimaParallelelProgramType& type, cl_platform_id platformId = nullptr, bool graphicsInterop = false);

private:
	void ClearPrograms();
	void ClearContexts();

protected:
	
private:
	AnimaEngine* _engine;
	AnimaMappedArray<AnimaParallelProgram*> _programs;

	AnimaArray<AnimaParallelProgramContextInfo> _contexts;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaParallelProgramsManager__) */

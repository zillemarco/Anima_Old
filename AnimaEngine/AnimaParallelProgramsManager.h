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
	
	AnimaParallelProgram* CreateProgram(const AnimaString& name);

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

	bool GetContext(cl_context& context, const AnimaParallelelProgramType& type, cl_platform_id platformId = nullptr, bool tryCreateIfInvalid = true);
	bool CreateContext(const AnimaParallelelProgramType& type, cl_platform_id platformId = nullptr);

private:
	void ClearPrograms();

protected:
	
private:
	AnimaEngine* _engine;
	AnimaMappedArray<AnimaParallelProgram*> _programs;

	cl_context _cpuContext;
	cl_context _gpuContext;
	cl_context _defaultContext;
	cl_context _acceleratorContext;
	cl_context _allContext;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaParallelProgramsManager__) */

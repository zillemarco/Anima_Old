#ifndef _ANIMA_ENGINE_LIB_H
#define _ANIMA_ENGINE_LIB_H

#if defined ANIMA_ENGINE_EXPORT_ENABLED
extern "C"
{ 
#endif

class AnimaEngine
{
public:
	AnimaEngine();
	~AnimaEngine();

public:
	void HelloWorld();
};

#if defined ANIMA_ENGINE_EXPORT_ENABLED
}
#endif

#endif //_ANIMA_ENGINE_LIB_H